#ifndef GVSPCLIENT_H
#define GVSPCLIENT_H
#include "gvsp.h"
class GvspImage;
class GvspClient
{
public:
    GvspClient();
    ~GvspClient();
    void allocate(GvspImage &image);
    void push(GvspImage &image);
    void trash(GvspImage &image);
};
#endif // GVSPCLIENT_H
