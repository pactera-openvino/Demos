#ifndef INDUSTRYRESULT_H
#define INDUSTRYRESULT_H

#include <QDialog>

namespace Ui {
class IndustryResult;
}

class IndustryResult : public QDialog
{
    Q_OBJECT

public:
    explicit IndustryResult(QWidget *parent = nullptr);
    ~IndustryResult();

private:
    Ui::IndustryResult *ui;
};

#endif // INDUSTRYRESULT_H
