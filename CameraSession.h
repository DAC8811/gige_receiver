#ifndef CAMERASESSION_H
#define CAMERASESSION_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gvsp/gvspreceiver.h"
#include "gev/geviport.h"
#include "gev/gevmonitor.h"
#include <qsharedpointer.h>
class GvspReceiver;
namespace Ui { class MainWindow; }


class CameraSession
{
//       Q_OBJECT
public:
    explicit CameraSession(Ui::MainWindow *ui=nullptr);
    ~CameraSession();
    void initSession(const QHostAddress &controller, const QHostAddress &transmitter,const QString save_path,const QString file_type);

    void Session_stop();
    void Session_start(const QHostAddress &controller,const QHostAddress &transmitter);
    void Set_path(const QString save_path,const QString file_type);

    uint get_packet_num();

private:
    GvspReceiver *gvsp;
    GevIPort *iport;

    Ui::MainWindow *ui;

public:
    uint PacketSize = 2000;
    uint PacketDelay = 300;


};


#endif // CAMERASESSION_H
