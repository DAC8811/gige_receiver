#include "ForceIPPage.h"





ForceIPPage::ForceIPPage(GvcpDiscoverer* discoverer, QNetworkAddressEntry* controller, DISCOVERY_ACK* transmitter,
                         QWidget *parent)
    :QWizardPage(parent),discoverer(discoverer),controller(controller),transmitter(transmitter)
{
    setTitle(trUtf8("FORCEIP"));
    setSubTitle(trUtf8("FORCEIP允许您为相机分配IP "
                       "它应该用来与您交流。"));//FORCEIP va vous permettre d'indiquer à la caméra quelle IP " "elle doit utiliser pour communiquer avec vous.



    this->controllerIp = new QLabel;
    this->controllerIp->setAlignment(Qt::AlignCenter);
    this->controllerIp->setStyleSheet("background-color: white");
    this->transmeterInfos = new QLabel;
    this->transmeterInfos->setStyleSheet("background-color: white");
    QRegExp rx( "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3}" );
    this->useForceIP = new QCheckBox;
    this->useForceIP->setChecked(false);
    this->newIP =  new QLineEdit;
    this->newIP->setAlignment(Qt::AlignCenter);
    this->newIP->setValidator(new QRegExpValidator(rx));
    this->newMask =  new QLineEdit;
    this->newMask->setAlignment(Qt::AlignCenter);
    this->newMask->setValidator(new QRegExpValidator(rx));
    this->newGateway =  new QLineEdit;
    this->newGateway->setAlignment(Qt::AlignCenter);
    this->newGateway->setValidator(new QRegExpValidator(rx));
    this->apply = new QPushButton(trUtf8("应用"));//Appliquer
    forceIPToggled(false);


    QFormLayout *formLayout = new QFormLayout;
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->addRow(trUtf8("控制端"), this->controllerIp);
//    formLayout->addRow(new LineSeparator);
    formLayout->addRow(trUtf8("发送端（相机）"), this->transmeterInfos);
//    formLayout->addRow(new LineSeparator);
    formLayout->addRow(trUtf8("使用 FORCEIP"), this->useForceIP);//Utiliser FORCEIP
    formLayout->addRow(trUtf8("相机的新IP"), this->newIP);//Nouvelle IP caméra
    formLayout->addRow(trUtf8("相机的新掩码"), this->newMask);//Nouveau masque caméra
    formLayout->addRow(trUtf8("相机的新网关"), this->newGateway);//Nouvelle passerelle caméra
    formLayout->addRow(trUtf8("应用变更"), this->apply);//Appliquer les changements

    setLayout(formLayout);

    connect(this->apply, SIGNAL(clicked()),
            this, SLOT(applyChanges()));
    connect(this->discoverer, SIGNAL(forceIPsucces()),
            this, SLOT(onForceIPSucces()));
    connect(this->discoverer, SIGNAL(newDeviceDiscovered(DISCOVERY_ACK,int)),
            this, SLOT(onDeviceAdded(DISCOVERY_ACK, int)));
    connect(this->useForceIP, SIGNAL(toggled(bool)),
            this, SLOT(forceIPToggled(bool)));

}

void ForceIPPage::applyChanges()
{
    quint64 mac = static_cast<quint64>(DiscoveryAckHelper::deviceMACaddressHigh(*(this->transmitter))) << 32;
    mac |= static_cast<quint64>(DiscoveryAckHelper::deviceMACaddressLow(*(this->transmitter)));
    this->discoverer->forceIP(this->controller->ip(),
                          mac,
                          QHostAddress(this->newIP->text()).toIPv4Address(),
                          QHostAddress(this->newMask->text()).toIPv4Address(),
                          QHostAddress(this->newGateway->text()).toIPv4Address());
}

void ForceIPPage::forceIPToggled(bool checked)
{
    if (checked) {
        this->newIP->setText(QHostAddress(this->controller->ip().toIPv4Address() & this->controller->netmask().toIPv4Address()).toString());
        this->newMask->setText(this->controller->netmask().toString());
        this->newGateway->setText("0.0.0.0");
    } else {
        this->newIP->clear();
        this->newMask->clear();
        this->newGateway->clear();
    }
    this->newIP->setEnabled(checked);
    this->newMask->setEnabled(checked);
    this->newGateway->setEnabled(checked);
    this->apply->setEnabled(checked);
}

void ForceIPPage::onForceIPSucces()
{
    // redemande l'ack discovery
    this->forceipID = this->discoverer->sendDiscovery(this->controller->ip(), QHostAddress(this->newIP->text()));
}

void ForceIPPage::onDeviceAdded(const DISCOVERY_ACK &ack, int id)
{
    if (id == this->forceipID) {
        std::memcpy(this->transmitter, &ack, sizeof(DISCOVERY_ACK));
        initializePage();
    }
}

void ForceIPPage::initializePage()
{
    this->controllerIp->setText(QString("%0 / %1")
                             .arg(this->controller->ip().toString())
                             .arg(this->controller->netmask().toString()));
    this->transmeterInfos->setText(ackTohtml(*(this->transmitter)));
    this->useForceIP->blockSignals(true);
    this->useForceIP->setChecked(false);
    forceIPToggled(false);
    this->useForceIP->blockSignals(false);
    this->forceipID = -1;
}

bool ForceIPPage::validatePage()
{
    QHostAddress deviceIP(DiscoveryAckHelper::currentIP(*(this->transmitter)));
    return deviceIP.isInSubnet(this->controller->ip(), this->controller->prefixLength());
}

static QString rowtab(const QString &first, const QString &second) {
    return QString("<tr>"
                   "<th><span style=\" font-style:italic; font-weight:100;\">%0<span></th>"
                   "<th><span style=\" font-style:normal; font-weight:500;\">%1<span></th>"
                   "</tr>").arg(first).arg(second);
}

QString ForceIPPage::ackTohtml(const DISCOVERY_ACK &ack)
{
    quint16 macH = DiscoveryAckHelper::deviceMACaddressHigh(ack);
    quint32 macL = DiscoveryAckHelper::deviceMACaddressLow(ack);
    return QString("<table border=\"1\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" width=\"100%\">")
            + rowtab(trUtf8("型号"), DiscoveryAckHelper::modelName(ack))
            + rowtab(trUtf8("制造商"), DiscoveryAckHelper::manufacturerName(ack))
            + rowtab(trUtf8("version"), DiscoveryAckHelper::deviceVersion(ack))
            + rowtab(trUtf8("序列号"), DiscoveryAckHelper::serialNumber(ack))
            + rowtab(trUtf8("名字"), DiscoveryAckHelper::userDefinedName(ack))
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
