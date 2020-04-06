#ifndef NETWORKSELECTION_H
#define NETWORKSELECTION_H

#include "mainwindow.h"
#include <QWizardPage>

class NetworkInterfaceRadioButton : public QRadioButton//派生的按键类，存储已经找到的网络
{
    QNetworkAddressEntry m_address;
    QNetworkInterface m_ni;
public:
    explicit NetworkInterfaceRadioButton(const QNetworkAddressEntry &address, const QNetworkInterface &ni)
        : QRadioButton(), m_address(address), m_ni(ni)
    {
        setText(QString("%0 %1").arg(address.ip().toString()).arg(ni.name()));
    }
    quint32 ip() const//const对象只能调用const成员函数且不能在const成员函数中修改值
    {
        return m_address.ip().toIPv4Address();
    }
    quint32 netmask() const
    {
        return m_address.netmask().toIPv4Address();
    }
    quint32 broadcast() const
    {
        return m_address.broadcast().toIPv4Address();
    }
    int index() const
    {
        return m_ni.index();
    }

};

class NetworkPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit NetworkPage(QNetworkAddressEntry* controller,QWidget *parent=Q_NULLPTR);
    ~NetworkPage();
    //选择网络的部分
    QList<NetworkInterfaceRadioButton *> interfaces;
    QFormLayout* networkchoose;

    void clear_page();//清除页面
    void fresh_page();//重新扫描并现实网络
private slots:
    void on_network_fresh_clicked();

private:
    QNetworkAddressEntry* controller;
    void initializePage();//QWizardPage的虚函数，初始化页面
    bool validatePage();//QWizardPage的虚函数，页面下一步时执行的动作


};

#endif // NETWORKSELECTION_H
