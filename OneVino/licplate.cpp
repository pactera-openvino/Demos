#include "licplate.h"
#include "ui_licplate.h"

#include <algorithm>
#include <iomanip>
#include <list>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <set>

#include <inference_engine.hpp>
#include <vpu/vpu_plugin_config.hpp>
#include <ext_list.hpp>
#include <samples/ocv_common.hpp>
#include <samples/args_helper.hpp>

#include "common.hpp"
#include "grid_mat.hpp"
#include "input_wrappers.hpp"
#include "net_wrappers.hpp"

#include "interactive_face_detection.hpp"

using namespace std;
using namespace InferenceEngine;

#define MAX_LPR_LIST_LEN    15

cv::Mat g_ShowMat;
QList <QString> g_lprlist;

LpRecognitionThread *lpr_thread;

extern QImage MatToQImage(const cv::Mat& mat);

void LicPlate::RefreshPic(void)
{
    if(g_ShowMat.empty())
        return;
    QPixmap pixmap =QPixmap::fromImage(MatToQImage(g_ShowMat));
    QSize picSize(ui->VideoLable->width(),ui->VideoLable->height());
    this->ui->VideoLable->setPixmap(pixmap.scaled(picSize));

    if(!g_lprlist.empty())
    {
        if(g_lprlist.count(g_lprlist.first()) == MAX_LPR_LIST_LEN)
        {
            //cout << "continue recongtion plate : " << g_lprlist.first().toStdString() << endl;
            QList<QStandardItem*> list = LprListModel->findItems(g_lprlist.first() , Qt::MatchExactly , 0);
            if(list.size() <= 0)
            {
                LprListModel->setRowCount(LprListModel->rowCount());
                LprListModel->appendRow(new QStandardItem(g_lprlist.first()));
                cout << "add License plate " << g_lprlist.first().toStdString() << " to list"<<endl;
                //LprListModel->removeRow(list[0]->row(),QModelIndex());
            }
        }
    }

}

LicPlate::LicPlate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicPlate)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    showMaximized(); //默认最大化


    ui->LprList->setModel(LprListModel);

    lpr_thread = new (LpRecognitionThread);
    lpr_thread->start();

    //设置定时器是否为单次触发。默认为 false 多次触发
    m_refresh->setSingleShot(false);
    //启动或重启定时器, 并设置定时器时间：毫秒
    m_refresh->start(30);
    //定时器触发信号槽
    connect(m_refresh, SIGNAL(timeout()), this, SLOT(RefreshPic()));
}

LicPlate::~LicPlate()
{
    delete ui;
    lpr_thread->stop();
}





typedef std::chrono::duration<double, std::ratio<1, 1000>> ms;

struct BboxAndDescr {
    enum class ObjectType {
        NONE,
        VEHICCLE,
        PLATE,
    } objectType;
    cv::Rect rect;
    std::string descr;
};

struct InferRequestsContainer {
    explicit InferRequestsContainer(std::vector<InferRequest> inferRequests):
        actualInferRequests{inferRequests} {
        for (auto& ir : actualInferRequests) {
            inferRequests.push_back(ir);
        }
    }
    InferRequestsContainer() = default;
    InferRequestsContainer& operator=(const InferRequestsContainer& other) {  // copy assignment
        if (this != &other) {  // self-assignment check expected
            this->actualInferRequests = other.actualInferRequests;
            for (auto& ir : this->actualInferRequests) {
                this->inferRequests.container.push_back(ir);
            }
        }
        return *this;
    }
    std::vector<InferRequest> getActualInferRequests() {
        return actualInferRequests;
    }
    ConcurrentContainer<std::vector<std::reference_wrapper<InferRequest>>> inferRequests;

private:
    std::vector<InferRequest> actualInferRequests;
};

