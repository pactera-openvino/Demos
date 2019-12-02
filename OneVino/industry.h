#ifndef INDUSTRY_H
#define INDUSTRY_H

#include <QDialog>

#define REFERENCEFILE   "/root/OneVino/reference/pcb.jpg"

namespace Ui {
class Industry;
}

class Industry : public QDialog
{
    Q_OBJECT

public:
    explicit Industry(QWidget *parent = nullptr);
    ~Industry();

private slots:
    void on_pushButton_2_clicked();

    //void on_pushButton_4_clicked();

private:
    Ui::Industry *ui;
};

#endif // INDUSTRY_H
