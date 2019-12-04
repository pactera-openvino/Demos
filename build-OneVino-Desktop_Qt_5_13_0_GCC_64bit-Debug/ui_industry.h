/********************************************************************************
** Form generated from reading UI file 'industry.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INDUSTRY_H
#define UI_INDUSTRY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Industry
{
public:
    QLabel *RTemplateLable;
    QLabel *ToBeIdentified;
    QPushButton *pushButton_2;

    void setupUi(QDialog *Industry)
    {
        if (Industry->objectName().isEmpty())
            Industry->setObjectName(QString::fromUtf8("Industry"));
        Industry->resize(1920, 1080);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Industry->sizePolicy().hasHeightForWidth());
        Industry->setSizePolicy(sizePolicy);
        Industry->setStyleSheet(QString::fromUtf8("QWidget#Industry{ background-image: url(:/bt/image/industry_bg.png);}"));
        RTemplateLable = new QLabel(Industry);
        RTemplateLable->setObjectName(QString::fromUtf8("RTemplateLable"));
        RTemplateLable->setGeometry(QRect(50, 130, 885, 901));
        RTemplateLable->setStyleSheet(QString::fromUtf8("background:transparent;"));
        ToBeIdentified = new QLabel(Industry);
        ToBeIdentified->setObjectName(QString::fromUtf8("ToBeIdentified"));
        ToBeIdentified->setGeometry(QRect(985, 130, 885, 901));
        ToBeIdentified->setStyleSheet(QString::fromUtf8("background:transparent;"));
        pushButton_2 = new QPushButton(Industry);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(1320, 0, 200, 116));

        retranslateUi(Industry);

        QMetaObject::connectSlotsByName(Industry);
    } // setupUi

    void retranslateUi(QDialog *Industry)
    {
        Industry->setWindowTitle(QCoreApplication::translate("Industry", "Dialog", nullptr));
        RTemplateLable->setText(QString());
        ToBeIdentified->setText(QString());
        pushButton_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Industry: public Ui_Industry {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDUSTRY_H
