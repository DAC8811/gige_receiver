#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_start_clicked()
{
    QWizard wizard;
    GvcpDiscoverer* discoverer = new GvcpDiscoverer;
    QNetworkAddressEntry* controller = new QNetworkAddressEntry;
    DISCOVERY_ACK* transmitter = new DISCOVERY_ACK;
    DISCOVERY_ACK* receiver;
    wizard.setWindowTitle(trUtf8("配置GiGE Vision摄像机"));
    wizard.addPage(new NetworkPage(controller));
    wizard.addPage(new DiscoveryPage(discoverer,controller,transmitter));
    wizard.exec();
}
