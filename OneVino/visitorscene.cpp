#include "visitorscene.h"
#include "ui_visitorscene.h"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include <QDebug>
#include <functional>
#include <iostream>
#include <fstream>
#include <random>
#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iterator>
#include <map>
#include <list>
#include <set>
#include <QDateTime>

#include <inference_engine.hpp>

#include <samples/ocv_common.hpp>
#include <samples/slog.hpp>

#include "face_reid.hpp"
#include "database.hpp"

#include "interactive_face_detection.hpp"

using namespace std;
using namespace cv;
using namespace InferenceEngine;

visitorscene::visitorscene(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::visitorscene)
{
    ui->setupUi(this);

    //setWindowModality(Qt::ApplicationModal);
    //showMaximized();
    showFullScreen();
/*
    ui->DetailView->setStyleSheet("QTableView{background:transparent;border:1px solid #A7 A7 A7;border-radius:1px;font-size:16px;font-family:'黑体';gridline-color:#A7 A7 A7}"\
            "QTableWidget QTableCornerButton::section{background:transparent;}"\
            "QHeaderView{background:rgb(68,83,101);border-radius:1px;height:35;font-size:16px;font-family:'黑体'}"\
            "QHeaderView::section{background:rgb(68,83,101);padding:5px 10px 5px 10px;border-radius:1px;}"\
            "QTableView:item{background:transparent;color:#00FF7F;}"\
            "QTableView:indicator{width:20px;height:20px;}");

    ui->TotalView->setStyleSheet("QTableView{background:transparent;border:1px solid #A7 A7 A7;border-radius:1px;font-size:16px;font-family:'黑体';gridline-color:#A7 A7 A7}"\
            "QTableWidget QTableCornerButton::section{background:transparent;}"\
            "QHeaderView{background:rgb(68,83,101);border-radius:1px;height:35;font-size:16px;font-family:'黑体'}"\
            "QHeaderView::section{background:rgb(68,83,101);padding:5px 10px 5px 10px;border-radius:1px;}"\
            "QTableView:item{background:transparent;color:#00FF7F;}"\
            "QTableView:indicator{width:20px;height:20px;}");

    //设置列字段名
    Detailmodel->setColumnCount(8);//使用默认的1/2/3....

    //设置行字段名
    Detailmodel->setRowCount(4);
    Detailmodel->setHeaderData(0,Qt::Vertical, "ID");
    Detailmodel->setHeaderData(1,Qt::Vertical, "性别");
    Detailmodel->setHeaderData(2,Qt::Vertical, "年龄");
    Detailmodel->setHeaderData(3,Qt::Vertical, "情绪");

    ui->DetailView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->DetailView->setModel(Detailmodel);
    ui->DetailView->setShowGrid(false);

    ui->DetailView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->TotalView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QChart* Sexchart = new QChart();

    Sexseries = new QPieSeries();
    Sexseries->append("男性", 0);
    Sexseries->append("女性", 0);

    Sexseries->setPieSize(1);
    //chart->setTitle("abcd");
    // 将系列添加到图表
    Sexchart->addSeries(Sexseries);
    Sexchart->setBackgroundVisible(false);
    Sexchart->layout()->setContentsMargins(0, 0, 0, 0);//设置外边界全部为0
    Sexchart->setMargins(QMargins(0, 0, 0, 0));//设置内边界全部为0

    Sexchart->setBackgroundRoundness(10);//设置背景区域无圆角->setBackgroundRoundness(0);//设置背景区域无圆角
    // 基于已添加到图表的 series 来创建默认的坐标轴
    Sexchart->createDefaultAxes();

    // 将图表绑定到视图
    ui->SexChart->setRenderHint(QPainter::Antialiasing);//设置抗锯齿，看起来更平滑（不明显）
    ui->SexChart->setChart(Sexchart);
    ui->SexChart->setAutoFillBackground(true);



    QChart* Emchart = new QChart();

    //QPieSeries *series = new QPieSeries();
    Emseries = new QPieSeries();
    Emseries->append("无表情", 0);
    Emseries->append("高兴", 0);
    Emseries->append("伤心", 0);
    Emseries->append("惊讶", 0);
    Emseries->append("生气", 0);

    Emseries->setPieSize(1);
    // 将系列添加到图表
    Emchart->addSeries(Emseries);
    Emchart->setBackgroundVisible(false);
    Emchart->layout()->setContentsMargins(0, 0, 0, 0);//设置外边界全部为0
    Emchart->setMargins(QMargins(0, 0, 0, 0));//设置内边界全部为0
    Emchart->setBackgroundRoundness(0);//设置背景区域无圆角->setBackgroundRoundness(0);//设置背景区域无圆角
    // 基于已添加到图表的 series 来创建默认的坐标轴
    Sexchart->createDefaultAxes();

    // 将图表绑定到视图
    ui->EmChart->setRenderHint(QPainter::Antialiasing);//设置抗锯齿，看起来更平滑（不明显）
    ui->EmChart->setChart(Emchart);
    ui->EmChart->setAutoFillBackground(true);


    qRegisterMetaType<Face::Ptr>("Face::Ptr");

    VisitorScene_thread = new (VisitorSeceneThread);
    VisitorScene_thread->start();

    QObject::connect(VisitorScene_thread, SIGNAL(sendImage(QImage)), this, SLOT(acceptImage(QImage)));//连接信号与槽
    QObject::connect(VisitorScene_thread, SIGNAL(sendFaceInfo(Face::Ptr , int)), this, SLOT(acceptFaceInfo(Face::Ptr , int)));//连接信号与槽
    QObject::connect(VisitorScene_thread, SIGNAL(sendTotalFace(int)), this, SLOT(acceptTotalFace(int)));//连接信号与槽
    QObject::connect(VisitorScene_thread, SIGNAL(updatechart(int , int)), this, SLOT(acceptupdatechart(int , int)));//连接信号与槽
    QObject::connect(VisitorScene_thread, SIGNAL(inittable(int , struct personAttribute *)), this, SLOT(acceptinittable(int , struct personAttribute *)));//连接信号与槽
    QObject::connect(VisitorScene_thread, SIGNAL(updatetable(struct personAttribute *)), this, SLOT(acceptupdatetable(struct personAttribute *)));//连接信号与槽
    QObject::connect(VisitorScene_thread, SIGNAL(updateEmChart(int *)), this, SLOT(acceptupdateEmchart(int *)));//连接信号与槽*/
}

