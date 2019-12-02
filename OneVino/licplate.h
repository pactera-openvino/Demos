#ifndef LICPLATE_H
#define LICPLATE_H

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







#define LICPLATEFILE    "/root/OneVino/1574134377794.mp4"






namespace Ui {
class LicPlate;
}



class LpRecognitionThread :public QThread
{
    Q_OBJECT
public:
    bool LprRunning;

    LpRecognitionThread();
    virtual void run();  //线程函数，新创建的线程运行该函数内容。
    void stop();     //停止线程
signals:
    void sendImage(QImage msg);
private:

};

class LicPlate : public QDialog
{
    Q_OBJECT

public:
    explicit LicPlate(QWidget *parent = nullptr);
    ~LicPlate();

private slots:
    void RefreshPic(void);

private:
    Ui::LicPlate *ui;

    QStandardItemModel *LprListModel = new QStandardItemModel(this);

    QTimer *m_refresh = new QTimer;

};

#endif // LICPLATE_H
