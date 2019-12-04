/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *lab_bt;
    QPushButton *meeting_bt;
    QPushButton *visitor_bt;
    QPushButton *reg_button;
    QPushButton *Industry_bt;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1920, 1080);
        MainWindow->setStyleSheet(QString::fromUtf8("background-image: url(:/bt/image/bg.png);"));
        MainWindow->setAnimated(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lab_bt = new QPushButton(centralWidget);
        lab_bt->setObjectName(QString::fromUtf8("lab_bt"));
        lab_bt->setGeometry(QRect(600, 377, 360, 358));
        lab_bt->setStyleSheet(QString::fromUtf8(""));
        lab_bt->setFlat(true);
        meeting_bt = new QPushButton(centralWidget);
        meeting_bt->setObjectName(QString::fromUtf8("meeting_bt"));
        meeting_bt->setGeometry(QRect(960, 377, 360, 358));
        meeting_bt->setFlat(true);
        visitor_bt = new QPushButton(centralWidget);
        visitor_bt->setObjectName(QString::fromUtf8("visitor_bt"));
        visitor_bt->setGeometry(QRect(240, 342, 360, 428));
        visitor_bt->setFlat(true);
        reg_button = new QPushButton(centralWidget);
        reg_button->setObjectName(QString::fromUtf8("reg_button"));
        reg_button->setGeometry(QRect(1800, 40, 91, 91));
        reg_button->setFlat(true);
        Industry_bt = new QPushButton(centralWidget);
        Industry_bt->setObjectName(QString::fromUtf8("Industry_bt"));
        Industry_bt->setGeometry(QRect(1320, 340, 360, 428));
        Industry_bt->setStyleSheet(QString::fromUtf8(""));
        Industry_bt->setFlat(true);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "OneVino", nullptr));
        lab_bt->setText(QString());
        meeting_bt->setText(QString());
        visitor_bt->setText(QString());
        reg_button->setText(QString());
        Industry_bt->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
