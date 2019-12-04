/********************************************************************************
** Form generated from reading UI file 'visitor.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISITOR_H
#define UI_VISITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableView>
#include "qchartview.h"

QT_BEGIN_NAMESPACE

class Ui_Visitor
{
public:
    QLabel *video;
    QTableView *DetailView;
    QTableView *TotalView;
    QChartView *SexChart;
    QChartView *EmChart;

    void setupUi(QDialog *Visitor)
    {
        if (Visitor->objectName().isEmpty())
            Visitor->setObjectName(QString::fromUtf8("Visitor"));
        Visitor->resize(1920, 1080);
        QFont font;
        font.setPointSize(33);
        font.setKerning(false);
        Visitor->setFont(font);
        Visitor->setWindowTitle(QString::fromUtf8("Dialog"));
#if QT_CONFIG(accessibility)
        Visitor->setAccessibleName(QString::fromUtf8(""));
#endif // QT_CONFIG(accessibility)
        Visitor->setStyleSheet(QString::fromUtf8("QWidget#Visitor{ background-image: url(:/bt/image/visitor_bg.png);}"));
        video = new QLabel(Visitor);
        video->setObjectName(QString::fromUtf8("video"));
        video->setGeometry(QRect(49, 135, 1181, 611));
        video->setStyleSheet(QString::fromUtf8("background-color: rgb(46, 52, 54);"));
        DetailView = new QTableView(Visitor);
        DetailView->setObjectName(QString::fromUtf8("DetailView"));
        DetailView->setGeometry(QRect(43, 820, 1195, 216));
        DetailView->setStyleSheet(QString::fromUtf8("background:transparent;\n"
""));
        TotalView = new QTableView(Visitor);
        TotalView->setObjectName(QString::fromUtf8("TotalView"));
        TotalView->setGeometry(QRect(1270, 122, 618, 636));
        TotalView->setStyleSheet(QString::fromUtf8("font: 11pt \"Ubuntu\";\n"
"background:transparent;"));
        SexChart = new QChartView(Visitor);
        SexChart->setObjectName(QString::fromUtf8("SexChart"));
        SexChart->setGeometry(QRect(1270, 820, 301, 221));
        SexChart->setStyleSheet(QString::fromUtf8("background:transparent;"));
        EmChart = new QChartView(Visitor);
        EmChart->setObjectName(QString::fromUtf8("EmChart"));
        EmChart->setGeometry(QRect(1586, 820, 321, 221));
        EmChart->setStyleSheet(QString::fromUtf8("background:transparent;"));

        retranslateUi(Visitor);

        QMetaObject::connectSlotsByName(Visitor);
    } // setupUi

    void retranslateUi(QDialog *Visitor)
    {
        video->setText(QString());
        Q_UNUSED(Visitor);
    } // retranslateUi

};

namespace Ui {
    class Visitor: public Ui_Visitor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISITOR_H
