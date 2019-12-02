#include "meetingscene.h"
#include "ui_meetingscene.h"

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

#include <inference_engine.hpp>

#include <samples/ocv_common.hpp>
#include <samples/slog.hpp>

#include "face_reid.hpp"
#include "database.hpp"

#include "interactive_face_detection.hpp"

#include "addmeeting.h"
#include "database.hpp"
#include "playback.h"

using namespace std;
using namespace cv;
using namespace InferenceEngine;

MeetingScene::MeetingScene(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeetingScene)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    showMaximized(); //默认最大化

    ui->MeetingTotal->setStyleSheet("QTableView{background:transparent;border:1px solid #A7 A7 A7;border-radius:1px;font-size:11px;font-family:'黑体';gridline-color:#A7 A7 A7}"\
            "QTableView QTableCornerButton::section{color: white;background-color: transparent;border-radius:0px;}" \
            "QHeaderView{background:transparent;border-radius:1px;height:35;font-size:16px;font-family:'黑体'}"\
            "QHeaderView::section{background:transparent;padding:5px 10px 5px 10px;border-radius:1px;}"\
            "QHeaderView::section{color:#FFFFFF;padding:5px 10px 5px 10px;border-radius:1px;}"\
            "QTableView:item{background:transparent;color:#FFFFFF;}");

    ui->PlayList->setStyleSheet("QListView{background:transparent;border:1px solid #A7 A7 A7;border-radius:1px;font-size:16px;font-family:'黑体';gridline-color:#A7 A7 A7}"\
            "QListView QTableCornerButton::section{color: white;background-color: transparent;border-radius:0px;}" \
            "QListView:item{background:transparent;color:#FFFFFF;}");

    ui->AbsentView->setStyleSheet("QTableView{background:transparent;border:1px solid #A7 A7 A7;border-radius:1px;font-size:16px;font-family:'黑体';gridline-color:#A7 A7 A7}"\
            "QTableView QTableCornerButton::section{color: white;background-color: transparent;border-radius:0px;}" \
            "QHeaderView{background:transparent;border-radius:1px;height:35;font-size:16px;font-family:'黑体'}"\
            "QHeaderView::section{background:transparent;padding:5px 10px 5px 10px;border-radius:1px;}"\
            "QHeaderView::section{color:#FFFFFF;padding:5px 10px 5px 10px;border-radius:1px;}"\
            "QTableView:item{background:transparent;color:#FFFFFF;}");

    ui->LateView->setStyleSheet("QTableView{background:transparent;border:1px solid #A7 A7 A7;border-radius:1px;font-size:16px;font-family:'黑体';gridline-color:#A7 A7 A7}"\
            "QTableView QTableCornerButton::section{color: white;background-color: transparent;border-radius:0px;}" \
            "QHeaderView{background:transparent;border-radius:1px;height:35;font-size:16px;font-family:'黑体'}"\
            "QHeaderView::section{background:transparent;padding:5px 10px 5px 10px;border-radius:1px;}"\
            "QHeaderView::section{color:#FFFFFF;padding:5px 10px 5px 10px;border-radius:1px;}"\
            "QTableView:item{background:transparent;color:#FFFFFF;}");

    ui->PlayBackLable->setScaledContents(true);

    Meetingmodel->setColumnCount(6);//使用默认的1/2/3....
    /*Meetingmodel->setHorizontalHeaderItem(0 , new QStandardItem(QObject::tr("头像")));
    Meetingmodel->setHorizontalHeaderItem(1 , new QStandardItem(QObject::tr("姓名")));
    Meetingmodel->setHorizontalHeaderItem(2 , new QStandardItem(QObject::tr("开始时间")));
    Meetingmodel->setHorizontalHeaderItem(3 , new QStandardItem(QObject::tr("结束时间")));
    Meetingmodel->setHorizontalHeaderItem(4 , new QStandardItem(QObject::tr("地点")));
    Meetingmodel->setHorizontalHeaderItem(5 , new QStandardItem(QObject::tr("状态")));*/
    //设置行字段名
    Meetingmodel->setRowCount(4);

    ui->MeetingTotal->horizontalHeader()->setVisible(false);
    ui->MeetingTotal->verticalHeader()->setVisible(false);
    ui->MeetingTotal->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->MeetingTotal->setModel(Meetingmodel);
    ui->MeetingTotal->setShowGrid(false);

    ui->MeetingTotal->setIconSize(QSize(50,50));
    ui->MeetingTotal->setSelectionBehavior(QAbstractItemView::SelectRows);


    ui->LateView->horizontalHeader()->setVisible(false);
    ui->LateView->verticalHeader()->setVisible(false);
    ui->LateView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->LateView->setModel(Latemodel);
    ui->LateView->setShowGrid(false);
    ui->LateView->setIconSize(QSize(50,50));
    ui->LateView->setSelectionBehavior(QAbstractItemView::SelectRows);


    ui->AbsentView->horizontalHeader()->setVisible(false);
    ui->AbsentView->verticalHeader()->setVisible(false);
    ui->AbsentView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->AbsentView->setModel(Absentmodel);
    ui->AbsentView->setShowGrid(false);
    ui->AbsentView->setIconSize(QSize(50,50));
    ui->AbsentView->setSelectionBehavior(QAbstractItemView::SelectRows);



    qRegisterMetaType<Face::Ptr>("Face::Ptr");

    Meeting_thread = new (MeetingThread);
    Meeting_thread->start();

    QObject::connect(Meeting_thread, SIGNAL(sendImage(QImage)), this, SLOT(acceptImage(QImage)));
    QObject::connect(Meeting_thread, SIGNAL(sendfacename(QString)), this, SLOT(acceptfacename(QString)));


    //设置定时器是否为单次触发。默认为 false 多次触发
    m_timer->setSingleShot(false);
    //启动或重启定时器, 并设置定时器时间：毫秒
    m_timer->start(1000);
    //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));

    //设置定时器是否为单次触发。默认为 false 多次触发
    updatetimer->setSingleShot(false);
    //启动或重启定时器, 并设置定时器时间：毫秒
    updatetimer->start(1000);
    //定时器触发信号槽
    connect(updatetimer, SIGNAL(timeout()), this, SLOT(UpdateTableTimeOut()));


    //初始化视频栏
    QString VideoFolderPath = VIDEOPATH;
    QDir Videodir(VideoFolderPath);
    if (!Videodir.exists()) {
        return;
    }
    Videodir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList filters;
    filters << "*.avi";
    Videodir.setNameFilters(filters);

    QStringList allVideoNameList = Videodir.entryList();
    QFileInfoList fileInfoList = Videodir.entryInfoList();

    std::cout << "file num:" << allVideoNameList.count() << std::endl;
    if (allVideoNameList.count() > 0)
    {
        for (int i = 0; i<allVideoNameList.count(); ++i)
        {
            std::cout << "filename:" << fileInfoList.at(i).baseName().toStdString() << std::endl;
            QStandardItem *item = new QStandardItem(fileInfoList.at(i).baseName());
            VideoListModel->appendRow(item);
        }
        ui->PlayList->setModel(VideoListModel);
        ui->PlayList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }





    if (FAIL == CreateDatabaseConnectionSwitch(db , ON))
    {
        QMessageBox::information(this, "错误", "打开数据库失败",QMessageBox::Yes, QMessageBox::Yes);
    }

    if (FAIL == CreateMeetingTab(MEETINGTABLE))
    {
        QMessageBox::information(this, "错误", "打开数据库表失败",QMessageBox::Yes, QMessageBox::Yes);
    }

    std::list <MeetingListStr> lists;

    PrintGroupByMeetingTable(lists , MEETINGTABLE);
    if(lists.empty())
    {
        CreateDatabaseConnectionSwitch(db , OFF);
        return;
    }
    list<MeetingListStr>::iterator list;
    ui->MeetingList->clear();
    for ( list = lists.begin(); list != lists.end(); ++list)
    {
        std::ostringstream tim;
        tim<<list->start_time<<"-"<<list->end_time;
        ui->MeetingList->addItem(QString::fromStdString(tim.str()));
    }
    //std::list <MeetingInfoStr> infolists;
    PeopleMeetinglists.clear();
    GetMeetingListByTime(PeopleMeetinglists , lists.front().start_time , lists.front().end_time , MEETINGTABLE);
    std::list<MeetingInfoStr>::iterator m;

    if(PeopleMeetinglists.empty())
    {
        CreateDatabaseConnectionSwitch(db , OFF);
        return;
    }

    std::cout << "list size :" << PeopleMeetinglists.size() << std::endl;
    QString imageFolderPath = IMAGEPATH;
    int count = 0;
    Meetingmodel->clear();
    Meetingmodel->setRowCount(PeopleMeetinglists.size());
    for ( m = PeopleMeetinglists.begin(); m != PeopleMeetinglists.end(); ++m)
    {
        QString filepath = imageFolderPath + QString::fromStdString(m->name).append(".png");

        QIcon icon;
        icon.addFile(filepath);
        ui->MeetingTotal->setRowHeight(count, 50);
        Meetingmodel->setItem(count , 0 , new QStandardItem(icon , 0));
        Meetingmodel->setItem(count , 1, new QStandardItem(QString::fromStdString(m->name)));
        Meetingmodel->setItem(count , 2, new QStandardItem(QString::fromStdString(m->start_time)));
        Meetingmodel->setItem(count , 3, new QStandardItem(QString::fromStdString(m->end_time)));
        Meetingmodel->setItem(count , 4, new QStandardItem(QString::fromStdString(m->place)));
        Meetingmodel->setItem(count , 5, new QStandardItem(QString::fromStdString("")));

        count++;
    }
    if (FAIL == CreateDatabaseConnectionSwitch(db , OFF))
    {
        QMessageBox::information(this, "错误", "关闭数据库失败",QMessageBox::Yes, QMessageBox::Yes);
    }

    /* 设置宽度 */
    ui->MeetingTotal->setColumnWidth(0 , 50);
    ui->MeetingTotal->setColumnWidth(1 , 100);
    ui->MeetingTotal->setColumnWidth(2 , 150);
    ui->MeetingTotal->setColumnWidth(3 , 150);
    ui->MeetingTotal->setColumnWidth(4 , 100);
    ui->MeetingTotal->setColumnWidth(5 , 50);
}

