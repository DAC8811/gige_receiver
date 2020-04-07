#ifndef DISCOVERYPAGE_H
#define DISCOVERYPAGE_H

#include "mainwindow.h"
#include <QWizardPage>
#include "gvcp/gvcpdiscoverer.h"
#include "gvcp/discoveryhelper.h"
#include "gvcp/bootstrapregisters.h"
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>


class DiscoveryPage : public QWizardPage
{
     Q_OBJECT
public:
    explicit DiscoveryPage(GvcpDiscoverer* discoverer, QNetworkAddressEntry* controller, DISCOVERY_ACK* transmitter, QWidget *parent=Q_NULLPTR);
    ~DiscoveryPage();
    static QString ackTohtml(const DISCOVERY_ACK &ack);

    QNetworkAddressEntry* controller;
    GvcpDiscoverer* discoverer;
    DISCOVERY_ACK* transmitter;

private slots:
    void discover();
    void devicesListIndexChanged(int index);
    void onDeviceAdded(const DISCOVERY_ACK &ack, int discoverID);

private:
    int discoverID;
    QLabel *controllerIp;
    QLabel *transmeterInfos;
    QPushButton *discover_bt;
    QCheckBox *allNetworks;
    QHostAddress diffusion;
    QComboBox *devicesList;
    void initializePage();
    bool validatePage();
};





#endif // DISCOVERYPAGE_H
