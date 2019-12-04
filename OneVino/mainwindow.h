#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTimer"
#include "QImage"
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QThread>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include "opencv2/opencv.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_reg_button_clicked();

    void on_meeting_bt_clicked();

    void on_lab_bt_clicked();

    void on_visitor_bt_clicked();

    void on_Industry_bt_clicked();

private:
    Ui::MainWindow *ui;

};



#endif // MAINWINDOW_H