MeetingScene::~MeetingScene()
{
    delete ui;
}

void MeetingScene::on_AddMeeting_bt_clicked()
{
    AddMeeting dialog;
    dialog.setWindowModality(Qt::ApplicationModal);
    if(dialog.exec() != AddMeeting::Accepted)//如果是取消就返回
    {
        std::cout << "exit add meeting" << std::endl;

        if (FAIL == CreateDatabaseConnectionSwitch(db , ON))
        {
            QMessageBox::information(this, "错误", "打开数据库失败",QMessageBox::Yes, QMessageBox::Yes);
        }

        if (FAIL == CreateMeetingTab(MEETINGTABLE))
        {
            QMessageBox::information(this, "错误", "打开数据库表失败",QMessageBox::Yes, QMessageBox::Yes);
        }

        std::list <MeetingListStr> lists;

        PrintGroupByMeetingTable(lists , MEETINGTABLE);
        list<MeetingListStr>::iterator list;
        ui->MeetingList->clear();
        for ( list = lists.begin(); list != lists.end(); ++list)
        {
            std::ostringstream tim;
            tim<<list->start_time<<"-"<<list->end_time;
            ui->MeetingList->addItem(QString::fromStdString(tim.str()));
        }
        //std::list <MeetingInfoStr> infolists;
        PeopleMeetinglists.clear();
        GetMeetingListByTime(PeopleMeetinglists , lists.front().start_time , lists.front().end_time , MEETINGTABLE);
        std::list<MeetingInfoStr>::iterator m;

        if(PeopleMeetinglists.empty())
            return;

        std::cout << "list size :" << PeopleMeetinglists.size() << std::endl;
        QString imageFolderPath = "/root/OneVino/Identify/image/";
        int count = 0;
        Meetingmodel->clear();
        Meetingmodel->setRowCount(PeopleMeetinglists.size());
        for ( m = PeopleMeetinglists.begin(); m != PeopleMeetinglists.end(); ++m)
        {
            QString filepath = imageFolderPath + QString::fromStdString(m->name).append(".png");

            QIcon icon;
            icon.addFile(filepath);
            ui->MeetingTotal->setRowHeight(count, 50);
            Meetingmodel->setItem(count , 0 , new QStandardItem(icon , 0));
            Meetingmodel->setItem(count , 1, new QStandardItem(QString::fromStdString(m->name)));
            Meetingmodel->setItem(count , 2, new QStandardItem(QString::fromStdString(m->start_time)));
            Meetingmodel->setItem(count , 3, new QStandardItem(QString::fromStdString(m->end_time)));
            Meetingmodel->setItem(count , 4, new QStandardItem(QString::fromStdString(m->place)));
            /*ui->MeetingTotal->setColumnWidth(0 , 50);
            ui->MeetingTotal->setColumnWidth(1 , 100);
            ui->MeetingTotal->setColumnWidth(2 , 200);
            ui->MeetingTotal->setColumnWidth(3 , 200);
            ui->MeetingTotal->setColumnWidth(4 , 135);*/
            count++;
        }
        if (FAIL == CreateDatabaseConnectionSwitch(db , OFF))
        {
            QMessageBox::information(this, "错误", "关闭数据库失败",QMessageBox::Yes, QMessageBox::Yes);
        }

        return;
    }
}

