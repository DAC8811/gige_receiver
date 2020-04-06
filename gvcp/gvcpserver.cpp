/***************************************************************************
 *   Copyright (C) 2014-2015 by Cyril BALETAUD                             *
 *   cyril.baletaud@gmail.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "gvcpserver.h"
#include "gvcpserver_p.h"


#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>

#include <QHostAddress>
#include <cstring>

GvcpServerPrivate::~GvcpServerPrivate()
{}

GvcpServer::GvcpServer(QObject *parent)
    : QObject(parent), d_ptr(new GvcpServerPrivate)
{}

GvcpServer::~GvcpServer()
{}

void GvcpServer::listen(const QNetworkInterface &netInterface)
{
    Q_D(GvcpServer);
    if (d->udp.bind(GVCP_PORT, QUdpSocket::ShareAddress)) {
        d->bootstrap.setNetworkInterface(netInterface);
        connect(&d->udp, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
        connect(&d->heartbeat, SIGNAL(timeout()), this, SLOT(onHeartbeat()));
        d->heartbeat.start(d->bootstrap.value(Register::HeartbeatTimeout));
        qDebug() << "start" << d->bootstrap.value(Register::HeartbeatTimeout);
    }
}

GvcpServer::GvcpServer(GvcpServerPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{}

void GvcpServer::readPendingDatagrams()
{
    Q_D(GvcpServer);
    while (d->udp.hasPendingDatagrams()) {
        QHostAddress sender;
        quint16 senderPort;
        const qint64 size = d->udp.readDatagram(d->packet, GVCP_PACKET_MAX_SIZE, &sender, &senderPort);
        if (HeaderCmdHelper::isValid(d->headerCmd, size) && HeaderCmdHelper::acknowledgeIsSet(d->headerCmd))
        {
            switch (HeaderCmdHelper::command(d->headerCmd)) {
            case GVCP_CMD_DISCOVERY:
            {
                DiscoveryAckHelper ack(DISCOVERY_ACK_LENGTH);
                ack.setAckID(HeaderCmdHelper::reqId(d->headerCmd));
                ack.setSpecVersion(d->bootstrap.value(Register::Version));
                ack.setDeviceMode(d->bootstrap.value(Register::DeviceMode));
                ack.setDeviceMACaddressHigh(d->bootstrap.value(Register::DeviceMacAddressHigh_0));
                ack.setDeviceMACaddressLow(d->bootstrap.value(Register::DeviceMacAddressLow_0));
                ack.setIPconfigOptions(d->bootstrap.value(Register::SupportedIPconfiguration_0));
                ack.setIPconfigCurrent(d->bootstrap.value(Register::CurrentIPconfiguration_0));
                ack.setCurrentIP(d->bootstrap.value(Register::CurrentIPAddress_0));
                ack.setCurrentSubnetMask(d->bootstrap.value(Register::CurrentSubnetMask_0));
                ack.setDefaultGateway(d->bootstrap.value(Register::CurrentDefaultGateway_0));
                ack.setManufacturerName(d->bootstrap.constBlock(MemoryBlocs::ManufacturerName));
                ack.setModelName(d->bootstrap.constBlock(MemoryBlocs::ModelName));
                ack.setDeviceVersion(d->bootstrap.constBlock(MemoryBlocs::DeviceVersion));
                ack.setManufacturerInformation(d->bootstrap.constBlock(MemoryBlocs::ManufacturerInfo));
                ack.setSerialNumber(d->bootstrap.constBlock(MemoryBlocs::SerialNumber));
                ack.setUserName(d->bootstrap.constBlock(MemoryBlocs::UserDefinedName));

                d->udp.writeDatagram(ack.data(), ack.size(), sender, senderPort);
                return;
            }
            case GVCP_CMD_READREG:
            {
                QList<quint32> addresses = ReadregCmdHelper::addresses(d->readregCmd);
                QList<quint32> answers;
                foreach (quint32 address, addresses) {
                    if (d->bootstrap.addressIsValid(address)) {
                        answers.append(d->bootstrap.value(address));
                        readRegister(address, sender);
                    }
                    else break;
                }

                ReadregAckHelper ack(answers.count() * sizeof(quint32));
                ack.setAnswers(answers);
                ack.setAckID(HeaderCmdHelper::reqId(d->headerCmd));
                // si le nombre de réponses est différent du nombre d'adresses,
                // on signale dans la réponse
                if (answers.count() != addresses.count()) {
                    ack.changeStatus(STATUS::INVALID_ADDRESS);
                }

                // injecte
                d->heartbeat.start(d->bootstrap.value(Register::HeartbeatTimeout));
                d->udp.writeDatagram(ack.data(), ack.size(), sender, senderPort);
                break;
            }
            case GVCP_CMD_WRITEREG:
            {
                RegistersValueList rvL = WriteregCmdHelper::regsValList(d->writeregCmd);
                quint16 index = 0;

                WriteregAckHelper ack(WRITEREG_ACK_LENGTH);
                for (index=0; index<rvL.count(); ++index) {
                    if (d->bootstrap.addressIsValid(rvL.at(index).first)) {
                        const quint16 status = d->bootstrap.setValue(rvL.at(index).first, rvL.at(index).second);
                        ack.setIndex(index + 1);
                        if (status == STATUS::SUCCESS) {
                            ack.setIndex(index + 1);
                            // hook
                            writeRegister(rvL.at(index).first, rvL.at(index).second);

                        }
                        else{
                            qWarning("Writereg status 0x%x address 0x%x", status, rvL.at(index).first);
                            ack.setIndex(index);
                            ack.changeStatus(status);
                            break;
                        }
                    }
                }

                ack.setAckID(HeaderCmdHelper::reqId(d->headerCmd));
                // injecte
                d->heartbeat.start(d->bootstrap.value(Register::HeartbeatTimeout));
                d->udp.writeDatagram(ack.data(), ack.size(), sender, senderPort);


                break;
            }
            case GVCP_CMD_READMEM:
            {
                ReadmemResult res = d->bootstrap.readmemPtr(ReadmemCmdHelper::address(d->readmemCmd),
                                                            ReadmemCmdHelper::dataCount(d->readmemCmd));

                quint16 count = (res.first==STATUS::SUCCESS) ? ReadmemCmdHelper::dataCount(d->readmemCmd) : 0;

                // la taille de l'ack varie avec la réponse
                ReadmemAckHelper ack((count>0) ? count+sizeof(quint32) : 0);
                if (res.first == STATUS::SUCCESS) {
                    ack.setAddress(ReadmemCmdHelper::address(d->readmemCmd));
                    ack.addMem(res.second, count);
                }
                else {
                    ack.changeStatus(res.first);
                }
                ack.setAckID(HeaderCmdHelper::reqId(d->headerCmd));

                // injecte
                d->heartbeat.start(d->bootstrap.value(Register::HeartbeatTimeout));
                d->udp.writeDatagram(ack.data(), ack.size(), sender, senderPort);
                break;
            }
            case GVCP_CMD_WRITEMEM:
            {
                // le bootstrap gère les erreurs, et la validité du pointeur retourné
                WritememResult res = d->bootstrap.writememPtr(WritememCmdHelper::address(d->writememCmd),
                                                              WritememCmdHelper::count(d->writememCmd));

                // détermine le nombre d'écritures réussissables
                quint16 count = (res.first==STATUS::SUCCESS) ? WritememCmdHelper::count(d->writememCmd) : 0;
                // effectue les écritures
                if (count > 0) {
                    std::memcpy(res.second, WritememCmdHelper::cmdData(d->writememCmd), count);
                }

                // construit l'ack
                WritememAckHelper ack(WRITEMEM_ACK_LENGTH);
                ack.changeStatus(res.first);
                ack.setAckID(HeaderCmdHelper::reqId(d->headerCmd));
                ack.setIndex(count);

                // injecte
                d->heartbeat.start(d->bootstrap.value(Register::HeartbeatTimeout));
                d->udp.writeDatagram(ack.data(), ack.size(), sender, senderPort);
                break;
            }
            default:
                qWarning("GvcpServer paquet ignored %x", HeaderCmdHelper::command(d->headerCmd));

            }
        }
    }
}

void GvcpServer::onHeartbeat()
{
    Q_D(GvcpServer);
    d->bootstrap.setValue(Register::ControlChannelPrivilege, 0x00);
}

void GvcpServer::readRegister(quint32 address, const QHostAddress &sender)
{}

void GvcpServer::writeRegister(quint32 address, quint32 value)
{}