visitorscene::~visitorscene()
{
    delete ui;
}

void visitorscene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::blue,4));//设置画笔形式
    painter.drawLine(20,20,220,220);//画直线
    painter.drawLine(20,220,220,20);
    painter.drawEllipse(20,20,200,200);//画圆
    painter.drawRect(20,20,200,200);//画矩形
}

void visitorscene::closeEvent( QCloseEvent * event )//关闭窗口会先处理该事件函数
{
    std::cout << "end thread" << std::endl;
    VisitorScene_thread->stop();
    std::cout << "closed and clear resources" <<std::endl;
}

/* 接收从VisitorThread线程发送过来的图像数据进行线索 */
void visitorscene::acceptImage(QImage msg)
{
    QPixmap pixmap =QPixmap::fromImage(msg);
    pixmap.scaled(ui->video->size(), Qt::KeepAspectRatio);
    this->ui->video->setPixmap(QPixmap::fromImage(msg));
}

/* 接收从VisitorThread面部信息进行显示 */
void visitorscene::acceptFaceInfo(Face::Ptr face , int ImageFaceId)
{
   // this->model->setItem(0, ImageFaceId, new QStandardItem(QString::number(face->getId() , 10)));
    this->Detailmodel->setItem(0, ImageFaceId, new QStandardItem(QString::fromStdString(face->getfacename())));
    this->Detailmodel->setItem(1, ImageFaceId, new QStandardItem(face->isMale()?"男":"女"));
    this->Detailmodel->setItem(2, ImageFaceId, new QStandardItem(QString::number(face->getAge() , 10)));

    auto emotion = face->getMainEmotion();
    this->Detailmodel->setItem(3, ImageFaceId, new QStandardItem(emotion.first.data()));
}