void MeetingScene::on_MeetingList_activated(const QString &arg1)
{
    if (FAIL == CreateDatabaseConnectionSwitch(db , ON))
    {
        QMessageBox::information(this, "错误", "打开数据库失败",QMessageBox::Yes, QMessageBox::Yes);
    }

    if (FAIL == CreateMeetingTab(MEETINGTABLE))
    {
        QMessageBox::information(this, "错误", "打开数据库表失败",QMessageBox::Yes, QMessageBox::Yes);
    }

    std::list <MeetingListStr> lists;

    PrintGroupByMeetingTable(lists , MEETINGTABLE);
    list<MeetingListStr>::iterator list;
    for ( list = lists.begin(); list != lists.end(); ++list)
    {
        QString tim = QString::fromStdString(list->start_time) + "-" + QString::fromStdString(list->end_time);
        if(tim == arg1)
        {
            std::cout << tim.toStdString() << std::endl;
            break;
        }
    }
    //std::cout << "find time :" << list->start_time << "/" << list->end_time << std::endl;

    //std::list <MeetingInfoStr> infolists;
    PeopleMeetinglists.clear();
    GetMeetingListByTime(PeopleMeetinglists , list->start_time , list->end_time , MEETINGTABLE);
    std::list<MeetingInfoStr>::iterator m;

    std::cout << "list size :" << PeopleMeetinglists.size() << std::endl;
    QString imageFolderPath = IMAGEPATH;
    int count = 0;
    Meetingmodel->clear();
    Meetingmodel->setRowCount(PeopleMeetinglists.size());
    for ( m = PeopleMeetinglists.begin(); m != PeopleMeetinglists.end(); ++m)
    {
        QString filepath = imageFolderPath + QString::fromStdString(m->name).append(".png");

        QIcon icon;
        icon.addFile(filepath);
        ui->MeetingTotal->setRowHeight(count, 50);
        Meetingmodel->setItem(count , 0 , new QStandardItem(icon , 0));
        Meetingmodel->setItem(count , 1, new QStandardItem(QString::fromStdString(m->name)));
        Meetingmodel->setItem(count , 2, new QStandardItem(QString::fromStdString(m->start_time)));
        Meetingmodel->setItem(count , 3, new QStandardItem(QString::fromStdString(m->end_time)));
        Meetingmodel->setItem(count , 4, new QStandardItem(QString::fromStdString(m->place)));
        /*ui->MeetingTotal->setColumnWidth(0 , 50);
        ui->MeetingTotal->setColumnWidth(1 , 100);
        ui->MeetingTotal->setColumnWidth(2 , 200);
        ui->MeetingTotal->setColumnWidth(3 , 200);
        ui->MeetingTotal->setColumnWidth(4 , 135);*/
        count++;
    }

    if (FAIL == CreateDatabaseConnectionSwitch(db , OFF))
    {
        QMessageBox::information(this, "错误", "关闭数据库失败",QMessageBox::Yes, QMessageBox::Yes);
    }
}

