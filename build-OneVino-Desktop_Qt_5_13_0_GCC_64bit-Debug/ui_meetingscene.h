/********************************************************************************
** Form generated from reading UI file 'meetingscene.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEETINGSCENE_H
#define UI_MEETINGSCENE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_MeetingScene
{
public:
    QLabel *video;
    QTableView *MeetingTotal;
    QFrame *line_2;
    QTableView *LateView;
    QPushButton *AddMeeting_bt;
    QComboBox *MeetingList;
    QLabel *label_5;
    QTableView *AbsentView;
    QLabel *PlayBackLable;
    QListView *PlayList;

    void setupUi(QDialog *MeetingScene)
    {
        if (MeetingScene->objectName().isEmpty())
            MeetingScene->setObjectName(QString::fromUtf8("MeetingScene"));
        MeetingScene->resize(1920, 1080);
        MeetingScene->setStyleSheet(QString::fromUtf8("QWidget#MeetingScene{ background-image: url(:/bt/image/meeting_bg.png);}"));
        video = new QLabel(MeetingScene);
        video->setObjectName(QString::fromUtf8("video"));
        video->setGeometry(QRect(49, 130, 1181, 621));
        video->setStyleSheet(QString::fromUtf8("background-color: rgb(46, 52, 54);"));
        MeetingTotal = new QTableView(MeetingScene);
        MeetingTotal->setObjectName(QString::fromUtf8("MeetingTotal"));
        MeetingTotal->setGeometry(QRect(1280, 133, 601, 621));
        MeetingTotal->setStyleSheet(QString::fromUtf8("background:transparent;"));
        line_2 = new QFrame(MeetingScene);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(1915, 56, 5, 1114));
        line_2->setMouseTracking(false);
        line_2->setTabletTracking(false);
        line_2->setAcceptDrops(false);
        line_2->setAutoFillBackground(false);
        line_2->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        line_2->setLineWidth(0);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        LateView = new QTableView(MeetingScene);
        LateView->setObjectName(QString::fromUtf8("LateView"));
        LateView->setGeometry(QRect(40, 829, 571, 201));
        LateView->setStyleSheet(QString::fromUtf8("background:transparent;"));
        AddMeeting_bt = new QPushButton(MeetingScene);
        AddMeeting_bt->setObjectName(QString::fromUtf8("AddMeeting_bt"));
        AddMeeting_bt->setGeometry(QRect(1750, 30, 131, 31));
        AddMeeting_bt->setStyleSheet(QString::fromUtf8("background-color: rgb(32, 74, 135);\n"
"color: rgb(238, 238, 236);"));
        MeetingList = new QComboBox(MeetingScene);
        MeetingList->setObjectName(QString::fromUtf8("MeetingList"));
        MeetingList->setGeometry(QRect(1560, 80, 321, 30));
        MeetingList->setStyleSheet(QString::fromUtf8("background-color: rgb(32, 74, 135);\n"
"color: rgb(238, 238, 236);"));
        label_5 = new QLabel(MeetingScene);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(1480, 90, 67, 17));
        label_5->setStyleSheet(QString::fromUtf8("color:#ffFFFF;"));
        AbsentView = new QTableView(MeetingScene);
        AbsentView->setObjectName(QString::fromUtf8("AbsentView"));
        AbsentView->setGeometry(QRect(660, 830, 571, 201));
        AbsentView->setStyleSheet(QString::fromUtf8("background:transparent;"));
        PlayBackLable = new QLabel(MeetingScene);
        PlayBackLable->setObjectName(QString::fromUtf8("PlayBackLable"));
        PlayBackLable->setGeometry(QRect(1090, 750, 430, 241));
        PlayList = new QListView(MeetingScene);
        PlayList->setObjectName(QString::fromUtf8("PlayList"));
        PlayList->setGeometry(QRect(1300, 825, 601, 201));
        PlayList->setStyleSheet(QString::fromUtf8("background:transparent;"));

        retranslateUi(MeetingScene);

        QMetaObject::connectSlotsByName(MeetingScene);
    } // setupUi

    void retranslateUi(QDialog *MeetingScene)
    {
        MeetingScene->setWindowTitle(QCoreApplication::translate("MeetingScene", "Dialog", nullptr));
        video->setText(QString());
#if QT_CONFIG(accessibility)
        line_2->setAccessibleName(QString());
#endif // QT_CONFIG(accessibility)
        AddMeeting_bt->setText(QCoreApplication::translate("MeetingScene", "\346\267\273\345\212\240\344\274\232\350\256\256", nullptr));
        label_5->setText(QCoreApplication::translate("MeetingScene", "\344\274\232\350\256\256\351\200\211\346\213\251", nullptr));
        PlayBackLable->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MeetingScene: public Ui_MeetingScene {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEETINGSCENE_H
