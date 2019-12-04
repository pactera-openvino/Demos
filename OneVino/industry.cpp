#include "industry.h"
#include "ui_industry.h"

#include <iostream>
#include <QFileDialog>
#include <QMessageBox>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include "industryresult.h"


using namespace std;
using namespace cv;


Mat InImage , RefImage;
Mat ResultImage;

extern QImage MatToQImage(const cv::Mat& mat);

Industry::Industry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Industry)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    showMaximized(); //默认最大化


    ui->pushButton_2->setStyleSheet("QPushButton{background-image: url(:/bt/image/choosefile.png);border:none;}"
                              "QPushButton:hover{background-image: url(:/bt/image/choosefile_hover.png);border:none;}"
                              "QPushButton:pressed{background-image: url(:/bt/image/choosefile_hover.png);border:none;}");

    QString filename = REFERENCEFILE;

    std::cout << "choose file name :" << filename.toStdString() << std::endl;
    RefImage = imread(filename.toStdString());

    QPixmap pixmap = QPixmap::fromImage(MatToQImage(RefImage));
    int width = ui->RTemplateLable->width();
    int height = ui->RTemplateLable->height();
    QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->ui->RTemplateLable->setPixmap(fitpixmap);
}

Industry::~Industry()
{
    delete ui;
}

void Industry::on_pushButton_2_clicked()
{
    QString filename;
    QImage img;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择图像"),
        "/root/OneVino/",
        tr("*.png *.bmp *.jpg")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        std::cout << "choose file name :" << filename.toStdString() << std::endl;

        InImage = imread(filename.toStdString());
/*
        QPixmap pixmap = QPixmap::fromImage(MatToQImage(InImage));
        int width = ui->ToBeIdentified->width();
        int height = ui->ToBeIdentified->height();
        QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        this->ui->ToBeIdentified->setPixmap(fitpixmap);*/
        cv::Mat gray_input;
        cv::Mat gray_reference;

        if(InImage.empty())
        {
            QMessageBox::warning(this, "错误", "请先选择需要识别的图像",QMessageBox::Yes, QMessageBox::Yes);
            return;
        }

        if(RefImage.empty())
        {
            QMessageBox::warning(this, "错误", "请先选择参考图像",QMessageBox::Yes, QMessageBox::Yes);
            return;
        }

        cv::cvtColor(InImage, gray_input, COLOR_RGB2GRAY);
        cv::cvtColor(RefImage, gray_reference, COLOR_RGB2GRAY);

        auto threshold = 160;
        cv::threshold(gray_input, gray_input, threshold, 255, cv::THRESH_BINARY);
        cv::threshold(gray_reference, gray_reference, threshold, 255, cv::THRESH_BINARY);

        auto strel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::morphologyEx(gray_input, gray_input, cv::MORPH_GRADIENT, strel);
        cv::morphologyEx(gray_reference, gray_reference, cv::MORPH_GRADIENT, strel);

        cv::Mat xor_result;
        cv::bitwise_xor(gray_input, gray_reference, xor_result);

        cv::medianBlur(xor_result, xor_result, 5);

        if (cv::countNonZero(xor_result) > 1)
        {
            auto output = InImage.clone();
            cv::Mat defect_mask = 255 - xor_result;
            cv::cvtColor(defect_mask, defect_mask, COLOR_GRAY2RGB);
            cv::bitwise_and(output, defect_mask, output);

            auto color_xor = cv::Mat();
            auto strel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
            cv::morphologyEx(xor_result, color_xor, cv::MORPH_OPEN, strel);

            cv::cvtColor(color_xor, color_xor, COLOR_GRAY2RGB);

            for (auto pixel = color_xor.begin<cv::Vec3b>(); pixel != color_xor.end<cv::Vec3b>(); pixel++)
            {
                if ((*pixel)[0] > 0
                    || (*pixel)[1] > 0
                    || (*pixel)[2] > 0)
                {
                    *pixel = cv::Vec3b(0, 0, 255);
                }
            }
            cv::addWeighted(output, 0.4, color_xor, 1.0, -1, output);

            //cv::imshow("out!", color_xor);
            ResultImage = output.clone();
/*
            IndustryResult dialog;
            dialog.setWindowModality(Qt::ApplicationModal);
            if(dialog.exec() != IndustryResult::Accepted)//如果是取消就返回
            {
                return;
            }*/
            QPixmap pixmap = QPixmap::fromImage(MatToQImage(ResultImage));
            int width = ui->ToBeIdentified->width();
            int height = ui->ToBeIdentified->height();
            QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            this->ui->ToBeIdentified->setPixmap(fitpixmap);
        }
        else {
            QMessageBox::warning(this, "提示", "未检测到异常",QMessageBox::Yes, QMessageBox::Yes);
        }
    }
}
/*
void Industry::on_pushButton_4_clicked()
{
    cv::Mat gray_input;
    cv::Mat gray_reference;

    if(InImage.empty())
    {
        QMessageBox::warning(this, "错误", "请先选择需要识别的图像",QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(RefImage.empty())
    {
        QMessageBox::warning(this, "错误", "请先选择参考图像",QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    cv::cvtColor(InImage, gray_input, COLOR_RGB2GRAY);
    cv::cvtColor(RefImage, gray_reference, COLOR_RGB2GRAY);

    auto threshold = 160;
    cv::threshold(gray_input, gray_input, threshold, 255, cv::THRESH_BINARY);
    cv::threshold(gray_reference, gray_reference, threshold, 255, cv::THRESH_BINARY);

    auto strel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(gray_input, gray_input, cv::MORPH_GRADIENT, strel);
    cv::morphologyEx(gray_reference, gray_reference, cv::MORPH_GRADIENT, strel);

    cv::Mat xor_result;
    cv::bitwise_xor(gray_input, gray_reference, xor_result);

    cv::medianBlur(xor_result, xor_result, 5);

    if (cv::countNonZero(xor_result) > 1)
    {
        auto output = InImage.clone();
        cv::Mat defect_mask = 255 - xor_result;
        cv::cvtColor(defect_mask, defect_mask, COLOR_GRAY2RGB);
        cv::bitwise_and(output, defect_mask, output);

        auto color_xor = cv::Mat();
        auto strel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
        cv::morphologyEx(xor_result, color_xor, cv::MORPH_OPEN, strel);

        cv::cvtColor(color_xor, color_xor, COLOR_GRAY2RGB);

        for (auto pixel = color_xor.begin<cv::Vec3b>(); pixel != color_xor.end<cv::Vec3b>(); pixel++)
        {
            if ((*pixel)[0] > 0
                || (*pixel)[1] > 0
                || (*pixel)[2] > 0)
            {
                *pixel = cv::Vec3b(0, 0, 255);
            }
        }
        cv::addWeighted(output, 0.4, color_xor, 1.0, -1, output);

        //cv::imshow("out!", color_xor);
        ResultImage = output.clone();

        IndustryResult dialog;
        dialog.setWindowModality(Qt::ApplicationModal);
        if(dialog.exec() != IndustryResult::Accepted)//如果是取消就返回
        {
            return;
        }
    }
    else {
        QMessageBox::warning(this, "提示", "未检测到异常",QMessageBox::Yes, QMessageBox::Yes);
    }
}
*/
