#include "GvspClient.h"
#include <opencv2/opencv.hpp>

GvspClient::GvspClient()
{
}
GvspClient::~GvspClient()
{
}
void GvspClient::allocate(GvspImage &image)
{
    image.datas.p = new quint8[image.datas.size];
}
void GvspClient::push(GvspImage &image)
{
    cv::Mat file(image.geometry.height, image.geometry.width, image.type, image.datas.p);
    cv::imwrite("/home/ash-1/qt_pj/data_save/test3.jpg",file);
    qWarning("ok");
}
void GvspClient::trash(GvspImage &image)
{
    delete [] image.datas.p;
}