/* 接收从MeetingThread线程发送过来的图像数据进行显示 */
void MeetingScene::acceptImage(QImage msg)
{
    QPixmap pixmap =QPixmap::fromImage(msg);
    pixmap.scaled(ui->video->size(), Qt::KeepAspectRatio);
    this->ui->video->setPixmap(QPixmap::fromImage(msg));
}

void MeetingScene::acceptplayImage(QImage msg)
{
    QPixmap pixmap =QPixmap::fromImage(msg);
    QSize picSize(ui->PlayBackLable->width(),ui->PlayBackLable->height());
    pixmap.scaled(picSize);
    ui->PlayBackLable->setPixmap(pixmap);
}

void MeetingScene::acceptfacename(QString msg)
{
#if 1
    int count = 0;
    switch(MeetingFlag)
    {
    case 1:
    case 2:
    {
        if(PeopleMeetinglists.empty())
            break;
        std::list<MeetingInfoStr>::iterator m;
        for(m = PeopleMeetinglists.begin(); m != PeopleMeetinglists.end(); ++m)
        {
            //不区分大小写比较
            if((msg.compare(QString::fromStdString(m->name) , Qt::CaseInsensitive) == 0) && ((m->status == 0) || (m->status == 3)))
            {
                std::cout << msg.toStdString() <<" is comming for meeting!!" << std::endl;

                if(m->status == 0)
                {
                    m->status = 1;/* 正常签到 */
                    Meetingmodel->setItem(count , 5, new QStandardItem(QString::fromStdString("已签到")));
                }
                else if(m->status == 3)
                {
                    /* 之前标记为缺席，改为迟到，并从缺席列表中删除，添加到迟到列表 */
                    QString imageFolderPath = IMAGEPATH;
                    m->status = 2;

                    QString name = msg.toLower();/* python 脚本将名字首字母改成大写，所以在这里改为小写再比较 */
                    QList<QStandardItem*> list = Absentmodel->findItems(name , Qt::MatchExactly , 1);
                    if(list.size() > 0)
                    {
                        Absentmodel->removeRow(list[0]->row(),QModelIndex());
                    }
                    else
                        std::cout << "not found " << msg.toStdString() << std::endl;

                    Latemodel->setRowCount(Latemodel->rowCount(QModelIndex()));
                    QString filepath = imageFolderPath + QString::fromStdString(m->name).append(".png");

                    int latetotal =Latemodel->rowCount(QModelIndex());
                    QIcon icon;
                    icon.addFile(filepath);
                    ui->LateView->setRowHeight(latetotal, 50);
                    Latemodel->setItem(latetotal , 0 , new QStandardItem(icon , 0));
                    Latemodel->setItem(latetotal , 1, new QStandardItem(QString::fromStdString(m->name)));
                }
                //m->status = (MeetingFlag==1)?1:2;


                //int i;
                //for(i=0;i<(Meetingmodel->rowCount());i++)
                //    Meetingmodel->item(count, i)->setBackground(QBrush(MeetingFlag==1?QColor(0, 255, 0):QColor(255, 0, 0)));
            }
            count++;
        }
    }
        break;
    case 3:break;/* 会议已结束 */
    default:break;/* 未到开会时间 */
    }
#endif
}

