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
    ui->path_box->addItem("/home/ash-1/qt_pj/data_save");
    ui->name->setAlignment(Qt::AlignHCenter);
    ui->type->setAlignment(Qt::AlignHCenter);
    ui->IP->setAlignment(Qt::AlignHCenter);
    ui->mask->setAlignment(Qt::AlignHCenter);
    ui->gate->setAlignment(Qt::AlignHCenter);
    ui->mac->setAlignment(Qt::AlignHCenter);
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
        session = new CameraSession(this->ui);
        session->initSession(controller->ip(), QHostAddress(DiscoveryAckHelper::currentIP(*(transmitter))),ui->path_box->currentText(),ui->form_box->currentText());
    }
    else if(linked)
    {
        session->Set_path(ui->path_box->currentText(),ui->form_box->currentText());
        session->Session_start(controller->ip(),QHostAddress(DiscoveryAckHelper::currentIP(*(transmitter))));
    }
    ui->start->setEnabled(false);
    ui->stop->setEnabled(true);
    QString for_tip = "正在传输,已传输"+QString::number(session->get_packet_num())+"张图片";
    ui->tips->setText(for_tip);
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
        ui->tips->setText("点击开始按键开始传输");
    }
}

void MainWindow::on_stop_clicked()
{
    if(linked&&session)
    {
        session->Session_stop();
    }
    ui->stop->setEnabled(false);
    ui->start->setEnabled(true);
    QString for_tip = "传输暂停,已传输"+QString::number(session->get_packet_num())+"张图片";
    ui->tips->setText(for_tip);
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
             ui->tips->setText("点击开始按键开始传输");
         }
         else
             ui->tips->setText("请设置存储路径和保存格式");
         quint16 macH = DiscoveryAckHelper::deviceMACaddressHigh(*transmitter);
         quint32 macL = DiscoveryAckHelper::deviceMACaddressLow(*transmitter);
         ui->name->setText(DiscoveryAckHelper::userDefinedName(*transmitter));
         ui->IP->setText(QHostAddress(DiscoveryAckHelper::currentIP(*transmitter)).toString());
         ui->gate->setText(QHostAddress(DiscoveryAckHelper::defaultGateway(*transmitter)).toString());
         ui->mask->setText(QHostAddress(DiscoveryAckHelper::currentSubnetMask(*transmitter)).toString());
         ui->type->setText(DiscoveryAckHelper::modelName(*transmitter));
         ui->mac->setText(QString("%0:%1:%2:%3:%4:%5")
                          .arg((macH >> 8) & 0xFF,2,16, QLatin1Char('0'))
                          .arg((macH >> 0) & 0xFF,2,16, QLatin1Char('0'))
                          .arg((macL >> 24) & 0xFF,2,16, QLatin1Char('0'))
                          .arg((macL >> 16) & 0xFF,2,16, QLatin1Char('0'))
                          .arg((macL >> 8) & 0xFF,2,16, QLatin1Char('0'))
                          .arg((macL >> 0) & 0xFF,2,16, QLatin1Char('0')).toUpper());

    }
}