void visitorscene::acceptTotalFace(int FaceNum)
{
    Detailmodel->setColumnCount(FaceNum);//修改QTableView的为实际数量
}

void visitorscene::acceptupdatechart(int malenum , int femalenum)
{
    QPieSlice *slice_male =Sexseries->slices().at(0);//可通过QPieSlice设置单项数据
    slice_male->setValue(malenum);
    slice_male->setLabel(QString("男(%1)").arg(malenum));

    QPieSlice *slice_female =Sexseries->slices().at(1);//可通过QPieSlice设置单项数据
    slice_female->setValue(femalenum);
    slice_female->setLabel(QString("女(%1)").arg(femalenum));
/*
    int em[10];
    QueryTableAccordingEm(PERSONTABLE , em);

    for(size_t i=0;i<emotionsVec.size();i++)
    {
        QPieSlice *slice_female =Emseries->slices().at(i);//可通过QPieSlice设置单项数据
        slice_female->setValue(em[i]);
        slice_female->setLabel(QString("%1").arg(emotionsVec[i].c_str()));
    }*/
}

void visitorscene::acceptupdateEmchart(int *em)
{
    if(em == NULL)
    {
        std::cout << "input is null ,update em chart failed" << std::endl;
        return;
    }
    for(size_t i=0;i<emotionsVec.size();i++)
    {
        QPieSlice *Emslice =Emseries->slices().at(i);//可通过QPieSlice设置单项数据
        Emslice->setValue(em[i]);
        Emslice->setLabel(QString("%1").arg(emotionsVec[i].c_str()));
        std::cout << emotionsVec[i].c_str() << ":"<< em[i] << std::endl;
    }
}

void visitorscene::acceptinittable(int TotalRecord , struct personAttribute *pAttTotal)
{
    /* 总表显示姓名/年龄/性别/时间 */
    TotalTableModel->setColumnCount(4);//使用默认的1/2/3....

    QStringList labels = QObject::trUtf8("姓名,年龄,性别,时间").simplified().split(",");
    TotalTableModel->setHorizontalHeaderLabels(labels);
    //设置行字段名
    TotalTableModel->setRowCount(TotalRecord);

    //model->clear();
    ui->TotalView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TotalView->setModel(TotalTableModel);
    ui->TotalView->setShowGrid(false);

    for(int i=0;i<TotalRecord;i++)
    {
        this->TotalTableModel->setItem(i , 0 ,new QStandardItem(QString::fromStdString(pAttTotal[i].name)));
        this->TotalTableModel->setItem(i , 1 ,new QStandardItem(QString::number(pAttTotal[i].age  , 10)));
        this->TotalTableModel->setItem(i , 2 ,new QStandardItem(QString::fromStdString(pAttTotal[i].sex)));
        this->TotalTableModel->setItem(i , 3 ,new QStandardItem(QString::fromStdString(pAttTotal[i].time)));
    }
}

void visitorscene::acceptupdatetable(struct personAttribute *pAtt)
{
    int row_count = TotalTableModel->rowCount();
    row_count = row_count+1;

    //std::cout << "update tablle.................." << std::endl;
    std::cout << "table time:" << pAtt->time<<std::endl;
    this->TotalTableModel->setItem(row_count-1 , 0 ,new QStandardItem(QString::fromStdString(pAtt->name)));
    this->TotalTableModel->setItem(row_count-1 , 1 ,new QStandardItem(QString::number(pAtt->age  , 10)));
    this->TotalTableModel->setItem(row_count-1 , 2 ,new QStandardItem(QString::fromStdString(pAtt->sex)));
    this->TotalTableModel->setItem(row_count-1 , 3 ,new QStandardItem(QString::fromStdString(pAtt->time)));
    std::cout << "table time:" << pAtt->time<<std::endl;
}

extern QImage MatToQImage(const cv::Mat& mat);

VisitorSeceneThread::VisitorSeceneThread()
{
    VisitorRunning = true;
}