void MeetingScene::acceptupdateplaylist(QString msg)
{
    std::cout << "add file to playlist::" << msg.toStdString() << std::endl;
    QStandardItem *item = new QStandardItem(msg);
    VideoListModel->appendRow(item);
}

void MeetingScene::TimerTimeOut(void)
{
    uint current_time = QDateTime::currentDateTime().toTime_t();
    if(PeopleMeetinglists.empty())
        return;
#if 1
    uint meeting_start_time = QDateTime::fromString(QString::fromStdString(PeopleMeetinglists.front().start_time), "yyyy.MM.dd hh:mm:ss").toTime_t();
    uint meeting_end_time = QDateTime::fromString(QString::fromStdString(PeopleMeetinglists.front().end_time), "yyyy.MM.dd hh:mm:ss").toTime_t();

    //std::cout << "meeting time:" <<PeopleMeetinglists.front().start_time<<std::endl;

    if(meeting_start_time >= current_time)
    {
        if((meeting_start_time - current_time) <= MEETING_TIME_SECTION)
            MeetingFlag = 1;/* 登记参会人员时间段 */
        else {
            MeetingFlag = 0;/* 未到开会时间 */
        }
    }
    else
    {
        uint meeting_length = meeting_end_time - meeting_start_time;
        uint late_time_length = current_time - meeting_start_time;
        if(meeting_length > late_time_length && MeetingFlag == 1)
        {
            MeetingFlag =2 ;/* 会议正在进行 */
            std::cout << "meeting is start!!!!" << std::endl;
            std::cout << "add person to uncome table" << std::endl;

            /* 会议已开始，将未到的人员添加到缺席表中 */
            int i=0;
            QString imageFolderPath = IMAGEPATH;
            std::list<MeetingInfoStr>::iterator m;
            for(m = PeopleMeetinglists.begin(); m != PeopleMeetinglists.end(); ++m)
            {

                if(m->status == 0)
                {/* 会议开始还没有到的，置为缺席，会议中到改成迟到 */
                    m->status = 3;
                    Absentmodel->setRowCount(i+1);
                    QString filepath = imageFolderPath + QString::fromStdString(m->name).append(".png");

                    QIcon icon;
                    icon.addFile(filepath);
                    ui->MeetingTotal->setRowHeight(i, 50);
                    Absentmodel->setItem(i , 0 , new QStandardItem(icon , 0));
                    Absentmodel->setItem(i , 1, new QStandardItem(QString::fromStdString(m->name)));
                    i++;
                }
            }

            QString videopath =VIDEOPATH;
            QDateTime save_meeting_start_time = QDateTime::fromString(QString::fromStdString(PeopleMeetinglists.front().start_time), "yyyy.MM.dd hh:mm:ss");
            QDateTime save_meeting_end_time = QDateTime::fromString(QString::fromStdString(PeopleMeetinglists.front().end_time), "yyyy.MM.dd hh:mm:ss");

            std::cout <<  save_meeting_start_time.toString().toStdString() << std::endl;
            std::cout <<  save_meeting_end_time.toString().toStdString() << std::endl;
            //Meeting_thread->videoname = QString::fromStdString(PeopleMeetinglists.front().start_time.append("-").append(PeopleMeetinglists.front().end_time).append(".avi"));
            Meeting_thread->videoname = videopath.append(save_meeting_start_time.toString("yyyyMMddhhmmss")).append("-").append(save_meeting_end_time.toString("yyyyMMddhhmmss").append(".avi"));

            std::cout << "save file name:" <<Meeting_thread->videoname.toStdString() <<  std::endl;
            Meeting_thread->setrecoder(START_RECORDER);
        }
        else if ((meeting_length < late_time_length) &&  (MeetingFlag == 2)){
            MeetingFlag = 3;/* 会议已结束 */
            std::cout << "meeting is end!!!" << std::endl;
            Meeting_thread->setrecoder(STOP_RECORDER);
        }
    }
#endif
}

