/********************************************************************************
** Form generated from reading UI file 'licplate.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LICPLATE_H
#define UI_LICPLATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>

QT_BEGIN_NAMESPACE

class Ui_LicPlate
{
public:
    QLabel *VideoLable;
    QListView *LprList;

    void setupUi(QDialog *LicPlate)
    {
        if (LicPlate->objectName().isEmpty())
            LicPlate->setObjectName(QString::fromUtf8("LicPlate"));
        LicPlate->resize(1920, 1080);
        LicPlate->setStyleSheet(QString::fromUtf8("QWidget#LicPlate{ background-image: url(:/bt/image/licplate_bg.png);}"));
        VideoLable = new QLabel(LicPlate);
        VideoLable->setObjectName(QString::fromUtf8("VideoLable"));
        VideoLable->setGeometry(QRect(50, 130, 1181, 901));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VideoLable->sizePolicy().hasHeightForWidth());
        VideoLable->setSizePolicy(sizePolicy);
        VideoLable->setStyleSheet(QString::fromUtf8("background:transparent;"));
        LprList = new QListView(LicPlate);
        LprList->setObjectName(QString::fromUtf8("LprList"));
        LprList->setGeometry(QRect(1280, 130, 591, 901));
        LprList->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"color: rgb(243, 243, 243);\n"
"font: 15pt \"Ubuntu\";"));

        retranslateUi(LicPlate);

        QMetaObject::connectSlotsByName(LicPlate);
    } // setupUi

    void retranslateUi(QDialog *LicPlate)
    {
        LicPlate->setWindowTitle(QCoreApplication::translate("LicPlate", "Dialog", nullptr));
        VideoLable->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LicPlate: public Ui_LicPlate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LICPLATE_H
