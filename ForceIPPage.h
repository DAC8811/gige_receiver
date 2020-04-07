#ifndef FORCEIPPAGE_H
#define FORCEIPPAGE_H

#include "mainwindow.h"
#include "gvcp/discoveryhelper.h"
#include "gvcp/gvcpdiscoverer.h"
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>


class ForceIPPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ForceIPPage(GvcpDiscoverer* discoverer, QNetworkAddressEntry* controller, DISCOVERY_ACK* transmitter,
                      QWidget *parent = 0);
    static QString ackTohtml(const DISCOVERY_ACK &ack);

    QNetworkAddressEntry* controller;
    GvcpDiscoverer* discoverer;
    DISCOVERY_ACK* transmitter;

private slots:
    void applyChanges();
    void forceIPToggled(bool checked);
    void onForceIPSucces();
    void onDeviceAdded(const DISCOVERY_ACK &ack, int id);

private:
    int forceipID;
    QLabel *controllerIp;
    QLabel *transmeterInfos;
    QCheckBox *useForceIP;
    QLineEdit *newIP;
    QLineEdit *newMask;
    QLineEdit *newGateway;
    QPushButton *apply;
    void initializePage();
    bool validatePage();
};
#endif // FORCEIPPAGE_H