void MeetingScene::UpdateTableTimeOut(void)
{
    if(PeopleMeetinglists.empty())
        return;

    std::list<MeetingInfoStr>::iterator m;
    for(m = PeopleMeetinglists.begin(); m != PeopleMeetinglists.end(); ++m)
    {
    }
}

void MeetingScene::closeEvent( QCloseEvent * event )//关闭窗口会先处理该事件函数
{
    std::cout << "end thread" << std::endl;
    Meeting_thread->stop();
    std::cout << "closed and clear resources" <<std::endl;
}


/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/
/******************************************************************************************************************************/
extern QImage MatToQImage(const cv::Mat& mat);

MeetingThread::MeetingThread()
{
    MeetingRunning = true;
}

void MeetingThread::run()
{
    qDebug()<<"Visitor Thread Running !!";
    MeetingRunning = true;

    Timer timer;

    MeetingCap.open(0);
    if(!MeetingCap.isOpened())
    {
            printf("open video file failed\n");
            return;
    }
    else
            printf("open video ok\n");

    MeetingCap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    MeetingCap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);

    cv::Mat frame;
    if (!MeetingCap.read(frame)) {
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
    // ----------------------------------------------------------------------------------------------------

    // --------------------------- 3. Doing inference -----------------------------------------------------
    // Starting inference & calculating performance
    std::cout << "Start inference " << std::endl;

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
    frameReadStatus = MeetingCap.read(frame);

    //Timer timer;
    timer.start("face_identified");

    VideoWriter vw;

    RecorderStatus = STOP_RECORDER;
    uint LastRecorderStatus =  STOP_RECORDER;

    while (MeetingRunning) {
        timer.start("total");
        framesCounter++;
        isLastFrame = !frameReadStatus;

        // Retrieving face detection results for the previous frame
        faceDetector.wait();
        faceDetector.fetchResults();

        auto prev_detection_results = faceDetector.results;

        // No valid frame to infer if previous frame is the last
        if (!isLastFrame) {
            faceDetector.enqueue(frame);
            faceDetector.submitRequest();
        }

        // Reading the next frame if the current one is not the last
        if (!isLastFrame) {
            frameReadStatus = MeetingCap.read(next_frame);
            if (FLAGS_loop_video && !frameReadStatus) {
                if (!(FLAGS_i == "cam" ? MeetingCap.open(0) : MeetingCap.open(FLAGS_i))) {
                    throw std::logic_error("Cannot open input file or camera: ");
                }
                frameReadStatus = MeetingCap.read(next_frame);
            }
        }

        //  Postprocessing
        std::list<Face::Ptr> prev_faces;

        if (!FLAGS_no_smooth) {
            prev_faces.insert(prev_faces.begin(), faces.begin(), faces.end());
        }

        faces.clear();

        std::vector<cv::Mat> face_rois, landmarks, embeddings;

        /* 选出最大的人脸，会议场景每次只识别一个人*/
        Mat Max_Pic;
        size_t max_t = 0;
        for (size_t i = 0; i < prev_detection_results.size(); i++) {
            auto& result = prev_detection_results[i];
            cv::Rect rect = result.location & cv::Rect(0, 0, width, height);

            Face::Ptr face;
            face = std::make_shared<Face>(id++, rect);
            Mat pic =prev_frame(face->_location);
            if((pic.rows > Max_Pic.rows) && (pic.cols > Max_Pic.cols))
            {
                Max_Pic = pic;
                max_t = i;
            }
        }

        /* 之识别选中的最大人脸 */
        for (size_t i = 0; i < prev_detection_results.size(); i++) {
            if(i == max_t)
            {
                auto& result = prev_detection_results[i];
                cv::Rect rect = result.location & cv::Rect(0, 0, width, height);

                Face::Ptr face;
                face = std::make_shared<Face>(id++, rect);

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
                    sendfacename(QString::fromStdString(face_gallery.GetLabelByID(ids[i])));
                }
                else {
                    face->updatefacename("Unknown");
                }

                faces.push_back(face);
                break;
            }
        }

        //sendpic(MatToQImage(Max_Pic));
        //  Visualizing results
        visualizer->draw(prev_frame, faces);

        QImage image = QImage(prev_frame.data,prev_frame.cols,prev_frame.rows,QImage::Format_RGB888).rgbSwapped();
        sendImage(image);

        if(START_RECORDER == RecorderStatus)
        {
            /* 开始录像 */
            if(LastRecorderStatus == STOP_RECORDER)
            {
                int fps = MeetingCap.get(CAP_PROP_FPS);  //获取摄像机帧率
                if (fps <= 0)fps = 25;
                vw.open(videoname.toStdString(), //路径
                    VideoWriter::fourcc('X', '2', '6', '4'), //编码格式
                    fps, //帧率
                    Size(MeetingCap.get(CAP_PROP_FRAME_WIDTH),MeetingCap.get(CAP_PROP_FRAME_HEIGHT))  //尺寸
                    );
                if (!vw.isOpened())
                {
                    cout << "VideoWriter open failed!" << endl;
                }
                cout << "VideoWriter open success!" << endl;
            }

            vw.write(prev_frame);
        }
        else if(RecorderStatus == STOP_RECORDER && LastRecorderStatus == START_RECORDER)
        {
            vw.release();
            sendupdateplaylist(videoname);
        }
        LastRecorderStatus = RecorderStatus;


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
    MeetingCap.release();

    //关闭数据库
    CreateDatabaseConnectionSwitch(db , OFF);

    qDebug()<<"Pthread exit!!";
}

