#include "addmeeting.h"
#include "ui_addmeeting.h"

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
#include <QDir>
#include <QMessageBox>

#include "database.hpp"
#include "meetingscene.h"

using namespace std;

AddMeeting::AddMeeting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMeeting)
{
    ui->setupUi(this);

    ui->AddPeople->setStyleSheet("QPushButton{background-image: url(:/bt/image/right.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/right_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/right_hover.png);border:none;}");

    ui->DelPeople->setStyleSheet("QPushButton{background-image: url(:/bt/image/left.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/left_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/left_hover.png);border:none;}");

    ui->pushButton_3->setStyleSheet("QPushButton{background-image: url(:/bt/image/save.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/save_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/save_hover.png);border:none;}");

    setFixedSize(this->width(),this->height());

    QString imageFolderPath = IMAGEPATH;
    QDir dir(imageFolderPath);
    if (!dir.exists()) {
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList filters;
    filters << "*.png";
    dir.setNameFilters(filters);

    QStringList allImageNameList = dir.entryList();
    QFileInfoList fileInfoList = dir.entryInfoList();

    std::cout << "file num:" << allImageNameList.count() << std::endl;
    if (allImageNameList.count() <= 0)
    {
        return;
    }
    for (int i = 0; i<allImageNameList.count(); ++i)
    {
        std::cout << "filename:" << fileInfoList.at(i).baseName().toStdString() << std::endl;
        QStandardItem *item = new QStandardItem(fileInfoList.at(i).baseName());
        TotalModel->appendRow(item);
    }
    ui->TotalList->setModel(TotalModel);

    ui->MeetingList->setModel(MeetingModel);


    ui->StartTime->setCalendarPopup(true);
    ui->StartTime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->StartTime->setMinimumDate(QDate::currentDate());
    ui->StartTime->setMaximumDate(QDate::currentDate().addDays(365));

    ui->EndTime->setCalendarPopup(true);
    ui->EndTime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->EndTime->setMinimumDate(QDate::currentDate());
    ui->EndTime->setMaximumDate(QDate::currentDate().addDays(365));
}

AddMeeting::~AddMeeting()
{
    delete ui;
}

void AddMeeting::on_AddPeople_clicked()
{
    /* 添加人物到会议人员列表 */
    std::cout << "select index : " << ui->TotalList->currentIndex().row() << std::endl;

    QStandardItem *item = new QStandardItem(ui->TotalList->currentIndex().data().toString());
    MeetingModel->appendRow(item);

    TotalModel->removeRows(ui->TotalList->currentIndex().row() , 1);
}

void AddMeeting::on_DelPeople_clicked()
{
    /* 从会议列表中删除指定人员 */
    std::cout << "select index : " << ui->MeetingList->currentIndex().row() << std::endl;

    QStandardItem *item = new QStandardItem(ui->MeetingList->currentIndex().data().toString());
    TotalModel->appendRow(item);
    MeetingModel->removeRows(ui->MeetingList->currentIndex().row() , 1);
}

void AddMeeting::on_pushButton_3_clicked()
{
    if(ui->MeetingPlaceEdit->text().toStdString().empty() == true)
    {
        QMessageBox::information(this, "错误", "请输入会议地点",QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    std::cout << ui->MeetingPlaceEdit->text().toStdString() << std::endl;
    std::cout << "add people to meeting:" << ui->MeetingList->model()->rowCount()<< std::endl;
    /* 将列表添加到数据库 */
    if (FAIL == CreateDatabaseConnectionSwitch(db , ON))
    {
        QMessageBox::information(this, "错误", "打开数据库失败",QMessageBox::Yes, QMessageBox::Yes);
    }

    if (FAIL == CreateMeetingTab(MEETINGTABLE))
    {
        QMessageBox::information(this, "错误", "打开数据库表失败",QMessageBox::Yes, QMessageBox::Yes);
    }

    for(int i=0;i< ui->MeetingList->model()->rowCount();i++)
    {
        std::cout << "item:" << i << "/" << MeetingModel->item(i , 0 )->text().toStdString() <<std::endl;

        MeetingInfoStr pt;
        pt.name = MeetingModel->item(i , 0 )->text().toStdString();
        pt.start_time =ui->StartTime->dateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString();
        pt.end_time = ui->EndTime->dateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString();
        pt.place =ui->MeetingPlaceEdit->text().toStdString();
        AddOnePerson2Meeting(&pt , MEETINGTABLE);
    }
    PrintWholeMeetingTable(MEETINGTABLE);

    if (FAIL == CreateDatabaseConnectionSwitch(db , OFF))
    {
        QMessageBox::information(this, "错误", "关闭数据库失败",QMessageBox::Yes, QMessageBox::Yes);
    }

    this->close();
}
