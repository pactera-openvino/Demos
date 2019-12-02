#ifndef VISITOR_H
#define VISITOR_H

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

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Visitor;
}

class VisitorThread :public QThread
{
    Q_OBJECT
public:
    VisitorThread();
    virtual void run();  //线程函数，新创建的线程运行该函数内容。
    void stop();     //停止线程

    void UpdateTotalTable_TimeOut(void);
signals:
    void sendImage(QImage msg); //发送信号
    void sendTotalFace( int FaceNum); //发送信号
    void sendFaceInfo(Face::Ptr face , int ImageFaceId); //发送信号

    void updatechart(int malenum , int femalenum); //发送信号
    void inittable(int TotalRecord , struct personAttribute *pAttTotal); //初始化右侧列表框，发送数据更新右侧列表框
    void updatetable(struct personAttribute *pAtt );
    void updateEmChart(int *em);

    void sendpic(QImage msg);
private:
    bool VisitorRunning;   //运行标志

    cv::VideoCapture cap;
    QTimer *updatetabletimer;
};

class Visitor : public QDialog
{
    Q_OBJECT

public:
    void paintEvent(QPaintEvent *);
    explicit Visitor(QWidget *parent = nullptr);
    ~Visitor();

    QPieSeries *Sexseries;
    QPieSeries *Emseries;

    QStandardItemModel* Detailmodel = new QStandardItemModel(this);
    QStandardItemModel* TotalTableModel = new QStandardItemModel(this);
private slots:
    void closeEvent( QCloseEvent * event );

    void acceptImage(QImage msg);
    void acceptTotalFace(int FaceNum);
    void acceptFaceInfo(Face::Ptr face , int ImageFaceId);//接收访客线程发送的脸部信息的槽函数，可根据信息显示 , ImageFaceId此参数表示这是这张图像中的第几人的id
    void acceptupdatechart(int malenum , int femalenum);
    void acceptinittable(int TotalRecord , struct personAttribute *pAttTotal);
    void acceptupdatetable(struct personAttribute *pAtt);
    void acceptupdateEmchart(int *em);
private:
    Ui::Visitor *ui;
    VisitorThread *Visitor_thread;   //定义线程类型指针
};

#endif // VISITOR_H
