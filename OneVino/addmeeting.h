#ifndef ADDMEETING_H
#define ADDMEETING_H

#include <QDialog>
#include <QThread>
#include "QTimer"
#include "QImage"
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QDebug>

namespace Ui {
class AddMeeting;
}

class AddMeeting : public QDialog
{
    Q_OBJECT

public:
    explicit AddMeeting(QWidget *parent = nullptr);
    ~AddMeeting();

    QStandardItemModel *TotalModel = new QStandardItemModel(this);
    QStandardItemModel *MeetingModel = new QStandardItemModel(this);

private slots:
    void on_AddPeople_clicked();

    void on_DelPeople_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::AddMeeting *ui;
};

#endif // ADDMEETING_H