struct Context {  // stores all global data for tasks
    Context(const std::vector<std::shared_ptr<InputChannel>>& inputChannels, const std::weak_ptr<Worker>& readersWorker,
            const Detector& detector, const std::weak_ptr<Worker>& inferTasksWorker,
            const VehicleAttributesClassifier& vehicleAttributesClassifier, const Lpr& lpr, const std::weak_ptr<Worker>& detectionsProcessorsWorker,
            int pause, const std::vector<cv::Size>& gridParam, cv::Size displayResolution, std::chrono::steady_clock::duration showPeriod,
                const std::weak_ptr<Worker>& drawersWorker,
            uint64_t lastFrameId,
            const std::weak_ptr<Worker> resAggregatorsWorker,
            uint64_t nireq,
            bool isVideo,
            std::size_t nclassifiersireq, std::size_t nrecognizersireq):
        readersContext{inputChannels, readersWorker, std::vector<int64_t>(inputChannels.size(), -1), std::vector<std::mutex>(inputChannels.size())},
        inferTasksContext{detector, inferTasksWorker},
        detectionsProcessorsContext{vehicleAttributesClassifier, lpr, detectionsProcessorsWorker},
        drawersContext{pause, gridParam, displayResolution, showPeriod, drawersWorker},
        videoFramesContext{std::vector<uint64_t>(inputChannels.size(), lastFrameId), std::vector<std::mutex>(inputChannels.size())},
        resAggregatorsWorker{resAggregatorsWorker},
        nireq{nireq},
        isVideo{isVideo},
        t0{std::chrono::steady_clock::time_point()},
        freeDetectionInfersCount{0},
        frameCounter{0}
    {
        assert(inputChannels.size() == gridParam.size());
        std::vector<InferRequest> detectorInferRequests;
        std::vector<InferRequest> attributesInferRequests;
        std::vector<InferRequest> lprInferRequests;
        detectorInferRequests.reserve(nireq);
        attributesInferRequests.reserve(nclassifiersireq);
        lprInferRequests.reserve(nrecognizersireq);
        std::generate_n(std::back_inserter(detectorInferRequests), nireq, [&]{
            return inferTasksContext.detector.createInferRequest();});
        std::generate_n(std::back_inserter(attributesInferRequests), nclassifiersireq, [&]{
            return detectionsProcessorsContext.vehicleAttributesClassifier.createInferRequest();});
        std::generate_n(std::back_inserter(lprInferRequests), nrecognizersireq, [&]{
            return detectionsProcessorsContext.lpr.createInferRequest();});
        detectorsInfers = InferRequestsContainer(detectorInferRequests);
        attributesInfers = InferRequestsContainer(attributesInferRequests);
        platesInfers = InferRequestsContainer(lprInferRequests);
    }
    struct {
        std::vector<std::shared_ptr<InputChannel>> inputChannels;
        std::weak_ptr<Worker> readersWorker;
        std::vector<int64_t> lastCapturedFrameIds;
        std::vector<std::mutex> lastCapturedFrameIdsMutexes;
    } readersContext;
    struct {
        Detector detector;
        std::weak_ptr<Worker> inferTasksWorker;
    } inferTasksContext;
    struct {
        VehicleAttributesClassifier vehicleAttributesClassifier;
        Lpr lpr;
        std::weak_ptr<Worker> detectionsProcessorsWorker;
    } detectionsProcessorsContext;
    struct DrawersContext {
        DrawersContext(int pause, const std::vector<cv::Size>& gridParam, cv::Size displayResolution, std::chrono::steady_clock::duration showPeriod,
                       const std::weak_ptr<Worker>& drawersWorker):
            pause{pause}, gridParam{gridParam}, displayResolution{displayResolution}, showPeriod{showPeriod}, drawersWorker{drawersWorker},
            lastShownframeId{0}, prevShow{std::chrono::steady_clock::time_point()} {}
        int pause;
        std::vector<cv::Size> gridParam;
        cv::Size displayResolution;
        std::chrono::steady_clock::duration showPeriod;  // desiered frequency of imshow
        std::weak_ptr<Worker> drawersWorker;
        int64_t lastShownframeId;
        std::chrono::steady_clock::time_point prevShow;  // time stamp of previous imshow
        std::map<int64_t, GridMat> gridMats;
        std::mutex drawerMutex;
    } drawersContext;
    struct {
        std::vector<uint64_t> lastframeIds;
        std::vector<std::mutex> lastFrameIdsMutexes;
    } videoFramesContext;
    std::weak_ptr<Worker> resAggregatorsWorker;
    uint64_t nireq;
    bool isVideo;
    std::chrono::steady_clock::time_point t0;
    std::atomic<std::vector<InferRequest>::size_type> freeDetectionInfersCount;
    std::atomic<uint64_t> frameCounter;
    InferRequestsContainer detectorsInfers, attributesInfers, platesInfers;
};

class ReborningVideoFrame: public VideoFrame {
public:
    ReborningVideoFrame(Context& context, const unsigned sourceID, const int64_t frameId, const cv::Mat& frame = cv::Mat()) :
        VideoFrame{sourceID, frameId, frame}, context(context) {}  // can not write context{context} because of CentOS 7.4 compiler bug
    virtual ~ReborningVideoFrame();
    Context& context;
};

class Drawer: public Task {  // accumulates and shows processed frames
public:
    explicit Drawer(VideoFrame::Ptr sharedVideoFrame):
        Task{sharedVideoFrame, 1.0} {}
    bool isReady() override;
    void process() override;
};

