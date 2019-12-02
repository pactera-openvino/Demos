#ifndef MEETINGSCENE_H
#define MEETINGSCENE_H

#include <iostream>
#include <fstream>

#include <QDialog>
#include <QThread>
#include "QTimer"
#include "QImage"
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QDebug>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include <QtCharts>

#include "detectors.hpp"
#include "face.hpp"
#include "visualizer.hpp"

#include <ie_iextension.h>
#include <ext_list.hpp>

#include "opencv2/opencv.hpp"

#include "database.hpp"

using namespace std;

namespace Ui {
class MeetingScene;
}


#define IMAGEPATH   "/root/OneVino/Identify/image/"

#define VIDEOSUFFIX ".avi"
#define VIDEOPATH   "/root/OneVino/MeetingRecorder/"
#define MEETING_TIME_SECTION    900/* 参会时间为会议开始时间的前15分钟以内 */


#define START_RECORDER      1
#define STOP_RECORDER       2

class MeetingThread :public QThread
{
    Q_OBJECT
public:
    MeetingThread();
    virtual void run();  //线程函数，新创建的线程运行该函数内容。
    void stop();     //停止线程

    void setrecoder(uint msg);

    QString videoname;

signals:
    void sendImage(QImage msg);
    void sendfacename(QString name);

    void sendupdateplaylist(QString msg);

private:
    bool MeetingRunning;   //运行标志

    cv::VideoCapture MeetingCap;


    uint RecorderStatus = 0;
};


class MeetingScene : public QDialog
{
    Q_OBJECT

public:
    explicit MeetingScene(QWidget *parent = nullptr);
    ~MeetingScene();

private slots:
    void closeEvent( QCloseEvent * event );

    void on_AddMeeting_bt_clicked();
    void on_MeetingList_activated(const QString &arg1);

    void acceptImage(QImage msg);
    void acceptfacename(QString msg);

    void acceptplayImage(QImage msg);


   void TimerTimeOut();/* 定时器超时函数，用于判断是否到会议时间 */
   void UpdateTableTimeOut();/* 定时器超时函数，用于判断是否到会议时间 */

   void acceptupdateplaylist(QString msg);

   void on_PlayList_clicked(const QModelIndex &index);

   void on_PlayList_doubleClicked(const QModelIndex &index);

private:
    Ui::MeetingScene *ui;
    MeetingThread *Meeting_thread;   //定义线程类型指针

    std::list <MeetingInfoStr> PeopleMeetinglists;
    QStandardItemModel* Meetingmodel = new QStandardItemModel();
    QStandardItemModel* Latemodel = new QStandardItemModel();
    QStandardItemModel* Absentmodel = new QStandardItemModel();

    QStandardItemModel *VideoListModel = new QStandardItemModel(this);

    uchar SendSigalRec;/* 发送信号开始录像 , 发送信号停止录像 */
    uchar MeetingFlag;/* 0:未到开会时间  1：正在登记参会人员  2：会议已经开始（此时间来的是迟到） 3：会议结束（一直未登记参会的是缺席） */
    QTimer *m_timer = new QTimer;

    QTimer *updatetimer = new QTimer;


    //PlayBackThread *PlayBack_thread = new (PlayBackThread);
};

#endif // MEETINGSCENE_H
