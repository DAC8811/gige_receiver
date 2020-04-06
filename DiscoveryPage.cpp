#include "DiscoveryPage.h"



Q_DECLARE_METATYPE(DISCOVERY_ACK)

DiscoveryPage::DiscoveryPage(GvcpDiscoverer* discoverer, QNetworkAddressEntry* controller, DISCOVERY_ACK* transmitter, QWidget *parent)
    :QWizardPage(parent),discoverer(discoverer),controller(controller),transmitter(transmitter)
{
    setTitle(trUtf8("DISCOVERY"));
    setSubTitle(trUtf8("DISCOVERY允许自动检测所有"
                       "符合GiGE的<b>设备发现</ b>"));//DISCOVERY permet la détection automatique de tous les périphériques " "GiGE respectant le chapitre <b>Device discovery</b>"
    this->controllerIp = new QLabel;
    this->controllerIp->setAlignment(Qt::AlignCenter);
    this->controllerIp->setStyleSheet("background-color: white");
    this->transmeterInfos = new QLabel;
    this->transmeterInfos->setStyleSheet("background-color: white");
    this->discover_bt = new QPushButton(trUtf8("Start"));
    this->allNetworks = new QCheckBox(trUtf8("所有网络"));//Tous les réseaux


    //    QRegExp rx( "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3}" );


    this->devicesList = new QComboBox;
    this->devicesList->setEnabled(false);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->addRow(trUtf8("控制端"), this->controllerIp);//Contrôleur
//    formLayout->addRow(new LineSeparator);
    formLayout->addRow(trUtf8("发射端（相机）"), this->transmeterInfos);//Transmetteur (caméra)
//    formLayout->addRow(new LineSeparator);
    QHBoxLayout *dl = new QHBoxLayout;
    dl->addWidget(this->discover_bt,1);
    dl->addWidget(this->allNetworks);
    formLayout->addRow(trUtf8("已发现清单"), this->devicesList);//Liste des découverts
    formLayout->addRow(trUtf8("搜索"), dl);//Découvrir


    setLayout(formLayout);

    connect(this->discover_bt, SIGNAL(clicked()),
            this, SLOT(discover()));
    connect(this->discoverer, SIGNAL(newDeviceDiscovered(DISCOVERY_ACK,int)),
            this, SLOT(onDeviceAdded(DISCOVERY_ACK,int)));
    connect(this->devicesList, SIGNAL(currentIndexChanged(int)),
            this, SLOT(devicesListIndexChanged(int)));

}

DiscoveryPage::~DiscoveryPage()
{}


void DiscoveryPage::discover()
{
    this->devicesList->setEnabled(false);
    this->devicesList->clear();
    this->discoverID = this->discoverer->sendDiscovery(
                QHostAddress(this->controller->ip()),
                this->allNetworks->isChecked() ? QHostAddress::Broadcast : this->controller->broadcast(),
                this->allNetworks->isChecked());
}


void DiscoveryPage::devicesListIndexChanged(int index)
{
    if (index >= 0) {
        const DISCOVERY_ACK ack = this->devicesList->itemData(index).value<DISCOVERY_ACK>();
        std::memcpy(this->transmitter, &ack, sizeof(DISCOVERY_ACK));
        this->transmeterInfos->setText(ackTohtml(*(this->transmitter)));
    }
}



void DiscoveryPage::onDeviceAdded(const DISCOVERY_ACK &ack, int discoverID)
{
    if (discoverID == this->discoverID) {
        const quint32 deviceMode = DiscoveryAckHelper::deviceMode(ack);
        if (DiscoveryAckHelper::deviceClass(deviceMode) == DeviceClass::TRANSMITTER) {
            QVariant variant;
            variant.setValue(ack);
            this->devicesList->addItem(QByteArray(DiscoveryAckHelper::modelName(ack), MemoryBlocs::ModelNameSize), variant);
            this->devicesList->setEnabled(true);
        }
    }
}

void DiscoveryPage::initializePage()
{
    this->controllerIp->setText(QString("%0 / %1")
                             .arg(this->controller->ip().toString())
                             .arg(this->controller->netmask().toString()));
    this->devicesList->clear();
    std::memset(this->transmitter, 0, sizeof(DISCOVERY_ACK));
    this->transmeterInfos->setText(ackTohtml(*(this->transmitter)));
    this->allNetworks->setChecked(false);
}

bool DiscoveryPage::validatePage()
{


    this->discoverID = -1;
    return (this->devicesList->count() > 0);
}

static QString rowtab(const QString &first, const QString &second) {
    return QString("<tr>"
                   "<th><span style=\" font-style:italic; font-weight:100;\">%0<span></th>"
                   "<th><span style=\" font-style:normal; font-weight:500;\">%1<span></th>"
                   "</tr>").arg(first).arg(second);
}

QString DiscoveryPage::ackTohtml(const DISCOVERY_ACK &ack)
{
    quint16 macH = DiscoveryAckHelper::deviceMACaddressHigh(ack);
    quint32 macL = DiscoveryAckHelper::deviceMACaddressLow(ack);
    return QString("<table border=\"1\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" width=\"100%\">")
            + rowtab(trUtf8("型号"), DiscoveryAckHelper::modelName(ack))//modèle
            + rowtab(trUtf8("制造商"), DiscoveryAckHelper::manufacturerName(ack))//fabricant
            + rowtab(trUtf8("version"), DiscoveryAckHelper::deviceVersion(ack))
            + rowtab(trUtf8("序列号"), DiscoveryAckHelper::serialNumber(ack))//n° série
            + rowtab(trUtf8("名字"), DiscoveryAckHelper::userDefinedName(ack))//nom
            + rowtab(trUtf8("ip"), QHostAddress(DiscoveryAckHelper::currentIP(ack)).toString())
            + rowtab(trUtf8("netmask"), QHostAddress(DiscoveryAckHelper::currentSubnetMask(ack)).toString())
            + rowtab(trUtf8("gateway"), QHostAddress(DiscoveryAckHelper::defaultGateway(ack)).toString())
            + rowtab(trUtf8("mac"), QString("%0:%1:%2:%3:%4:%5")
                     .arg((macH >> 8) & 0xFF,2,16, QLatin1Char('0'))
                     .arg((macH >> 0) & 0xFF,2,16, QLatin1Char('0'))
                     .arg((macL >> 24) & 0xFF,2,16, QLatin1Char('0'))
                     .arg((macL >> 16) & 0xFF,2,16, QLatin1Char('0'))
                     .arg((macL >> 8) & 0xFF,2,16, QLatin1Char('0'))
                     .arg((macL >> 0) & 0xFF,2,16, QLatin1Char('0')).toUpper())
            + QString("</table>");
}
