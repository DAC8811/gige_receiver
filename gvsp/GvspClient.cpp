#include "GvspClient.h"

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
}
void GvspClient::trash(GvspImage &image)
{
    delete [] image.datas.p;
}
