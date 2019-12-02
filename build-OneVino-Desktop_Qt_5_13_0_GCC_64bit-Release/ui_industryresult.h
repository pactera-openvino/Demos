/********************************************************************************
** Form generated from reading UI file 'industryresult.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INDUSTRYRESULT_H
#define UI_INDUSTRYRESULT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_IndustryResult
{
public:
    QLabel *VideoLable;
    QLabel *label_2;

    void setupUi(QDialog *IndustryResult)
    {
        if (IndustryResult->objectName().isEmpty())
            IndustryResult->setObjectName(QString::fromUtf8("IndustryResult"));
        IndustryResult->resize(992, 637);
        VideoLable = new QLabel(IndustryResult);
        VideoLable->setObjectName(QString::fromUtf8("VideoLable"));
        VideoLable->setGeometry(QRect(10, 60, 970, 570));
        VideoLable->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 87, 83);"));
        label_2 = new QLabel(IndustryResult);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(390, 10, 161, 41));
        label_2->setStyleSheet(QString::fromUtf8("font: 25pt \"Ubuntu\";"));

        retranslateUi(IndustryResult);

        QMetaObject::connectSlotsByName(IndustryResult);
    } // setupUi

    void retranslateUi(QDialog *IndustryResult)
    {
        IndustryResult->setWindowTitle(QCoreApplication::translate("IndustryResult", "Dialog", nullptr));
        VideoLable->setText(QString());
        label_2->setText(QCoreApplication::translate("IndustryResult", "\350\257\206\345\210\253\347\273\223\346\236\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IndustryResult: public Ui_IndustryResult {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDUSTRYRESULT_H
