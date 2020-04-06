#include "NetworkPage.h"
#include <QPushButton>
#include <QDebug>



NetworkPage::NetworkPage(QNetworkAddressEntry* controller,QWidget *parent):QWizardPage(parent),controller(controller)
{
    setMinimumSize(500,400);
    QPushButton *get = new QPushButton(trUtf8("刷新"));

    this->networkchoose = new QFormLayout;
    this->networkchoose->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    this->networkchoose->addRow(trUtf8("更新界面"), get);
//    this->networkchoose->addRow(new LineSeparator);
    setLayout(this->networkchoose);
    connect(get, SIGNAL(clicked()),
            this, SLOT(on_network_fresh_clicked()));
}

NetworkPage::~NetworkPage()
{
}

void NetworkPage::on_network_fresh_clicked()
{
    clear_page();
    fresh_page();
}

void NetworkPage::clear_page()
{
//    Q_D(NetworkSelectionPage);
    for (QWidget *w:this->interfaces)
    {
        int row;
        QFormLayout::ItemRole role;
        this->networkchoose->getWidgetPosition(w, &row, &role);
        QLayoutItem *item = this->networkchoose->itemAt(row, role);
        this->networkchoose->removeItem(item);
        QLayoutItem *label = this->networkchoose->itemAt(row, QFormLayout::LabelRole);
        if (label != NULL) {
            this->networkchoose->removeItem(label);
            delete label->widget();
            delete label;
        }
    }
    qDeleteAll(this->interfaces);
    this->interfaces.clear();
    this->controller->setIp(QHostAddress(0u));
    this->controller->setNetmask(QHostAddress(0u));
    this->controller->setBroadcast(QHostAddress(0u));
}

void NetworkPage::fresh_page()
{
    for(QNetworkInterface ni:QNetworkInterface::allInterfaces())
    {
        for(QNetworkAddressEntry ae:ni.addressEntries())
        {
            if (ae.ip().protocol() == QAbstractSocket::IPv4Protocol&& ae.ip() != QHostAddress::LocalHost)
            {
                       NetworkInterfaceRadioButton *interface = new NetworkInterfaceRadioButton(ae, ni);
                       this->interfaces.append(interface);

                       if (this->interfaces.count() == 1) {
                           interface->setChecked(true);
                           this->networkchoose->addRow("可连接的网络", interface);
                       }
                       else {
                           this->networkchoose->addWidget(interface);
                       }
            }
        }
    }
}
void NetworkPage::initializePage()
{
    clear_page();
    fresh_page();
}

bool NetworkPage::validatePage()
{
//    Q_D(NetworkSelectionPage);
    for (NetworkInterfaceRadioButton *b:this->interfaces) {
        if (b->isChecked()) {
            this->controller->setIp(QHostAddress(b->ip()));
            this->controller->setNetmask(QHostAddress(b->netmask()));
            this->controller->setBroadcast(QHostAddress(b->broadcast()));
            return true;
        }
    }
    return false;
}