class ResAggregator: public Task {  // draws results on the frame
public:
    ResAggregator(const VideoFrame::Ptr& sharedVideoFrame, std::list<BboxAndDescr>&& boxesAndDescrs):
        Task{sharedVideoFrame, 4.0}, boxesAndDescrs{std::move(boxesAndDescrs)} {}
    bool isReady() override {
        return true;
    }
    void process() override;
private:
    std::list<BboxAndDescr> boxesAndDescrs;
};

class ClassifiersAggreagator {  // waits for all classifiers and recognisers accumulating results
public:
    std::string rawDetections;
    ConcurrentContainer<std::list<std::string>> rawAttributes;
    ConcurrentContainer<std::list<std::string>> rawDecodedPlates;

    explicit ClassifiersAggreagator(const VideoFrame::Ptr& sharedVideoFrame):
        sharedVideoFrame{sharedVideoFrame} {}
    ~ClassifiersAggreagator() {
        static std::mutex printMutex;
        printMutex.lock();
        std::cout << rawDetections;
        for (const std::string& rawAttribute : rawAttributes.container) {  // destructor assures that none uses the container
            std::cout << rawAttribute;
        }
        for (const std::string& rawDecodedPlate : rawDecodedPlates.container) {
            std::cout << rawDecodedPlate;
        }
        printMutex.unlock();
        tryPush(static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context.resAggregatorsWorker,
                std::make_shared<ResAggregator>(sharedVideoFrame, std::move(boxesAndDescrs)));
    }
    void push(BboxAndDescr&& bboxAndDescr) {
        boxesAndDescrs.lockedPush_back(std::move(bboxAndDescr));
    }
    const VideoFrame::Ptr sharedVideoFrame;

private:
    ConcurrentContainer<std::list<BboxAndDescr>> boxesAndDescrs;
};

class DetectionsProcessor: public Task {  // extracts detections from blob InferRequests and runs classifiers and recognisers
public:
    DetectionsProcessor(VideoFrame::Ptr sharedVideoFrame, InferRequest* inferRequest):
        Task{sharedVideoFrame, 1.0}, inferRequest{inferRequest}, requireGettingNumberOfDetections{true} {}
    DetectionsProcessor(VideoFrame::Ptr sharedVideoFrame, std::shared_ptr<ClassifiersAggreagator>&& classifiersAggreagator, std::list<cv::Rect>&& vehicleRects,
    std::list<cv::Rect>&& plateRects):
        Task{sharedVideoFrame, 1.0}, classifiersAggreagator{std::move(classifiersAggreagator)}, inferRequest{nullptr},
        vehicleRects{std::move(vehicleRects)}, plateRects{std::move(plateRects)}, requireGettingNumberOfDetections{false} {}
    bool isReady() override;
    void process() override;

private:
    std::shared_ptr<ClassifiersAggreagator> classifiersAggreagator;  // when no one stores this object we will draw
    InferRequest* inferRequest;
    std::list<cv::Rect> vehicleRects;
    std::list<cv::Rect> plateRects;
    std::vector<std::reference_wrapper<InferRequest>> reservedAttributesRequests;
    std::vector<std::reference_wrapper<InferRequest>> reservedLprRequests;
    bool requireGettingNumberOfDetections;
};

class InferTask: public Task {  // runs detection
public:
    explicit InferTask(VideoFrame::Ptr sharedVideoFrame):
        Task{sharedVideoFrame, 5.0} {}
    bool isReady() override;
    void process() override;
};

class Reader: public Task {
public:
    explicit Reader(VideoFrame::Ptr sharedVideoFrame):
        Task{sharedVideoFrame, 2.0} {}
    bool isReady() override;
    void process() override;
};

ReborningVideoFrame::~ReborningVideoFrame() {
    try {
        const std::shared_ptr<Worker>& worker = std::shared_ptr<Worker>(context.readersContext.readersWorker);
        context.videoFramesContext.lastFrameIdsMutexes[sourceID].lock();
        const auto frameId = ++context.videoFramesContext.lastframeIds[sourceID];
        context.videoFramesContext.lastFrameIdsMutexes[sourceID].unlock();
        std::shared_ptr<ReborningVideoFrame> reborn = std::make_shared<ReborningVideoFrame>(context, sourceID, frameId, frame);
        worker->push(std::make_shared<Reader>(reborn));
    } catch (const std::bad_weak_ptr&) {}
}

