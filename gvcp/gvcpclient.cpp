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

#include "gvcpclient.h"
#include "gvcpclient_p.h"

#include <QThread>

static CCP::Privilege privFromCCP(quint32 CCP)
{
    if ((CCP&0x01)!=0) return CCP::ExclusiveAcces;
    if ((CCP&0x02)==0) return CCP::OpenAcces;
    if ((CCP&0x04)==0) return CCP::ControlAcces;
    return CCP::ControlAccesWithSwitchoverEnabled;
}
static quint32 CCPfromPrivilege(CCP::Privilege privilege) {
    if (privilege==CCP::OpenAcces) return 0x00;
    if (privilege==CCP::ControlAcces) return 0x02;
    if (privilege==CCP::ControlAccesWithSwitchoverEnabled) return 0x06;
    return 0x01;
}

bool GvcpClientPrivate::proceedPacket(HeaderCmdHelper &cmd, quint16 ackType)
{
    const quint16 currentId = id;
    id = (currentId == 0xFFFFu) ? 1 : currentId + 1;
    cmd.setReqId(currentId);

    int retry = 3;
    while (retry-- > 0) {

        // relance le heartbeat
        if (heartbeat.isActive()) {
            heartbeat.start();
        }
        if (udp.writeDatagram(cmd.data(), cmd.size(), peer, GVCP_PORT) < 0) {
            qWarning("gvcp failed to send datagram : %s", qPrintable(udp.errorString()));
            continue;
        }
        // au timeout réexpédie la commande
        if (!udp.waitForReadyRead(500)) {
            qWarning("gvcp timeout, retry %d", 3);
            continue;
        }

        while (udp.hasPendingDatagrams()) {
            QHostAddress sender;
            quint16 senderPort;
            // s'assure que le taille du packet contient au moins un header
            qint64 size = udp.readDatagram(buffer, GVCP_PACKET_MAX_SIZE, &sender, &senderPort);
            // s'assure de la source
            if (sender != peer || senderPort != GVCP_PORT) {
                qWarning("gvcp spamed by %s port %d", qPrintable(sender.toString()), senderPort);
                continue;
            }

            if (!HeaderAckHelper::isValid(headerAck, size)) {
                qWarning("gvcp ack error");
                continue;
            }

            // l'ID de la réponse doit correspondre à l'ID de la commande
            if (HeaderAckHelper::ackId(headerAck) != currentId) {
                qWarning("gvcp bad ID. expected:%d not:%d", currentId, ReadregAckHelper::ackId(headerAck));
                continue;
            }

            // controle que l'ack est du bon type
            if (HeaderAckHelper::acknowledge(headerAck) != ackType) {
                qWarning("gvcp bad aknowledge");
                continue;
            }

            // le status n'est pas succes
            if (HeaderAckHelper::status(headerAck) != STATUS::SUCCESS) {
                qWarning("%s : %s", qPrintable(cmd.toString()), qPrintable(Status::toString(HeaderAckHelper::status(headerAck))));
                return false;
            }

            return true;
        }
    }
    return false;
}


GvcpClient::GvcpClient(QObject *parent)
    : QObject(parent), d_ptr(new GvcpClientPrivate)
{}

GvcpClient::~GvcpClient()
{
    qDebug("Deleting GVCP client");
}

bool GvcpClient::connectTo(const QHostAddress &client, const QHostAddress &server, CCP::Privilege privilege)
{
    Q_D(GvcpClient);
    if (d->udp.state() != QAbstractSocket::BoundState) {
        d->udp.bind(client, 0, QUdpSocket::DontShareAddress);
        d->peer = server;
        d->id = 1;
        connect(&d->heartbeat, SIGNAL(timeout()), this, SLOT(heartbeat()));

        // obtient CCP
        ValuesList regs = readRegisters(AddressList() << Register::ControlChannelPrivilege);
        if (regs.isEmpty()) {
            qWarning("gvcp connection failed: can't read CCP");
            d->udp.close();
            return false;
        }

        CCP::Privilege current = privFromCCP(regs.at(0));
        // on demande un acces en lecture
        if ((privilege == CCP::OpenAcces)&&(current == CCP::ExclusiveAcces)) {
            qWarning("gvcp connection failed: acces denied");
            return false;
        }
        // on demande un acces en écriture
        if ((privilege >= CCP::ControlAcces)&&(current >= CCP::ControlAcces)) {
            qWarning("gvcp connection failed: acces denied");
            return false;
        }
        // acces control
        if (privilege >= CCP::ControlAcces) {
            if (!writeRegisters(AdressValueList() << AddressValue(Register::ControlChannelPrivilege, CCPfromPrivilege(privilege)))) {
                qWarning("gvcp connection failed: CCP acces denied");
                return false;
            }
            // oki on configure le heartbeat
            regs = readRegisters(AddressList() << Register::HeartbeatTimeout);
            if (!regs.isEmpty()) {
                d->heartbeat.start((regs.at(0)/100) *80);
            }
        }
        return true;
    }
    qWarning("gvcp connection failed, already in bound state");
    return false;
}

ValuesList GvcpClient::readRegisters(const AddressList &addresses)
{
    Q_D(GvcpClient);
    // forge la commande
    ReadregCmdHelper cmd(addresses.count() * sizeof(quint32));
    cmd.setAddresses(addresses);

    ValuesList result;
    if (d->proceedPacket(cmd, GVCP_ACK_READREG)) {
        result = ReadregAckHelper::aswers(d->readregAck);
    }
    return result;
}

bool GvcpClient::writeRegisters(const AdressValueList &values)
{
    Q_D(GvcpClient);
    // forge la commande
    WriteregCmdHelper cmd(values.count() * 2 * sizeof(quint32));
    cmd.setRegsValList(values);

    return d->proceedPacket(cmd, GVCP_ACK_WRITEREG);
}

const quint8 *GvcpClient::readMemory(quint32 address, quint16 count)
{
    Q_D(GvcpClient);
    // forge la commande
    ReadmemCmdHelper cmd(READMEM_CMD_LENGTH);
    cmd.setAddress(address);
    cmd.setDataCount(count);

    const quint8 *result = 0;
    if (d->proceedPacket(cmd, GVCP_ACK_READMEM)) {
        result = ReadmemAckHelper::dataPtr(d->readmemAck);
    }
    else {
        qWarning("GvcpClient failed to read memory at %#08X", address);
    }
    return result;
}

bool GvcpClient::writeMemory(quint32 address, const quint8 *data, uint size)
{
    Q_D(GvcpClient);
    // forge la commande
    WritememCmdHelper cmd(sizeof(quint32) + size);
    cmd.setAddress(address);
    cmd.setCmdData(data, size);
    return d->proceedPacket(cmd, GVCP_ACK_WRITEMEM);

}

void GvcpClient::heartbeat()
{
    Q_D(GvcpClient);
    ValuesList regs = readRegisters(AddressList() << Register::ControlChannelPrivilege);
    if (regs.isEmpty()) {
        qWarning("heartbeat failed: can't read CCP");
        //d->udp.close();
    }
}








