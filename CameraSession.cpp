#include "CameraSession.h"
#include "gvsp/GvspClient.h"
#include "gvsp/gvspreceiver.h"
#include <iostream>

#include <QDebug>



CameraSession::CameraSession(Ui::MainWindow *ui)
{
    this->ui=ui;
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
    this->gvsp = new GvspReceiver(new GvspClient,0,this->ui);
    this->gvsp->Save_path = save_path;
    this->gvsp->File_type = file_type;
    quint16 receiverPort = this->gvsp->getFreePort(controller);
    this->gvsp->listen(controller, receiverPort, transmitter);

//    QSharedPointer<GevIPort> iport(new GevIPort);
    iport = new GevIPort;
    iport->connectAsController(controller, transmitter);
    iport->addReceiver(0, controller, receiverPort);
//    std::cout<<this->ui->packetsize->text().toStdString()<<std::endl;
//    std::cout<<this->ui->packetdelay->text().toStdString()<<std::endl;
    iport->SetPacketSize(0,this->ui->packetsize->text().toUInt());
    iport->SetPacketDelay(0,this->ui->packetdelay->text().toUInt());

}

void CameraSession::Session_stop()
{
   iport->stopReceiver(0);
   this->gvsp->_run=false;
//   delete iport;
//   iport = NULL;
}

void CameraSession::Session_start(const QHostAddress &controller,const QHostAddress &transmitter)
{
//    this->gvsp = new GvspReceiver(new GvspClient,0,this->ui);
//    this->gvsp->Save_path = save_path;
//    this->gvsp->File_type = file_type;
    this->gvsp->_run = true;
    quint16 receiverPort = this->gvsp->getFreePort(controller);
    this->gvsp->listen(controller, receiverPort, transmitter);
//    iport = new GevIPort;
//    iport->connectAsController(controller, transmitter);
    iport->addReceiver(0, controller, receiverPort);
    iport->SetPacketSize(0,this->ui->packetsize->text().toUInt());
    iport->SetPacketDelay(0,this->ui->packetdelay->text().toUInt());
}

void CameraSession::Set_path(const QString save_path,const QString file_type)
{
    this->gvsp->Save_path = save_path;
    this->gvsp->File_type = file_type;
}

uint CameraSession::get_packet_num()
{
    if(this->gvsp)
        return gvsp->packet_num;
    else
        return 0;
}
