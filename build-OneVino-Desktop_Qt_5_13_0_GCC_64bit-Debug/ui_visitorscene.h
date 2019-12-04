/********************************************************************************
** Form generated from reading UI file 'visitorscene.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISITORSCENE_H
#define UI_VISITORSCENE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableView>
#include "qchartview.h"

QT_BEGIN_NAMESPACE

class Ui_visitorscene
{
public:
    QLabel *video;
    QTableView *TotalView;
    QTableView *DetailView;
    QChartView *EmChart;
    QChartView *SexChart;

    void setupUi(QDialog *visitorscene)
    {
        if (visitorscene->objectName().isEmpty())
            visitorscene->setObjectName(QString::fromUtf8("visitorscene"));
        visitorscene->resize(1920, 1080);
        visitorscene->setStyleSheet(QString::fromUtf8("QWidget#visitorscene{ border-image: url(:/bt/image/visitor_bg.png);}"));
        video = new QLabel(visitorscene);
        video->setObjectName(QString::fromUtf8("video"));
        video->setGeometry(QRect(50, 130, 1181, 621));
        video->setStyleSheet(QString::fromUtf8("background-color: rgb(46, 52, 54);"));
        TotalView = new QTableView(visitorscene);
        TotalView->setObjectName(QString::fromUtf8("TotalView"));
        TotalView->setGeometry(QRect(1280, 130, 601, 621));
        TotalView->setStyleSheet(QString::fromUtf8("background-color: rgb(52, 101, 164);"));
        DetailView = new QTableView(visitorscene);
        DetailView->setObjectName(QString::fromUtf8("DetailView"));
        DetailView->setGeometry(QRect(50, 825, 1181, 211));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu"));
        font.setPointSize(11);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        DetailView->setFont(font);
        DetailView->setStyleSheet(QString::fromUtf8("font: 11pt \"Ubuntu\";"));
        EmChart = new QChartView(visitorscene);
        EmChart->setObjectName(QString::fromUtf8("EmChart"));
        EmChart->setGeometry(QRect(1566, 820, 321, 221));
        EmChart->setStyleSheet(QString::fromUtf8("background-color: rgb(211, 215, 207);"));
        SexChart = new QChartView(visitorscene);
        SexChart->setObjectName(QString::fromUtf8("SexChart"));
        SexChart->setGeometry(QRect(1270, 820, 291, 221));
        SexChart->setStyleSheet(QString::fromUtf8("background-color: rgb(211, 215, 207);"));

        retranslateUi(visitorscene);

        QMetaObject::connectSlotsByName(visitorscene);
    } // setupUi

    void retranslateUi(QDialog *visitorscene)
    {
        visitorscene->setWindowTitle(QCoreApplication::translate("visitorscene", "Dialog", nullptr));
        video->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class visitorscene: public Ui_visitorscene {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISITORSCENE_H
