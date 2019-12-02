#include "mainwindow.h"
#include "ui_mainwindow.h"

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
#include <QMessageBox>

#include "reg_face.h"
#include "visitor.h"
#include "meetingscene.h"
#include "licplate.h"
#include "industry.h"
#include "visitorscene.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    QList <QString> faces_list;
    faces_list.reserve(3);
    faces_list.push_back("test1");
    std::cout <<"list size:" <<faces_list.size() << std::endl;
    faces_list.push_back("test2");
    std::cout <<"list size:" <<faces_list.size() << std::endl;
    faces_list.push_back("test3");
    std::cout <<"list size:" <<faces_list.size() << std::endl;
    faces_list.push_back("test4");
    std::cout <<"list size:" <<faces_list.size() << std::endl;
    faces_list.push_back("test3");
    std::cout <<"list size:" <<faces_list.size() << std::endl;

    std::cout << "cout :" <<  faces_list.count("test3") << std::endl;
*/

    ui->reg_button->setStyleSheet("QPushButton{background-image: url(:/bt/image/info.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/info_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/info_hover.png);border:none;}");

    ui->visitor_bt->setStyleSheet("QPushButton{background-image: url(:/bt/image/user.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/user_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/user_hover.png);border:none;}");

    ui->meeting_bt->setStyleSheet("QPushButton{background-image: url(:/bt/image/conference.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/conference_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/conference_hover.png);border:none;}");

    ui->lab_bt->setStyleSheet("QPushButton{background-image: url(:/bt/image/car.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/car_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/car_hover.png);border:none;}");

    ui->Industry_bt->setStyleSheet("QPushButton{background-image: url(:/bt/image/industry.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/industry_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/industry_hover.png);border:none;}");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_reg_button_clicked()
{
    std::cout << "Get Video Width/Height!!!!!!!!" << std::endl;
    Dialog dialog;
    if(dialog.exec() != QDialog::Accepted)//如果是取消就返回
    {
        return;
    }
}

void MainWindow::on_meeting_bt_clicked()
{
    MeetingScene dialog;
    dialog.setWindowModality(Qt::ApplicationModal);
    if(dialog.exec() != MeetingScene::Accepted)//如果是取消就返回
    {
        return;
    }
}

void MainWindow::on_lab_bt_clicked()
{
    //QMessageBox::information(this, "提示", "开发中，敬请期待!!!",QMessageBox::Yes, QMessageBox::Yes);

    LicPlate dialog;
    dialog.setWindowModality(Qt::ApplicationModal);
    if(dialog.exec() != LicPlate::Accepted)//如果是取消就返回
    {
        return;
    }
}

void MainWindow::on_visitor_bt_clicked()
{
    Visitor dialog;
    dialog.setWindowModality(Qt::ApplicationModal);
    if(dialog.exec() != Visitor::Accepted)//如果是取消就返回
    {
        return;
    }
}

void MainWindow::on_Industry_bt_clicked()
{
    Industry dialog;
    dialog.setWindowModality(Qt::ApplicationModal);
    if(dialog.exec() != Industry::Accepted)//如果是取消就返回
    {
        return;
    }
}
