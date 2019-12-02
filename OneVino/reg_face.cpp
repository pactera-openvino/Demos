#include "reg_face.h"
#include "ui_reg_face.h"


#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

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
#include <QThread>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
#include <QFlags>

#include <inference_engine.hpp>

#include "interactive_face_detection.hpp"

using namespace std;
using namespace cv;
using namespace InferenceEngine;

QImage MatToQImage(const cv::Mat& mat)
{
    switch (mat.type())
    {
        // 8-bit, 4 channel
        case CV_8UC4:
            {
                QImage image(mat.data,
                             mat.cols, mat.rows,
                             static_cast<int>(mat.step),
                             QImage::Format_ARGB32);

                return image;
            }

         // 8-bit, 3 channel
         case CV_8UC3:
            {
                QImage image(mat.data,
                             mat.cols, mat.rows,
                             static_cast<int>(mat.step),
                             QImage::Format_RGB888);

                return image.rgbSwapped();
            }

         // 8-bit, 1 channel
        case CV_8UC1:
            {
                #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
                QImage image(mat.data,
                             mat.cols, mat.rows,
                             static_cast<int>(mat.step),
                             QImage::Format_Grayscale8);
                #else
                static QVector<QRgb>  sColorTable;

                // only create our color table the first time
                if (sColorTable.isEmpty())
                {
                    sColorTable.resize( 256 );

                    for ( int i = 0; i < 256; ++i )
                    {
                        sColorTable[i] = qRgb( i, i, i );
                    }
                }

                QImage image(mat.data,
                             mat.cols, mat.rows,
                             static_cast<int>(mat.step),
                             QImage::Format_Indexed8 );

                image.setColorTable(sColorTable);
                #endif

                return image;
            }

        // wrong
        default:
            qDebug() << "ERROR: Mat could not be converted to QImage.";
            break;
    }
    return QImage();
}

FaceDetectThread::FaceDetectThread()
{
    FaceDetectRunning = true;
}

void FaceDetectThread::run()
{
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

    Core ie;
    std::set<std::string> loadedDevices;
    std::map<std::string, Core> pluginsForDevices;
    std::vector<std::pair<std::string, std::string>> cmdOptions =
    {
        {FLAGS_d, FLAGS_m},
    };

    std::cout << FLAGS_m << std::endl;
    std::cout << FLAGS_d << std::endl;

    FaceDetection faceDetector(FLAGS_m, FLAGS_d, 1, false, FLAGS_async, FLAGS_t, FLAGS_r,
                                       static_cast<float>(FLAGS_bb_enlarge_coef), static_cast<float>(FLAGS_dx_coef), static_cast<float>(FLAGS_dy_coef));

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

        if ((deviceName.find("CPU") != std::string::npos)) {
            ie.AddExtension(std::make_shared<Extensions::Cpu::CpuExtensions>(), "CPU");
/*
            if (!FLAGS_l.empty()) {
                auto extension_ptr = make_so_pointer<IExtension>(FLAGS_l);
                ie.AddExtension(extension_ptr, "CPU");
                slog::info << "CPU Extension loaded: " << FLAGS_l << slog::endl;
            }*/
        }/* else if (!FLAGS_c.empty()) {
            ie.SetConfig({{PluginConfigParams::KEY_CONFIG_FILE, FLAGS_c}}, "GPU");
        }*/

        loadedDevices.insert(deviceName);
    }

    Load(faceDetector).into(ie, FLAGS_d, false);

    std::ostringstream out;
    size_t framesCounter = 0;
    bool frameReadStatus;
    bool isLastFrame;
    cv::Mat prev_frame, next_frame;
    std::list<Face::Ptr> faces;
    size_t id = 0;

    Visualizer::Ptr visualizer;
    visualizer = std::make_shared<Visualizer>(cv::Size(width, height));

    // Detecting all faces on the first frame and reading the next one
    faceDetector.enqueue(frame);
    faceDetector.submitRequest();

    prev_frame = frame.clone();

    // Reading the next frame
    frameReadStatus = cap.read(frame);

    while( true == FaceDetectRunning )
    {
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

        //  Postprocessing
        std::list<Face::Ptr> prev_faces;

        if (!FLAGS_no_smooth) {
            prev_faces.insert(prev_faces.begin(), faces.begin(), faces.end());
        }

        faces.clear();

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

            Mat pic =prev_frame(face->_location);
            if((pic.rows > Max_Pic.rows) && (pic.cols > Max_Pic.cols))
            {
                Max_Pic = pic;
            }

            faces.push_back(face);
        }
        sendimage(MatToQImage(Max_Pic));

        //Visualizing results
        visualizer->draw(prev_frame, faces);

        //QImage image = QImage(prev_frame.data,prev_frame.cols,prev_frame.rows,QImage::Format_RGB888).rgbSwapped();
       //this->ui->frame_label->setPixmap(QPixmap::fromImage(image));
       // cv::imshow("Detection results", Max_Pic);
       // sendImage(image);

        prev_frame = frame;
        frame = next_frame;
        next_frame = cv::Mat();

        timer.finish("total");
    }
    // Showing performance results
    if (FLAGS_pc) {
        faceDetector.printPerformanceCounts(getFullDeviceName(pluginsForDevices[FLAGS_d], FLAGS_d));
    }

    // release input video stream
    cap.release();

    std::cout<<"Pthread exit!!"<<std::endl;
}

