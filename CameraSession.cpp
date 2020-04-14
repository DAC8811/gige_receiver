#include "CameraSession.h"
#include "gvsp/GvspClient.h"
#include "gvsp/gvspreceiver.h"

#include <QDebug>



CameraSession::CameraSession()
{
}

CameraSession::~CameraSession()
{
}

void CameraSession::initSession(const QHostAddress &controller, const QHostAddress &transmitter,const QString save_path,const QString file_type)
{
    GevMonitor monitor;
    if (!monitor.connectTo(controller, transmitter))
    {
        qWarning("Failed to connect transmitter %s", qPrintable(transmitter.toString()));
        return;
    }
    QByteArray firstURL = monitor.getFirstUrl();
    QString fileName = GevMonitor::filenameFromFirstURL(firstURL);
    QByteArray xml = monitor.getXml(monitor.getFirstUrl());
    this->gvsp = new GvspReceiver(new GvspClient);
    this->gvsp->Save_path = save_path;
    this->gvsp->File_type = file_type;
    quint16 receiverPort = this->gvsp->getFreePort(controller);
    this->gvsp->listen(controller, receiverPort, transmitter);

//    QSharedPointer<GevIPort> iport(new GevIPort);

    iport.connectAsController(controller, transmitter);
    iport.addReceiver(0, controller, receiverPort);
}

void CameraSession::Session_stop()
{
   iport.stopReceiver(0);
}

void CameraSession::Session_start(const QHostAddress &controller)
{
    quint16 receiverPort = this->gvsp->getFreePort(controller);
    iport.addReceiver(0, controller, receiverPort);
}

void CameraSession::Set_path(const QString save_path,const QString file_type)
{
    this->gvsp->Save_path = save_path;
    this->gvsp->File_type = file_type;
}
