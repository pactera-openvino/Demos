/********************************************************************************
** Form generated from reading UI file 'reg_face.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REG_FACE_H
#define UI_REG_FACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *frame_label;
    QPushButton *takePic;
    QLineEdit *FileName;
    QLabel *label;
    QPushButton *Cancel;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(620, 628);
        Dialog->setStyleSheet(QString::fromUtf8("QWidget#Dialog{ background-image: url(:/bt/image/reg_bg.png);}"));
        frame_label = new QLabel(Dialog);
        frame_label->setObjectName(QString::fromUtf8("frame_label"));
        frame_label->setGeometry(QRect(170, 140, 280, 200));
        takePic = new QPushButton(Dialog);
        takePic->setObjectName(QString::fromUtf8("takePic"));
        takePic->setGeometry(QRect(340, 520, 88, 36));
        FileName = new QLineEdit(Dialog);
        FileName->setObjectName(QString::fromUtf8("FileName"));
        FileName->setGeometry(QRect(210, 450, 201, 41));
        FileName->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"color: rgb(255, 255, 255);"));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(100, 460, 101, 21));
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 15pt \"Ubuntu\";"));
        Cancel = new QPushButton(Dialog);
        Cancel->setObjectName(QString::fromUtf8("Cancel"));
        Cancel->setGeometry(QRect(190, 520, 88, 36));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        frame_label->setText(QString());
        takePic->setText(QString());
        label->setText(QCoreApplication::translate("Dialog", "\345\247\223\345\220\215(\350\213\261\346\226\207)", nullptr));
        Cancel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REG_FACE_H
