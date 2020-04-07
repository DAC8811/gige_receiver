#ifndef GVSPPAGE_H
#define GVSPPAGE_H

#include "mainwindow.h"
#include <QTextEdit>
#include <QComboBox>

class GvspPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit GvspPage(GvcpDiscoverer* discoverer, QNetworkAddressEntry* controller, DISCOVERY_ACK* transmitter, DISCOVERY_ACK* receiver,
                      QWidget *parent = 0);
    virtual ~GvspPage();
    QNetworkAddressEntry* controller;
    GvcpDiscoverer* discoverer;
    DISCOVERY_ACK* transmitter;
    DISCOVERY_ACK* receiver;


private slots:
    void discover();
    void receiversIndexChanged(int index);

private:
    QRadioButton *localRadio;
    QRadioButton *distRadio;
    QLineEdit *socketInfo;
    QPushButton *discover_bt;
    QComboBox *receivers;
    QTextEdit *receiverDescription;
    void initializePage();
    bool validatePage();

};









#endif // GVSPPAGE_H