void MeetingThread::setrecoder(uint msg)
{
    std::cout << "recv msg:" << msg << std::endl;
    RecorderStatus = msg;
}

void MeetingThread::stop()
{
    MeetingRunning = false;   //线程退出标志
    qDebug() << "Meeting Thread end";
}

void MeetingScene::on_PlayList_clicked(const QModelIndex &index)
{
    std::cout << "choice index:" << index.row() << std::endl;
}

extern QString PlayFileName;
void MeetingScene::on_PlayList_doubleClicked(const QModelIndex &index)
{
    std::cout << "double cliched index:" << index.row() << std::endl;

    QString videopath = VIDEOPATH;
    PlayFileName = videopath.append(QString::fromStdString(VideoListModel->item(index.row() , 0 )->text().toStdString())).append(QString::fromStdString(VIDEOSUFFIX));
    PlayBack PlayBackDla;

    std::cout << "Play File:" << PlayFileName.toStdString() << std::endl;

    if(PlayBackDla.exec() != PlayBack::Accepted)//如果是取消就返回
    {
        return;
    }
/*
    if(PlayBack_thread->PlayBackRunning == true)
    {
        //正在播放视频，先停止，然后载运行
        std::cout << "stop play file!!" <<std::endl;
        PlayBack_thread->stop();
        QThread::msleep(300);
    }

    PlayBack_thread->PlayFileName = QString::fromStdString(VideoListModel->item(index.row() , 0 )->text().toStdString());

    std::cout << "Play File Name :" << PlayBack_thread->PlayFileName.toStdString() << std::endl;

    PlayBack_thread->PlayBackRunning = true;
    PlayBack_thread->start();
    QObject::connect(PlayBack_thread, SIGNAL(sendplayImage(QImage)), this, SLOT(acceptplayImage(QImage)));*/
}



