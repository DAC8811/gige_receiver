#include "GvspFileSave.h"
#include <opencv2/opencv.hpp>
#include <QTime>
#include <iostream>


GvspFileSave::GvspFileSave(Ui::MainWindow* ui):ui(ui)
{
}

GvspFileSave::~GvspFileSave()
{
}

void GvspFileSave::Save()
{
    cv::Mat file(this->height, this->width, this->type, this->p);
    QString file_path = save_path+"/"+QString::number(num)+"."+file_type;
    cv::imwrite(file_path.toStdString(),file);
}

void GvspFileSave::Set(uint num,uint width,uint height,quint32 type,size_t size,uchar* p, const QString save_path, const QString file_type)
{
    this->num = num;
    this->width = width;
    this->height = height;
    this->type = type;
    this->p = p;
    this->save_path = save_path;
    this->file_type = file_type;
    this->size = size;
}

void GvspFileSave::run()
{
    cv::Mat file(this->height, this->width, this->type, this->p);
    QString file_path = save_path+"/"+QString::number(num)+"."+file_type;
    cv::imwrite(file_path.toStdString(),file);
//    if(start_time.isNull()||!start_time.isValid())
//        start_time = QTime::currentTime();
//    else
//    {
//        QTime current = QTime::currentTime();
//        int msec = current.msec() - start_time.msec();
//        start_time = current;
//        double speed = width*height*3/msec;
//        this->ui->speed_show->setText(QString::number(speed,10,3));
//        std::cout << std::to_string(msec) << std::endl;
//    }
    if(start_time.isNull())
        start_time.start();
    else
    {
        int msec = start_time.restart();
        double speed =(size)/(double)(msec*1000);
        this->ui->speed_show->setText(QString::number(speed,'f',3));
    }
}