void VisitorSeceneThread::run()
{
    qDebug()<<"Visitor Thread Running !!";
    VisitorRunning = true;

    Timer timer;

    cap.open(0);
    if(!cap.isOpened())
    {
            printf("open video file failed\n");
            return;
    }
    else
            printf("open video ok\n");

    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);

    cv::Mat frame;
    if (!cap.read(frame)) {
        throw std::logic_error("Failed to get frame from cv::VideoCapture");
    }

    const size_t width  = static_cast<size_t>(frame.cols);
    const size_t height = static_cast<size_t>(frame.rows);

    std::cout << "Get Video Width/Height!!!!!!!!" << std::endl;
    std::cout << width << std::endl;
    std::cout << height << std::endl;

    Core ie;
    std::set<std::string> loadedDevices;
    std::map<std::string, Core> pluginsForDevices;
    std::vector<std::pair<std::string, std::string>> cmdOptions =
    {
        {FLAGS_d, FLAGS_m},
        {FLAGS_d_ag, FLAGS_m_ag},
        {FLAGS_d_hp, FLAGS_m_hp},
        {FLAGS_d_em, FLAGS_m_em},
        {FLAGS_d_lm, FLAGS_m_lm}
    };

    std::cout << FLAGS_m << std::endl;
    std::cout << FLAGS_d << std::endl;
    std::cout << FLAGS_m_ag << std::endl;
    std::cout << FLAGS_m_hp << std::endl;
    std::cout << FLAGS_m_em << std::endl;
    std::cout << FLAGS_m_lm << std::endl;

    FaceDetection faceDetector(FLAGS_m, FLAGS_d, 1, false, FLAGS_async, FLAGS_t, FLAGS_r,
                                       static_cast<float>(FLAGS_bb_enlarge_coef), static_cast<float>(FLAGS_dx_coef), static_cast<float>(FLAGS_dy_coef));
    AgeGenderDetection ageGenderDetector(FLAGS_m_ag, FLAGS_d_ag, FLAGS_n_ag, FLAGS_dyn_ag, FLAGS_async, FLAGS_r);
    HeadPoseDetection headPoseDetector(FLAGS_m_hp, FLAGS_d_hp, FLAGS_n_hp, FLAGS_dyn_hp, FLAGS_async, FLAGS_r);
    EmotionsDetection emotionsDetector(FLAGS_m_em, FLAGS_d_em, FLAGS_n_em, FLAGS_dyn_em, FLAGS_async, FLAGS_r);
    FacialLandmarksDetection facialLandmarksDetector(FLAGS_m_lm, FLAGS_d_lm, FLAGS_n_lm, FLAGS_dyn_lm, FLAGS_async, FLAGS_r);


    auto fr_model_path = FLAGS_m_reid;
    auto fr_weights_path = fileNameNoExt(FLAGS_m_reid) + ".bin";
    auto lm_model_path = FLAGS_m_lm;
    auto lm_weights_path = fileNameNoExt(FLAGS_m_lm) + ".bin";

    int max_batch_size = 1;

    std::string driver_name = "";
    //std::map<std::string, Core> pluginsForDevices;

    CnnConfig reid_config(fr_model_path, fr_weights_path);
    reid_config.max_batch_size = max_batch_size;
    reid_config.enabled =  true;
    reid_config.plugin = pluginsForDevices[FLAGS_d_reid];
    reid_config.deviceName = FLAGS_d_reid;
    VectorCNN face_reid(reid_config);

    // Load landmarks detector
    CnnConfig landmarks_config(lm_model_path, lm_weights_path);
    landmarks_config.max_batch_size = max_batch_size;
    landmarks_config.enabled =  true;
    landmarks_config.plugin = pluginsForDevices[FLAGS_d_lm];
    landmarks_config.deviceName = FLAGS_d_lm;
    VectorCNN landmarks_detector(landmarks_config);

    double t_reid = 0.4; // Cosine distance threshold between two vectors for face reidentification.
    EmbeddingsGallery face_gallery(FLAGS_fg, t_reid, landmarks_detector, face_reid);

    for (auto && option : cmdOptions)
    {
        auto deviceName = option.first;
        auto networkName = option.second;

        if (deviceName.empty() || networkName.empty()) {
            continue;
        }

        if (loadedDevices.find(deviceName) != loadedDevices.end()) {
            continue;
        }
        slog::info << "Loading device " << deviceName << slog::endl;
        std::cout << ie.GetVersions(deviceName) << std::endl;

        // Loading extensions for the CPU device
        if ((deviceName.find("CPU") != std::string::npos)) {
            ie.AddExtension(std::make_shared<Extensions::Cpu::CpuExtensions>(), "CPU");
        }

        loadedDevices.insert(deviceName);
    }

    // --------------------------- 2. Reading IR models and loading them to plugins ----------------------
    //Disable dynamic batching for face detector as it processes one image at a time
    Load(faceDetector).into(ie, FLAGS_d, false);
    Load(ageGenderDetector).into(ie, FLAGS_d_ag, FLAGS_dyn_ag);
    Load(headPoseDetector).into(ie, FLAGS_d_hp, FLAGS_dyn_hp);
    Load(emotionsDetector).into(ie, FLAGS_d_em, FLAGS_dyn_em);
    //Load(facialLandmarksDetector).into(ie, FLAGS_d_lm, FLAGS_dyn_lm);
    // ----------------------------------------------------------------------------------------------------

    // --------------------------- 3. Doing inference -----------------------------------------------------
    // Starting inference & calculating performance
    std::cout << "Start inference " << std::endl;

    bool isFaceAnalyticsEnabled = ageGenderDetector.enabled() || headPoseDetector.enabled() ||
                                  emotionsDetector.enabled() || facialLandmarksDetector.enabled();

    std::ostringstream out;
    size_t framesCounter = 0;
    bool frameReadStatus;
    bool isLastFrame;
    cv::Mat prev_frame, next_frame;
    std::list<Face::Ptr> faces;


    size_t id = 0;

    Visualizer::Ptr visualizer;
    visualizer = std::make_shared<Visualizer>(cv::Size(width, height));
    if (!FLAGS_no_show_emotion_bar && emotionsDetector.enabled()) {
        visualizer->enableEmotionBar(emotionsDetector.emotionsVec);
    }

    // Detecting all faces on the first frame and reading the next one
    faceDetector.enqueue(frame);
    faceDetector.submitRequest();

    prev_frame = frame.clone();

    // Reading the next frame
    frameReadStatus = cap.read(frame);

    //创建数据库
   if (FAIL == CreateDatabaseConnectionSwitch(db , ON))
    {
           slog::info<<("链接数据库失败")<<slog::endl;
    }

   if (FAIL == CreatePersonAttributeTab(PERSONTABLE))
    {
        slog::info<<("数据库表创建失败")<<slog::endl;
    }

    //QueryVistorsNum(PERSONTABLE)

    slog::info << "-----------------------------------------"<<slog::endl;
    QueryWholeTable(PERSONTABLE);
    slog::info << "-----------------------------------------"<<slog::endl;

    // 根据数据库中的男女比例，更新图表
    int malenum = QueryTotalPerson(PERSONTABLE , "male");
    int femalenum = QueryTotalPerson(PERSONTABLE , "female");

    updatechart(malenum , femalenum);

    slog::info<<"男："<<malenum<<slog::endl;
    slog::info<<"女："<<femalenum<<slog::endl;

    int TotalRecord = malenum + femalenum;
    struct personAttribute *pAttTotal = new struct personAttribute[TotalRecord+1];
    qryCondstr qryCond;
    qryCond.startAddr = 0;
    qryCond.dataLenth =TotalRecord;
    QueryVistorsNum(pAttTotal ,PERSONTABLE , &qryCond);
    inittable(TotalRecord , pAttTotal);

    int Em[10];
    QueryTableAccordingEm(PERSONTABLE , Em);
    updateEmChart(Em);
    msleep(5);

    //Timer timer;
    timer.start("face_identified");

    while (VisitorRunning) {
        timer.start("total");
        framesCounter++;
        isLastFrame = !frameReadStatus;

        // Retrieving face detection results for the previous frame
        faceDetector.wait();
        faceDetector.fetchResults();
        //std::cout << height << std::endl;

        auto prev_detection_results = faceDetector.results;

        // No valid frame to infer if previous frame is the last
        if (!isLastFrame) {
            faceDetector.enqueue(frame);
            faceDetector.submitRequest();
        }

        // Filling inputs of face analytics networks
        for (auto &&face : prev_detection_results) {
            if (isFaceAnalyticsEnabled) {
                auto clippedRect = face.location & cv::Rect(0, 0, width, height);
                cv::Mat face = prev_frame(clippedRect);
                ageGenderDetector.enqueue(face);
                headPoseDetector.enqueue(face);
                emotionsDetector.enqueue(face);
                //facialLandmarksDetector.enqueue(face);
            }
        }

        // Running Age/Gender Recognition, Head Pose Estimation, Emotions Recognition, and Facial Landmarks Estimation networks simultaneously
        if (isFaceAnalyticsEnabled) {
            ageGenderDetector.submitRequest();
            headPoseDetector.submitRequest();
            emotionsDetector.submitRequest();
            //facialLandmarksDetector.submitRequest();
        }

        // Reading the next frame if the current one is not the last
        if (!isLastFrame) {
            frameReadStatus = cap.read(next_frame);
            if (FLAGS_loop_video && !frameReadStatus) {
                if (!(FLAGS_i == "cam" ? cap.open(0) : cap.open(FLAGS_i))) {
                    throw std::logic_error("Cannot open input file or camera: ");
                }
                frameReadStatus = cap.read(next_frame);
            }
        }

        if (isFaceAnalyticsEnabled) {
                        ageGenderDetector.wait();
                        headPoseDetector.wait();
                        emotionsDetector.wait();
                        //facialLandmarksDetector.wait();
        }

        //  Postprocessing
        std::list<Face::Ptr> prev_faces;

        if (!FLAGS_no_smooth) {
            prev_faces.insert(prev_faces.begin(), faces.begin(), faces.end());
        }

        faces.clear();

        // For every detected face
        //model->setColumnCount(prev_detection_results.size());//修改QTableView的为实际数量
        sendTotalFace(prev_detection_results.size());

        std::vector<cv::Mat> face_rois, landmarks, embeddings;

        Mat Max_Pic;

        for (size_t i = 0; i < prev_detection_results.size(); i++) {
            auto& result = prev_detection_results[i];
            cv::Rect rect = result.location & cv::Rect(0, 0, width, height);

            Face::Ptr face;
            if (!FLAGS_no_smooth) {
                face = matchFace(rect, prev_faces);
                float intensity_mean = calcMean(prev_frame(rect));

                if ((face == nullptr) ||
                    ((face != nullptr) && ((std::abs(intensity_mean - face->_intensity_mean) / face->_intensity_mean) > 0.07f))) {
                    face = std::make_shared<Face>(id++, rect);
                } else {
                    prev_faces.remove(face);
                }

                face->_intensity_mean = intensity_mean;
                face->_location = rect;
            } else {
                face = std::make_shared<Face>(id++, rect);
            }

            face->ageGenderEnable((ageGenderDetector.enabled() &&
                                   i < ageGenderDetector.maxBatch));
            if (face->isAgeGenderEnabled()) {
                AgeGenderDetection::Result ageGenderResult = ageGenderDetector[i];
                face->updateGender(ageGenderResult.maleProb);
                face->updateAge(ageGenderResult.age);
            }


            face->emotionsEnable((emotionsDetector.enabled() &&
                                  i < emotionsDetector.maxBatch));
            if (face->isEmotionsEnabled()) {
                face->updateEmotions(emotionsDetector[i]);
            }
            face->headPoseEnable((headPoseDetector.enabled() &&
                                  i < headPoseDetector.maxBatch));
            if (face->isHeadPoseEnabled()) {
                face->updateHeadPose(headPoseDetector[i]);
            }

            sendFaceInfo(face , i);

            // 进行人脸再识别，分辨出人物名字，将人物名字和其他信息一起写入链表
            cv::Mat aux_prev_frame = prev_frame.clone();
            face_rois.push_back(aux_prev_frame(face->_location));

            landmarks_detector.Compute(face_rois, &landmarks, cv::Size(2, 5));
            AlignFaces(&face_rois, &landmarks);
            face_reid.Compute(face_rois, &embeddings);
            auto ids = face_gallery.GetIDsByEmbeddings(embeddings);
            if (!ids.empty() && ids[i] != EmbeddingsGallery::unknown_id)
            {
                face->updatefacename(face_gallery.GetLabelByID(ids[i]));
            }
            else {
                face->updatefacename("Unknown");
            }

            Mat pic =prev_frame(face->_location);
            if((pic.rows > Max_Pic.rows) && (pic.cols > Max_Pic.cols))
            {
                Max_Pic = pic;
            }

            struct personAttribute *QtPerAttrTable = new struct personAttribute[1];

            //将识别到的信息添加到数据库中

            //memset(&QtPerAttrTable , 0 , sizeof(QtPerAttrTable));

            QtPerAttrTable->id = face->getId();
            QtPerAttrTable->sex = face->isMale()?"男":"女";
            QtPerAttrTable->age = face->getAge();
            QtPerAttrTable->expression =face->getMainEmotion().first.data();
            QtPerAttrTable->name =face->getfacename();

            QDateTime current_time = QDateTime::currentDateTime();
            QString StrCurrentTime = current_time.toString("yyyy.MM.dd hh:mm:ss");
            QtPerAttrTable->time = StrCurrentTime.toStdString();

            if(SUCC == AddOnePersonAttribute(QtPerAttrTable , PERSONTABLE))
            {
                // 插入到数据库成功，更新图表
                int malenum = QueryTotalPerson(PERSONTABLE , "male");
                int femalenum = QueryTotalPerson(PERSONTABLE , "female");

                updatechart(malenum , femalenum);

                std::cout << "add time :" <<QtPerAttrTable->time << std::endl;
                updatetable(QtPerAttrTable);

                int *Em = new int[emotionsVec.size()+1];


                QueryTableAccordingEm(PERSONTABLE , Em);

                updateEmChart(Em);

                msleep(5);

                delete[]Em;
            }

            delete[] QtPerAttrTable;

            faces.push_back(face);
        }

        sendpic(MatToQImage(Max_Pic));
        //  Visualizing results
        visualizer->draw(prev_frame, faces);

        QImage image = QImage(prev_frame.data,prev_frame.cols,prev_frame.rows,QImage::Format_RGB888).rgbSwapped();
        //ui->label_video->setPixmap(QPixmap::fromImage(image));
        sendImage(image);

        prev_frame = frame;
        frame = next_frame;
        next_frame = cv::Mat();

        timer.finish("total");
    }
    // Showing performance results
    if (FLAGS_pc) {
        faceDetector.printPerformanceCounts(getFullDeviceName(pluginsForDevices[FLAGS_d], FLAGS_d));
        ageGenderDetector.printPerformanceCounts(getFullDeviceName(pluginsForDevices[FLAGS_d], FLAGS_d_ag));
        headPoseDetector.printPerformanceCounts(getFullDeviceName(pluginsForDevices[FLAGS_d], FLAGS_d_hp));
        emotionsDetector.printPerformanceCounts(getFullDeviceName(pluginsForDevices[FLAGS_d], FLAGS_d_em));
        facialLandmarksDetector.printPerformanceCounts(getFullDeviceName(pluginsForDevices[FLAGS_d], FLAGS_d_lm));
    }

    // release input video stream
    cap.release();

    //关闭数据库
    CreateDatabaseConnectionSwitch(db , OFF);

    qDebug()<<"Pthread exit!!";
}

void VisitorSeceneThread::stop()
{
    VisitorRunning = false;   //线程退出标志
    qDebug() << "Visitor Thread end";
}
