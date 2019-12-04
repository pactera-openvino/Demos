/********************************************************************************
** Form generated from reading UI file 'playback.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYBACK_H
#define UI_PLAYBACK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_PlayBack
{
public:
    QLabel *PlayBackLable;

    void setupUi(QDialog *PlayBack)
    {
        if (PlayBack->objectName().isEmpty())
            PlayBack->setObjectName(QString::fromUtf8("PlayBack"));
        PlayBack->resize(1280, 720);
        PlayBack->setStyleSheet(QString::fromUtf8("QWidget#PlayBack{ background-image: url(:/bt/image/playback_bg.png);}"));
        PlayBackLable = new QLabel(PlayBack);
        PlayBackLable->setObjectName(QString::fromUtf8("PlayBackLable"));
        PlayBackLable->setGeometry(QRect(0, 0, 1280, 720));
        PlayBackLable->setStyleSheet(QString::fromUtf8("background:transparent;"));

        retranslateUi(PlayBack);

        QMetaObject::connectSlotsByName(PlayBack);
    } // setupUi

    void retranslateUi(QDialog *PlayBack)
    {
        PlayBack->setWindowTitle(QCoreApplication::translate("PlayBack", "\350\247\206\351\242\221\345\233\236\346\224\276", nullptr));
        PlayBackLable->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PlayBack: public Ui_PlayBack {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYBACK_H
