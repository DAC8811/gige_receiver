#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkAddressEntry>
#include <qformlayout.h>
#include <QRadioButton>
#include "NetworkPage.h"
#include "DiscoveryPage.h"
#include "ForceIPPage.h"
#include "GvspPage.h"
#include "CameraSession.h"
#include <QWizard>
#include <set>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CameraSession;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_clicked();

    void on_toolButton_clicked();

    void on_stop_clicked();

    void on_create_clicked();

private:
    Ui::MainWindow *ui;
    std::set<QString> path_store;
    QWizard wizard;
    GvcpDiscoverer* discoverer;
    QNetworkAddressEntry* controller;
    DISCOVERY_ACK* transmitter;
    DISCOVERY_ACK* receiver;
    CameraSession* session;

    bool linked = false;
};


#endif // MAINWINDOW_H