void FaceDetectThread::stop()
{
    FaceDetectRunning = false;
}

/* 判断文件是否存在，需要完整路径 */
bool isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}

void Dialog::acceptimage(QImage msg)
{
    if(true == PreviewFlag)
    {
        if(msg.isNull())
            return;
        SavePixmap =QPixmap::fromImage(msg);

        int with = ui->frame_label->width();
        int height = ui->frame_label->height();
        QPixmap fitpixmap = SavePixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
        ui->frame_label->setPixmap(fitpixmap);

        ui->frame_label->setAlignment(Qt::AlignCenter);
    }
    else
    {
        if(FirstRun == false)
        {
            FirstRun = true;
            std::cout << "First run!!" << std::endl;

            QString FilePwd = SAVEFILEPATH;
            QString InFileName = ui->FileName->text();
            if(!InFileName.isEmpty())
            {
                InFileName.append(".png");

                FilePwd.append(InFileName);

                if(true == isFileExist(FilePwd))
                {
                    QMessageBox::information(this, "Title", "文件已存在",QMessageBox::Ok, QMessageBox::Yes);
                    FirstRun = false;
                }
                else
                {
                    QMessageBox::information(this, "Title", FilePwd,QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                    SavePixmap.save(FilePwd , "PNG" , -1);
                    FirstRun = false;
                    PreviewFlag = true;
                    system("python3 /root/OneVino/Identify/create_list.py /root/OneVino/Identify/image/");
                    system("mv faces_gallery.json /root/OneVino/Identify/");

                    std::cout << "save face ok , continue preview!!!!" << std::endl;
                }
            }
            else {
                QMessageBox::information(this, "错误", "保存失败，未输入姓名",QMessageBox::Yes, QMessageBox::Yes);
                FirstRun = false;
            }
            /*bool ok;
            QString FilePwd = SAVEFILEPATH;
            QString InFileName = QInputDialog::getText(this, tr("添加人物"),tr("请输入人物名字(英文)"), QLineEdit::Normal,0, &ok);
            if (ok && !InFileName.isEmpty())
            {
                InFileName.append(".png");

                FilePwd.append(InFileName);

                if(true == isFileExist(FilePwd))
                {
                    QMessageBox::information(this, "Title", "文件已存在",QMessageBox::Ok, QMessageBox::Yes);
                    FirstRun = false;
                }
                else
                {
                    QMessageBox::information(this, "Title", FilePwd,QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                    SavePixmap.save(FilePwd , "PNG" , -1);
                    FirstRun = false;
                    PreviewFlag = true;
                    system("python3 /root/OpenVino-Dev/Identify/create_list.py /root/OpenVino-Dev/Identify/image/");
                    std::cout << "save face ok , continue preview!!!!" << std::endl;
                }
            }
            else if(ok){
                QMessageBox::information(this, "错误", "保存失败，未输入姓名",QMessageBox::Yes, QMessageBox::Yes);
                FirstRun = false;
            }
            else {
                // 点击取消
                FirstRun = false;
                PreviewFlag = true;
            }*/
        }
    }
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint , true);


    ui->Cancel->setStyleSheet("QPushButton{background-image: url(:/bt/image/cancel.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/cancel_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/cancel_hover.png);border:none;}");

    ui->takePic->setStyleSheet("QPushButton{background-image: url(:/bt/image/save.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/save_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/save_hover.png);border:none;}");


    PreviewFlag = true;

    FaceDetect_thread = new (FaceDetectThread);
    FaceDetect_thread->start();

    QObject::connect(FaceDetect_thread, SIGNAL(sendimage(QImage)), this, SLOT(acceptimage(QImage)));//连接信号与槽
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_takePic_clicked()
{
    /* 关闭脸部预览，进行保存图像 */
    PreviewFlag = false;
}

void Dialog::closeEvent(QCloseEvent *event)
{
    FaceDetect_thread->stop();

    std::cout << "face detect thread stop!!" << std::endl;
}

void Dialog::on_Cancel_clicked()
{
    close();
}
