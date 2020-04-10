#include "CameraSession.h"
#include "gvsp/GvspClient.h"
#include "gvsp/gvspreceiver.h"
#include "gev/geviport.h"
#include <QDebug>



CameraSession::CameraSession()
{
}

CameraSession::~CameraSession()
{
}

void CameraSession::initSession(const QHostAddress &controller, const QHostAddress &transmitter)
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
    quint16 receiverPort = this->gvsp->getFreePort(controller);
    this->gvsp->listen(controller, receiverPort, transmitter);

    QSharedPointer<GevIPort> iport(new GevIPort);
    iport->connectAsController(controller, transmitter);
    iport->addReceiver(0, controller, receiverPort);
}
