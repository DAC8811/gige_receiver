#ifndef DISCOVERYPAGE_H
#define DISCOVERYPAGE_H

#include "mainwindow.h"
#include <QWizardPage>
#include "gvcp/gvcpdiscoverer.h"
#include "gvcp/discoveryhelper.h"
#include "gvcp/bootstrapregisters.h"

class DiscoveryPage : public public QWizardPage
{
public:
    explicit DiscoveryPage(QWidget *parent=Q_NULLPTR);
    ~DiscoveryPage();
private slots:
    void discover();
    void devicesListIndexChanged(int index);
    void onDeviceAdded(const DISCOVERY_ACK &ack, int discoverID);

private:
    void initializePage();
    bool validatePage();
};





#endif // DISCOVERYPAGE_H