bool Drawer::isReady() {
    Context& context = static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context;
    std::chrono::steady_clock::time_point prevShow = context.drawersContext.prevShow;
    std::chrono::steady_clock::duration showPeriod = context.drawersContext.showPeriod;
    if (1u == context.drawersContext.gridParam.size()) {
        if (std::chrono::steady_clock::now() - prevShow > showPeriod) {
            return true;
        } else {
            return false;
        }
    } else {
        std::map<int64_t, GridMat>& gridMats = context.drawersContext.gridMats;
        auto gridMatIt = gridMats.find(sharedVideoFrame->frameId);
        if (gridMats.end() == gridMatIt) {
            if (2 > gridMats.size()) {  // buffer size
                return true;
            } else {
                return false;
            }
        } else {
            if (1u == gridMatIt->second.getUnupdatedSourceIDs().size()) {
                if (context.drawersContext.lastShownframeId == sharedVideoFrame->frameId
                    && std::chrono::steady_clock::now() - prevShow > showPeriod) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }
    }
}

void Drawer::process() {
    const int64_t frameId = sharedVideoFrame->frameId;
    Context& context = static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context;
    std::map<int64_t, GridMat>& gridMats = context.drawersContext.gridMats;
    context.drawersContext.drawerMutex.lock();
    auto gridMatIt = gridMats.find(frameId);
    if (gridMats.end() == gridMatIt) {
        gridMatIt = gridMats.emplace(frameId, GridMat(context.drawersContext.gridParam,
                                                      context.drawersContext.displayResolution)).first;
    }

    gridMatIt->second.update(sharedVideoFrame->frame, sharedVideoFrame->sourceID);
    auto firstGridIt = gridMats.begin();
    int64_t& lastShownframeId = context.drawersContext.lastShownframeId;
    if (firstGridIt->first == lastShownframeId && firstGridIt->second.isFilled()) {
        lastShownframeId++;
        cv::Mat mat = firstGridIt->second.getMat();

        //float opacity = 0.6f;
        //fillROIColor(mat, cv::Rect(5, 5, 700, 115), cv::Scalar(255, 0, 0), opacity);
/*
        std::ostringstream out;
        out << "Mean overall time per all inputs: " << std::fixed << std::setprecision(2) << std::setw(6);
        const auto t1 = std::chrono::steady_clock::now();
        uint64_t frameCounter = context.frameCounter;
        const ms meanOverallTimePerAllInputs = std::chrono::duration_cast<ms>((t1 - context.t0)
                                               * context.readersContext.inputChannels.size()) / frameCounter;
        out << meanOverallTimePerAllInputs.count();
        out << "ms /" << std::setw(6) << std::chrono::seconds(1) / meanOverallTimePerAllInputs << "FPS";

        cv::putText(mat, out.str(), cv::Point2f(10, 35), cv::FONT_HERSHEY_TRIPLEX, 0.7, cv::Scalar{255, 255, 255});
        cv::putText(mat, "Detection InferRequests usage", cv::Point2f(10, 70), cv::FONT_HERSHEY_TRIPLEX, 0.7, cv::Scalar{255, 255, 255});
        cv::Rect usage(15, 90, 400, 20);
        cv::rectangle(mat, usage, {0, 255, 0}, 2);
        uint64_t nireq = context.nireq;
        usage.width = static_cast<int>(usage.width * static_cast<float>(frameCounter * nireq - context.freeDetectionInfersCount) / (frameCounter * nireq));
        cv::rectangle(mat, usage, {0, 255, 0}, cv::FILLED);
*/
        g_ShowMat = mat.clone();


        QThread::msleep(30);
        //cv::imshow("Detection results", firstGridIt->second.getMat());
        //context.drawersContext.prevShow = std::chrono::steady_clock::now();
        //const int key = cv::waitKey(context.drawersContext.pause);
       /* if (key == 27 || 'q' == key || 'Q' == key || !context.isVideo) {
            try {
                std::shared_ptr<Worker>(context.drawersContext.drawersWorker)->stop();
            } catch (const std::bad_weak_ptr&) {}
        } else if (key == 32) {
            context.drawersContext.pause = (context.drawersContext.pause + 1) & 1;
        }*/
        firstGridIt->second.clear();
        gridMats.emplace((--gridMats.end())->first + 1, firstGridIt->second);
        gridMats.erase(firstGridIt);
    }
    context.drawersContext.drawerMutex.unlock();
}

void ResAggregator::process() {
    Context& context = static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context;
    context.freeDetectionInfersCount += context.detectorsInfers.inferRequests.lockedSize();
    context.frameCounter++;
    if (!FLAGS_no_show) {
        for (const BboxAndDescr& bboxAndDescr : boxesAndDescrs) {
            switch (bboxAndDescr.objectType) {
                case BboxAndDescr::ObjectType::NONE: cv::rectangle(sharedVideoFrame->frame, bboxAndDescr.rect, {255, 255, 0},  4);
                                                     break;
                case BboxAndDescr::ObjectType::VEHICCLE: cv::rectangle(sharedVideoFrame->frame, bboxAndDescr.rect, {0, 255, 0},  4);
                                                         /*cv::putText(sharedVideoFrame->frame, bboxAndDescr.descr,
                                                                     cv::Point{bboxAndDescr.rect.x, bboxAndDescr.rect.y + 35},
                                                                     cv::FONT_HERSHEY_COMPLEX, 1.3, cv::Scalar(0, 255, 0), 4);*/
                                                         break;
                case BboxAndDescr::ObjectType::PLATE: cv::rectangle(sharedVideoFrame->frame, bboxAndDescr.rect, {0, 0, 255},  4);
                                                      /*cv::putText(sharedVideoFrame->frame, bboxAndDescr.descr,
                                                                  cv::Point{bboxAndDescr.rect.x, bboxAndDescr.rect.y - 10},
                                                                  cv::FONT_HERSHEY_COMPLEX, 1.3, cv::Scalar(0, 0, 255), 4);*/
                                                      //std::cout << "车牌：" << bboxAndDescr.descr << std::endl;
                                                      if(g_lprlist.size() == MAX_LPR_LIST_LEN)
                                                          g_lprlist.removeFirst();
                                                      g_lprlist.push_back(QString::fromStdString(bboxAndDescr.descr));
                                                      break;
                default: throw std::exception();  // must never happen
                          break;
            }
        }
        tryPush(context.drawersContext.drawersWorker, std::make_shared<Drawer>(sharedVideoFrame));
    } else {
        if (!context.isVideo) {
           try {
                std::shared_ptr<Worker>(context.drawersContext.drawersWorker)->stop();
            } catch (const std::bad_weak_ptr&) {}
        }
    }
}

bool DetectionsProcessor::isReady() {
    Context& context = static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context;
    if (requireGettingNumberOfDetections) {
        classifiersAggreagator = std::make_shared<ClassifiersAggreagator>(sharedVideoFrame);
        std::list<Detector::Result> results;
        if (!(FLAGS_r && ((sharedVideoFrame->frameId == 0 && !context.isVideo) || context.isVideo))) {
            results = context.inferTasksContext.detector.getResults(*inferRequest, sharedVideoFrame->frame.size());
        } else {
            std::ostringstream rawResultsStream;
            results = context.inferTasksContext.detector.getResults(*inferRequest, sharedVideoFrame->frame.size(), &rawResultsStream);
            classifiersAggreagator->rawDetections = rawResultsStream.str();
        }
        for (Detector::Result result : results) {
            switch (result.label) {
                case 1:
                {
                    vehicleRects.emplace_back(result.location & cv::Rect{cv::Point(0, 0), sharedVideoFrame->frame.size()});
                    break;
                }
                case 2:
                {
                    // expanding a bounding box a bit, better for the license plate recognition
                    result.location.x -= 5;
                    result.location.y -= 5;
                    result.location.width += 10;
                    result.location.height += 10;
                    plateRects.emplace_back(result.location & cv::Rect{cv::Point(0, 0), sharedVideoFrame->frame.size()});
                    break;
                }
                default: throw std::exception();  // must never happen
                         break;
            }
        }
        context.detectorsInfers.inferRequests.lockedPush_back(*inferRequest);
        requireGettingNumberOfDetections = false;
    }

    if (plateRects.empty()) {
        return true;
    } else {
        // isReady() is called under mutexes so it is assured that available InferRequests will not be taken, but new InferRequests can come in
        // acquire as many InferRequests as it is possible or needed
        InferRequestsContainer& attributesInfers = context.attributesInfers;
        attributesInfers.inferRequests.mutex.lock();
        const std::size_t numberOfAttributesInferRequestsAcquired = std::min(vehicleRects.size(), attributesInfers.inferRequests.container.size());
        reservedAttributesRequests.assign(attributesInfers.inferRequests.container.end() - numberOfAttributesInferRequestsAcquired,
                                          attributesInfers.inferRequests.container.end());
        attributesInfers.inferRequests.container.erase(attributesInfers.inferRequests.container.end() - numberOfAttributesInferRequestsAcquired,
                                                       attributesInfers.inferRequests.container.end());
        attributesInfers.inferRequests.mutex.unlock();

        InferRequestsContainer& platesInfers = context.platesInfers;
        platesInfers.inferRequests.mutex.lock();
        const std::size_t numberOfLprInferRequestsAcquired = std::min(plateRects.size(), platesInfers.inferRequests.container.size());
        reservedLprRequests.assign(platesInfers.inferRequests.container.end() - numberOfLprInferRequestsAcquired, platesInfers.inferRequests.container.end());
        platesInfers.inferRequests.container.erase(platesInfers.inferRequests.container.end() - numberOfLprInferRequestsAcquired,
                                                   platesInfers.inferRequests.container.end());
        platesInfers.inferRequests.mutex.unlock();
        return numberOfAttributesInferRequestsAcquired || numberOfLprInferRequestsAcquired;
    }
}

void DetectionsProcessor::process() {
    Context& context = static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context;

    for (const cv::Rect vehicleRect : vehicleRects) {
        classifiersAggreagator->push(BboxAndDescr{BboxAndDescr::ObjectType::NONE, vehicleRect, ""});
    }
    vehicleRects.clear();

    auto plateRectsIt = plateRects.begin();
    for (auto lprRequestsIt = reservedLprRequests.begin(); lprRequestsIt != reservedLprRequests.end(); plateRectsIt++, lprRequestsIt++) {
        const cv::Rect plateRect = *plateRectsIt;
        InferRequest& lprRequest = *lprRequestsIt;
        context.detectionsProcessorsContext.lpr.setImage(lprRequest, sharedVideoFrame->frame, plateRect);

        lprRequest.SetCompletionCallback(
            std::bind(
                [](std::shared_ptr<ClassifiersAggreagator> classifiersAggreagator,
                    InferRequest& lprRequest,
                    cv::Rect rect,
                    Context& context) {
                        lprRequest.SetCompletionCallback([]{});  // destroy the stored bind object

                        std::string result = context.detectionsProcessorsContext.lpr.getResults(lprRequest);

                        if (FLAGS_r && ((classifiersAggreagator->sharedVideoFrame->frameId == 0 && !context.isVideo) || context.isVideo)) {
                            classifiersAggreagator->rawDecodedPlates.lockedPush_back("License Plate Recognition results:" + result + '\n');
                        }
                        classifiersAggreagator->push(BboxAndDescr{BboxAndDescr::ObjectType::PLATE, rect, std::move(result)});
                        context.platesInfers.inferRequests.lockedPush_back(lprRequest);
                    }, classifiersAggreagator,
                       std::ref(lprRequest),
                       plateRect,
                       std::ref(context)));

        lprRequest.StartAsync();
    }
    plateRects.erase(plateRects.begin(), plateRectsIt);

    if (!vehicleRects.empty() || !plateRects.empty()) {
        tryPush(context.detectionsProcessorsContext.detectionsProcessorsWorker,
            std::make_shared<DetectionsProcessor>(sharedVideoFrame, std::move(classifiersAggreagator), std::move(vehicleRects), std::move(plateRects)));
    }
}

bool InferTask::isReady() {
    InferRequestsContainer& detectorsInfers = static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context.detectorsInfers;
    if (detectorsInfers.inferRequests.container.empty()) {
        return false;
    } else {
        detectorsInfers.inferRequests.mutex.lock();
        if (detectorsInfers.inferRequests.container.empty()) {
            detectorsInfers.inferRequests.mutex.unlock();
            return false;
        } else {
            return true;  // process() will unlock the mutex
        }
    }
}

void InferTask::process() {
    Context& context = static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context;
    InferRequestsContainer& detectorsInfers = context.detectorsInfers;
    std::reference_wrapper<InferRequest> inferRequest = detectorsInfers.inferRequests.container.back();
    detectorsInfers.inferRequests.container.pop_back();
    detectorsInfers.inferRequests.mutex.unlock();

    context.inferTasksContext.detector.setImage(inferRequest, sharedVideoFrame->frame);

    inferRequest.get().SetCompletionCallback(
        std::bind(
            [](VideoFrame::Ptr sharedVideoFrame,
               InferRequest& inferRequest,
               Context& context) {
                    inferRequest.SetCompletionCallback([]{});  // destroy the stored bind object
                    tryPush(context.detectionsProcessorsContext.detectionsProcessorsWorker,
                        std::make_shared<DetectionsProcessor>(sharedVideoFrame, &inferRequest));
                }, sharedVideoFrame,
                   inferRequest,
                   std::ref(context)));
    inferRequest.get().StartAsync();
    // do not push as callback does it
}

bool Reader::isReady() {
    Context& context = static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context;
    context.readersContext.lastCapturedFrameIdsMutexes[sharedVideoFrame->sourceID].lock();
    if (context.readersContext.lastCapturedFrameIds[sharedVideoFrame->sourceID] + 1 == sharedVideoFrame->frameId) {
        return true;
    } else {
        context.readersContext.lastCapturedFrameIdsMutexes[sharedVideoFrame->sourceID].unlock();
        return false;
    }
}

void Reader::process() {
    unsigned sourceID = sharedVideoFrame->sourceID;
    Context& context = static_cast<ReborningVideoFrame*>(sharedVideoFrame.get())->context;
    const std::vector<std::shared_ptr<InputChannel>>& inputChannels = context.readersContext.inputChannels;
    if (inputChannels[sourceID]->read(sharedVideoFrame->frame)) {
        context.readersContext.lastCapturedFrameIds[sourceID]++;
        context.readersContext.lastCapturedFrameIdsMutexes[sourceID].unlock();
        tryPush(context.inferTasksContext.inferTasksWorker, std::make_shared<InferTask>(sharedVideoFrame));
    } else {
        context.readersContext.lastCapturedFrameIds[sourceID]++;
        context.readersContext.lastCapturedFrameIdsMutexes[sourceID].unlock();
        try {
            std::shared_ptr<Worker>(context.drawersContext.drawersWorker)->stop();
        } catch (const std::bad_weak_ptr&) {}
    }
}

LpRecognitionThread::LpRecognitionThread()
{
    LprRunning = true;
}

void LpRecognitionThread::run()
{
    std::vector<std::string> files;

    files.push_back(LICPLATEFILE);
    //files.push_back("/root/OneVino/2.mp4");
    //files.push_back("/dev/video0");
    //files.push_back("/opt/intel/openvino/deployment_tools/demo/car_1.bmp");
    if (files.empty()) throw std::logic_error("No files were found");
    std::vector<std::shared_ptr<VideoCaptureSource>> videoCapturSourcess;
    std::vector<std::shared_ptr<ImageSource>> imageSourcess;
    if (FLAGS_nc) {
        for (size_t i = 0; i < FLAGS_nc; ++i) {
            cv::VideoCapture videoCapture(i);
            if (!videoCapture.isOpened()) {
                slog::info << "Cannot open web cam [" << i << "]" << slog::endl;
                return;
            }
            videoCapture.set(cv::CAP_PROP_FPS , 30);
            videoCapturSourcess.push_back(std::make_shared<VideoCaptureSource>(videoCapture, FLAGS_loop_video));
        }
    }
    for (const std::string& file : files) {
        cv::Mat frame = cv::imread(file, cv::IMREAD_COLOR);
        if (frame.empty()) {
            cv::VideoCapture videoCapture(file);
            if (!videoCapture.isOpened()) {
                slog::info << "Cannot open " << file << slog::endl;
                return;
            }
            videoCapturSourcess.push_back(std::make_shared<VideoCaptureSource>(videoCapture, FLAGS_loop_video));
        } else {
            imageSourcess.push_back(std::make_shared<ImageSource>(frame, true));
        }
    }
    uint32_t channelsNum = 0 == FLAGS_ni ? videoCapturSourcess.size() + imageSourcess.size() : FLAGS_ni;

    std::vector<std::shared_ptr<IInputSource>> inputSources;
    inputSources.reserve(videoCapturSourcess.size() + imageSourcess.size());
    for (const std::shared_ptr<VideoCaptureSource>& videoSource : videoCapturSourcess) {
        inputSources.push_back(videoSource);
    }
    for (const std::shared_ptr<ImageSource>& imageSource : imageSourcess) {
        inputSources.push_back(imageSource);
    }

    std::vector<std::shared_ptr<InputChannel>> inputChannels;
    inputChannels.reserve(channelsNum);
    for (decltype(inputSources.size()) channelI = 0, counter = 0; counter < channelsNum; channelI++, counter++) {
        if (inputSources.size() == channelI) {
            channelI = 0;
        }
        inputChannels.push_back(InputChannel::create(inputSources[channelI]));
    }

    InferenceEngine::Core ie;

    std::set<std::string> loadedDevices;
    std::vector<std::string> pluginNames = {
            FLAGS_d,
            FLAGS_d_va,
            FLAGS_d_lpr
    };

    for (auto && flag : pluginNames) {
        if (flag.empty())
            continue;
        if (loadedDevices.find(flag) != loadedDevices.end())
            continue;

        slog::info << "Loading device " << flag << slog::endl;

        /** Printing device version **/
        std::cout << ie.GetVersions(flag) << std::endl;

        if ((flag.find("CPU") != std::string::npos)) {
            /** Load default extensions lib for the CPU device (e.g. SSD's DetectionOutput)**/
            ie.AddExtension(std::make_shared<Extensions::Cpu::CpuExtensions>(), "CPU");

            auto extension_ptr = make_so_pointer<IExtension>(FLAGS_l);
            ie.AddExtension(extension_ptr, "CPU");
            slog::info << "CPU Extension loaded: " << FLAGS_l << slog::endl;

            if (inputChannels.size() > 1) {
                ie.SetConfig({{PluginConfigParams::KEY_CPU_THROUGHPUT_STREAMS, PluginConfigParams::CPU_THROUGHPUT_AUTO}}, "CPU");
            }
        }

        loadedDevices.insert(flag);
    }
    /** Per layer metrics **/
    std::map<std::string, std::string> mapDevices;
    if (FLAGS_pc) {
        ie.SetConfig({{PluginConfigParams::KEY_PERF_COUNT, PluginConfigParams::YES}});
        mapDevices = getMapFullDevicesNames(ie, pluginNames);
    }

    /** Graph tagging via config options**/
    auto makeTagConfig = [&](const std::string &deviceName, const std::string &suffix) {
        std::map<std::string, std::string> config;
        if (FLAGS_tag && deviceName == "HDDL") {
            config[VPU_HDDL_CONFIG_KEY(GRAPH_TAG)] = "tag" + suffix;
        }
        return config;
    };
    slog::info << FLAGS_mp << slog::endl;
    // -----------------------------------------------------------------------------------------------------
    unsigned nireq = FLAGS_nireq == 0 ? inputChannels.size() : FLAGS_nireq;
    slog::info << "Loading detection model to the "<< FLAGS_d << " plugin" << slog::endl;
    Detector detector(ie, FLAGS_d, FLAGS_mp,
        {static_cast<float>(FLAGS_t), static_cast<float>(FLAGS_t)}, FLAGS_auto_resize, makeTagConfig(FLAGS_d, "Detect"));
    VehicleAttributesClassifier vehicleAttributesClassifier;
    std::size_t nclassifiersireq{0};
    Lpr lpr;
    std::size_t nrecognizersireq{0};

    slog::info << "Loading Licence Plate Recognition (LPR) model to the "<< FLAGS_d_lpr << " plugin" << slog::endl;
    lpr = Lpr(ie, FLAGS_d_lpr, FLAGS_m_lpr, FLAGS_auto_resize, makeTagConfig(FLAGS_d_lpr, "LPR"));
    nrecognizersireq = nireq * 3;

    std::shared_ptr<Worker> worker = std::make_shared<Worker>(FLAGS_n_wt - 1);
    bool isVideo = imageSourcess.empty() ? true : false;
    int pause = imageSourcess.empty() ? 1 : 0;
    std::chrono::steady_clock::duration showPeriod = 0 == FLAGS_fps ? std::chrono::steady_clock::duration::zero()
        : std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds{1}) / FLAGS_fps;
    std::vector<cv::Size> gridParam;
    gridParam.reserve(inputChannels.size());
    for (const auto& inputChannel : inputChannels) {
        gridParam.emplace_back(inputChannel->getSize());
    }

    cv::Size displayResolution = cv::Size{1920,
                                          1080};
    slog::info << "Number of InferRequests: " << nireq << slog::endl;

    Context context{inputChannels, worker,
                    detector, worker,
                    vehicleAttributesClassifier, lpr, worker,
                    pause, gridParam, displayResolution, showPeriod, worker,
                    FLAGS_n_iqs - 1,
                    worker,
                    nireq,
                    isVideo,
                    nclassifiersireq, nrecognizersireq};

    for (uint64_t i = 0; i < FLAGS_n_iqs; i++) {
        for (unsigned sourceID = 0; sourceID < inputChannels.size(); sourceID++) {
            VideoFrame::Ptr sharedVideoFrame = std::make_shared<ReborningVideoFrame>(context, sourceID, i);
            worker->push(std::make_shared<Reader>(sharedVideoFrame));
        }
    }
    slog::info << "Number of allocated frames: " << FLAGS_n_iqs * (inputChannels.size()) << slog::endl;
    if (FLAGS_auto_resize) {
        slog::info << "Resizable input with support of ROI crop and auto resize is enabled" << slog::endl;
    } else {
        slog::info << "Resizable input with support of ROI crop and auto resize is disabled" << slog::endl;
    }

    // Running
    context.t0 = std::chrono::steady_clock::now();
    worker->runThreads();
    worker->threadFunc();
    worker->join();

    std::cout << "thread exit!!!" << std::endl;
}

void LpRecognitionThread::stop()
{
    LprRunning = false;
}
