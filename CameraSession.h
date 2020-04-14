#ifndef CAMERASESSION_H
#define CAMERASESSION_H

#include "mainwindow.h"
#include "gvsp/gvspreceiver.h"
#include "gev/geviport.h"
#include "gev/gevmonitor.h"
#include <qsharedpointer.h>


class CameraSession
{
//       Q_OBJECT
public:
    explicit CameraSession();
    ~CameraSession();
    void initSession(const QHostAddress &controller, const QHostAddress &transmitter,const QString save_path,const QString file_type);

    void Session_stop();
    void Session_start(const QHostAddress &controller);
    void Set_path(const QString save_path,const QString file_type);

private:
    GvspReceiver *gvsp;
    GevIPort iport;


};


#endif // CAMERASESSION_H
