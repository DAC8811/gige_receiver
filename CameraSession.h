#ifndef CAMERASESSION_H
#define CAMERASESSION_H

#include "mainwindow.h"
#include "gvsp/gvspreceiver.h"
#include "gev/gevmonitor.h"
#include <qsharedpointer.h>


class CameraSession
{
//       Q_OBJECT
public:
    explicit CameraSession();
    ~CameraSession();
    void initSession(const QHostAddress &controller, const QHostAddress &transmitter);

private:
    GvspReceiver *gvsp;
};


#endif // CAMERASESSION_H
