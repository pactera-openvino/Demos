#include "industryresult.h"
#include "ui_industryresult.h"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

using namespace std;
using namespace cv;

extern Mat ResultImage;
extern QImage MatToQImage(const cv::Mat& mat);

IndustryResult::IndustryResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IndustryResult)
{
    ui->setupUi(this);

    QPixmap pixmap = QPixmap::fromImage(MatToQImage(ResultImage));
    int width = ui->VideoLable->width();
    int height = ui->VideoLable->height();
    QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->ui->VideoLable->setPixmap(fitpixmap);
}

IndustryResult::~IndustryResult()
{
    delete ui;
}
