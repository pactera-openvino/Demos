#include "playback.h"
#include "ui_playback.h"

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
#include <QVBoxLayout>
#include <QSize>
#include <QThread>
#include <QObject>

#include "meetingscene.h"

using namespace std;
using namespace cv;

QString PlayFileName;
extern QImage MatToQImage(const cv::Mat& mat);

PlayBack::PlayBack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayBack)
{
    ui->setupUi(this);

    cout << "play:" << PlayFileName.toStdString() << endl;

    if(PlayBack_thread->PlayBackRunning == true)
    {
        //正在播放视频，先停止，然后载运行
        std::cout << "stop play file!!" <<std::endl;
        PlayBack_thread->stop();
        QThread::msleep(300);
    }

    PlayBack_thread->PlayBackRunning = true;
    PlayBack_thread->start();
    QObject::connect(PlayBack_thread, SIGNAL(sendplayImage(QImage)), this, SLOT(acceptplayImage(QImage)));
}

PlayBack::~PlayBack()
{
    delete ui;
}

void PlayBack::acceptplayImage(QImage msg)
{
    QPixmap pixmap = QPixmap::fromImage(msg);
    int width = ui->PlayBackLable->width();
    int height = ui->PlayBackLable->height();
    QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->ui->PlayBackLable->setPixmap(fitpixmap);
    ui->PlayBackLable->setAlignment(Qt::AlignCenter);
}

PlayBackThread::PlayBackThread()
{
    PlayBackRunning = true;
}

extern QImage MatToQImage(const cv::Mat& mat);
void PlayBackThread::run()
{
    cv::VideoCapture video;
    video.open(PlayFileName.toStdString());
    if(!video.isOpened())
    {
        std::cout<<"Failed to open video"<<std::endl;
    }
    std::cout << "open file " << PlayFileName.toStdString() << " success!" << std::endl;
    double totalFrame=video.get(CAP_PROP_FRAME_COUNT);  //获取总帧数
    double frameRate=video.get(CAP_PROP_FPS);   //获取帧率
    double pauseTime=1000000/frameRate; // 由帧率计算两幅图像间隔时间

    std::cout << "total frame:" << totalFrame << "  frame rate:" << frameRate << "  pause time:" << pauseTime<<"us" << std::endl;

    Mat image;
    double currentFrame = 0;

    if(PlayBackRunning == true)
        std::cout << "normal play " << std::endl;
    while(PlayBackRunning)
    {
        video.read(image);
        resize(image,image,Size(500,400));
        sendplayImage(MatToQImage(image));
        usleep(pauseTime);
        currentFrame++;
        if(currentFrame>totalFrame)  //播放完成退出
        {
            break;
        }
    }
    PlayBackRunning = false;
    std::cout << "file play over" << std::endl;
}

void PlayBackThread::stop()
{
    PlayBackRunning = false;
}
