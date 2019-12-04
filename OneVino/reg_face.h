#ifndef REG_FACE_H
#define REG_FACE_H

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

#include "detectors.hpp"
#include "face.hpp"
#include "visualizer.hpp"

#include <ie_iextension.h>
#include <ext_list.hpp>

#include "opencv2/opencv.hpp"


#define SAVEFILEPATH       "/root/OneVino/Identify/image/"

class FaceDetectThread :public QThread
{
    Q_OBJECT
public:
    FaceDetectThread();
    virtual void run();  //线程函数，新创建的线程运行该函数内容。
    void stop();     //停止线程
signals:

    void sendimage(QImage msg);
private:
    bool FaceDetectRunning;   //运行标志

    cv::VideoCapture cap;
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_takePic_clicked();

    void acceptimage(QImage msg); //接收脸部图像，用于采集人脸
    void on_Cancel_clicked();

private:
    void closeEvent(QCloseEvent *event);

    Ui::Dialog *ui;

    cv::VideoCapture cap;
    FaceDetectThread *FaceDetect_thread;   //定义线程类型指针

    QPixmap SavePixmap;
    bool SavePic;
    bool PreviewFlag = true;
    bool FirstRun = false;
};

#endif // REG_FACE_H
