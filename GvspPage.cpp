#include "GvspPage.h"

GvspPage::GvspPage(GvcpDiscoverer* discoverer, QNetworkAddressEntry* controller, DISCOVERY_ACK* transmitter, DISCOVERY_ACK* receiver,
                   QWidget *parent)
    : QWizardPage(parent),discoverer(discoverer), controller(controller), transmitter(transmitter), receiver(receiver)
{
    setTitle(trUtf8("GiGE Vision STREAM"));
    setSubTitle(trUtf8("视频流客户端可能是本地的（在此计算机上）"
                       "或远程（另一台机器）"));
    this->localRadio = new QRadioButton(trUtf8("本机"));
    this->distRadio = new QRadioButton(trUtf8("远程"));
    this->localRadio->setChecked(true);

    this->socketInfo = new QLineEdit;
    this->socketInfo->setReadOnly(true);

    this->discover_bt = new QPushButton(trUtf8("Start"));
    this->receivers = new QComboBox;
    this->receiverDescription = new QTextEdit;
    this->receiverDescription->setReadOnly(true);
    this->receiverDescription->setMinimumHeight(100);

    QFormLayout *mainLayout = new QFormLayout;
    mainLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    mainLayout->addRow(trUtf8("视频流接收端选择"), this->localRadio);
    mainLayout->addWidget(this->distRadio);
    mainLayout->addRow(trUtf8("Information socket GVSP"), this->socketInfo);
//    mainLayout->addRow(new LineSeparator);
    mainLayout->addRow(trUtf8("开始搜索"), this->discover_bt);
    mainLayout->addRow(trUtf8("接收端"), this->receivers);
    mainLayout->addRow(trUtf8("描述"), this->receiverDescription);
    setLayout(mainLayout);

    connect(this->discover_bt, SIGNAL(clicked()),
            this, SLOT(discover()));
//    connect(&transmitter, SIGNAL(GevReceiverFound(const GvcpPeers &)),
//            this, SLOT(deviceDiscovered(const GvcpPeers &)));
    connect(this->receivers, SIGNAL(currentIndexChanged(int)),
            this, SLOT(receiversIndexChanged(int)));

}

GvspPage::~GvspPage()
{}

void GvspPage::initializePage()
{
    // à l'initialisation, GVSP est local----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//    d->gevDescription.gvsp.clientIP = d->gevDescription.gvcp.clientIP;
//    d->gevDescription.gvsp.clientPort = d->gevDevice.getFreeGvspPort();
//    d->gevDescription.gvsp.cameraIP = d->gevDescription.gvcp.deviceIP;

//    d->socketInfo->clear();
//    d->socketInfo->setText(trUtf8("IP: %0 port: %1").arg(QHostAddress(d->gevDescription.gvsp.clientIP).toString()).arg(d->gevDescription.gvsp.clientPort));
   //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
}


void GvspPage::discover()
{
    this->receivers->setEnabled(false);
    this->receivers->clear();
    this->receiverDescription->clear();
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//    d->gevDevice.discoverDevices(
//                QHostAddress(d->gevDescription.gvcp.clientIP),
//                QHostAddress(QHostAddress::Broadcast),
//                false);
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
}

void GvspPage::receiversIndexChanged(int index)
{
    if (index >= 0) {
        //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //    GvcpPeers selected = d->receivers->itemData(index).value<GvcpPeers>();

    //    d->gevDescription.gvcp.GiGEVersion = selected.GiGEVersion;
    //    d->gevDescription.gvcp.deviceIP = selected.deviceIP;
    //    d->gevDescription.gvcp.deviceMask = selected.deviceMask;
    //    d->gevDescription.gvcp.deviceGateway = selected.deviceGateway;
    //    d->gevDescription.gvcp.deviceMAC[0] = selected.deviceMAC[0];
    //    d->gevDescription.gvcp.deviceMAC[1] = selected.deviceMAC[1];
    //    d->gevDescription.gvcp.deviceMAC[2] = selected.deviceMAC[2];
    //    d->gevDescription.gvcp.deviceMAC[3] = selected.deviceMAC[3];
    //    d->gevDescription.gvcp.deviceMAC[4] = selected.deviceMAC[4];
    //    d->gevDescription.gvcp.deviceMAC[5] = selected.deviceMAC[5];
    //    d->gevDescription.gvcp.deviceName = selected.deviceName;
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        this->receiverDescription->clear();
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //    d->receiverDescription->setDocumentTitle(d->gevDescription.gvcp.deviceName);
    //    d->receiverDescription->append(trUtf8("Version GiGE Vision : <b>%0</b>").arg(d->gevDescription.gvcp.GiGEVersion));
    //    d->receiverDescription->append(trUtf8("Adresse IP : <b>%0</b>").arg(QHostAddress(d->gevDescription.gvcp.deviceIP).toString()));
    //    d->receiverDescription->append(trUtf8("Masque sous-réseau : <b>%0</b>").arg(QHostAddress(d->gevDescription.gvcp.deviceMask).toString()));
    //    d->receiverDescription->append(trUtf8("Passerelle : <b>%0</b>").arg(QHostAddress(d->gevDescription.gvcp.deviceGateway).toString()));
    //    d->receiverDescription->append(trUtf8("Adresse MAC : <b>%0:%1:%2:%3:%4:%5</b>")
    //                                 .arg(d->gevDescription.gvcp.deviceMAC[0],2,16, QLatin1Char('0'))
    //            .arg(d->gevDescription.gvcp.deviceMAC[1],2,16, QLatin1Char('0'))
    //            .arg(d->gevDescription.gvcp.deviceMAC[2],2,16, QLatin1Char('0'))
    //            .arg(d->gevDescription.gvcp.deviceMAC[3],2,16, QLatin1Char('0'))
    //            .arg(d->gevDescription.gvcp.deviceMAC[4],2,16, QLatin1Char('0'))
    //            .arg(d->gevDescription.gvcp.deviceMAC[5],2,16, QLatin1Char('0')));
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    }
}

bool GvspPage::validatePage()
{
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//    if (d->withoutDiscovery->isChecked()) {
//        QHostAddress address(d->diffusionIP->text());
//        if (!address.isNull()) {
//            d->gevDescription.gvcp.deviceIP = address.toIPv4Address();
//        } else {
//            d->gevDescription.gvcp.deviceIP = 0;
//        }

//    }
//    return (d->gevDescription.gvcp.deviceIP != 0);
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    return true;
}

