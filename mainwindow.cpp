#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "CameraSession.h"
#include <QString>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->form_box->addItem("jpg");
    ui->form_box->addItem("png");
    ui->form_box->addItem("bmp");
    ui->form_box->addItem("ppm");
    ui->form_box->addItem("tif");
    ui->form_box->addItem("jp2");
    ui->start->setEnabled(false);
    ui->stop->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_start_clicked()
{
//    QWizard wizard;
    if(linked&&!session)
    {
        session = new CameraSession;
        session->initSession(controller->ip(), QHostAddress(DiscoveryAckHelper::currentIP(*(transmitter))),ui->path_box->currentText(),ui->form_box->currentText());
    }
    else if(linked)
    {
        session->Set_path(ui->path_box->currentText(),ui->form_box->currentText());
        session->Session_start(controller->ip());
    }
}

void MainWindow::on_toolButton_clicked()
{
    QString save_directory = QFileDialog::getExistingDirectory(this, tr("Find Directory"), "/home/ash-1/qt_pj/data_save");//QDir::currentPath()
    if(path_store.find(save_directory)==path_store.end())
    {
        ui->path_box->insertItem(0,save_directory);
        ui->path_box->setCurrentIndex(0);
        path_store.insert(save_directory);
    }
    if(linked&&ui->path_box->count())
    {
        ui->start->setEnabled(true);
        ui->stop->setEnabled(true);
    }
}

void MainWindow::on_stop_clicked()
{
    if(linked&&session)
    {
        session->Session_stop();
    }
}

void MainWindow::on_create_clicked()
{
    discoverer = new GvcpDiscoverer;
    controller = new QNetworkAddressEntry;
    transmitter = new DISCOVERY_ACK;
//    receiver = new DISCOVERY_ACK;
    wizard.setWindowTitle(trUtf8("配置GiGE Vision摄像机"));
    wizard.addPage(new NetworkPage(controller));
    wizard.addPage(new DiscoveryPage(discoverer,controller,transmitter));
    wizard.addPage(new ForceIPPage(discoverer,controller,transmitter));
//    wizard.addPage(new GvspPage(discoverer,controller,transmitter,receiver));
    if( wizard.exec() == QDialog::Accepted)
    {
         linked = true;
         if(ui->path_box->count())
         {
             ui->start->setEnabled(true);
             ui->stop->setEnabled(true);
         }
    }
}
