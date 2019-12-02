/********************************************************************************
** Form generated from reading UI file 'addmeeting.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDMEETING_H
#define UI_ADDMEETING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AddMeeting
{
public:
    QListView *TotalList;
    QListView *MeetingList;
    QPushButton *AddPeople;
    QPushButton *DelPeople;
    QDateTimeEdit *StartTime;
    QPushButton *pushButton_3;
    QDateTimeEdit *EndTime;
    QLineEdit *MeetingPlaceEdit;

    void setupUi(QDialog *AddMeeting)
    {
        if (AddMeeting->objectName().isEmpty())
            AddMeeting->setObjectName(QString::fromUtf8("AddMeeting"));
        AddMeeting->resize(620, 506);
        AddMeeting->setStyleSheet(QString::fromUtf8("QWidget#AddMeeting{ background-image: url(:/bt/image/addmeeting_bg.png);}"));
        TotalList = new QListView(AddMeeting);
        TotalList->setObjectName(QString::fromUtf8("TotalList"));
        TotalList->setGeometry(QRect(150, 178, 175, 185));
        TotalList->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"color: rgb(255, 255, 255);"));
        MeetingList = new QListView(AddMeeting);
        MeetingList->setObjectName(QString::fromUtf8("MeetingList"));
        MeetingList->setGeometry(QRect(362, 178, 175, 185));
        MeetingList->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"color: rgb(255, 255, 255);"));
        AddPeople = new QPushButton(AddMeeting);
        AddPeople->setObjectName(QString::fromUtf8("AddPeople"));
        AddPeople->setGeometry(QRect(335, 250, 12, 12));
        DelPeople = new QPushButton(AddMeeting);
        DelPeople->setObjectName(QString::fromUtf8("DelPeople"));
        DelPeople->setGeometry(QRect(335, 280, 12, 12));
        StartTime = new QDateTimeEdit(AddMeeting);
        StartTime->setObjectName(QString::fromUtf8("StartTime"));
        StartTime->setGeometry(QRect(150, 120, 175, 35));
        StartTime->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"font: 13pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);"));
        pushButton_3 = new QPushButton(AddMeeting);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(390, 64, 88, 36));
        EndTime = new QDateTimeEdit(AddMeeting);
        EndTime->setObjectName(QString::fromUtf8("EndTime"));
        EndTime->setGeometry(QRect(362, 120, 175, 35));
        EndTime->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"font: 13pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);"));
        MeetingPlaceEdit = new QLineEdit(AddMeeting);
        MeetingPlaceEdit->setObjectName(QString::fromUtf8("MeetingPlaceEdit"));
        MeetingPlaceEdit->setGeometry(QRect(150, 64, 175, 31));
        MeetingPlaceEdit->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"color: rgb(255, 255, 255);"));

        retranslateUi(AddMeeting);

        QMetaObject::connectSlotsByName(AddMeeting);
    } // setupUi

    void retranslateUi(QDialog *AddMeeting)
    {
        AddMeeting->setWindowTitle(QCoreApplication::translate("AddMeeting", "\346\267\273\345\212\240\344\274\232\350\256\256", nullptr));
        AddPeople->setText(QString());
        DelPeople->setText(QString());
        pushButton_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AddMeeting: public Ui_AddMeeting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDMEETING_H
