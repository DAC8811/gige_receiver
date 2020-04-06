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

#include "gvcpdiscoverer.h"
#include "gvcpdiscoverer_p.h"

#include "forceiphelper.h"


GvcpDiscoverer::GvcpDiscoverer(QObject *parent)
    : QObject(parent),
      d(new GvcpDiscovererPrivate)
{
    connect(&d->udp, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

GvcpDiscoverer::~GvcpDiscoverer()
{}

int GvcpDiscoverer::sendDiscovery(const QHostAddress &src, const QHostAddress &dest, bool broadcastResponse)
{
    if (d->udp.state() != QAbstractSocket::BoundState) {
        if (!d->udp.bind(0, QUdpSocket::DontShareAddress)) {
            qWarning("gvcp discoverer bind failed");
            return -1;
        }
    }
    d->id = (d->id == 0xFFFFu) ? 1 : d->id + 1;
    DiscoveryCmdHelper cmd(DISCOVERY_CMD_LENGTH);
    cmd.allowBroadcastAck(broadcastResponse);
    cmd.setReqId(d->id);
    d->udp.writeDatagramFrom(src, cmd.data(), cmd.size(), dest, GVCP_PORT);
    return d->id;
}

void GvcpDiscoverer::forceIP(const QHostAddress &src, quint64 mac, quint32 newIP, quint32 newNetmask, quint32 newGateway)
{
    if (d->udp.state() != QAbstractSocket::BoundState) {
        if (!d->udp.bind(0, QUdpSocket::DontShareAddress)) {
            qWarning("gvcp discoverer bind failed");
            return;
        }
    }
    d->id = (d->id == 0xFFFFu) ? 1 : d->id + 1;
    ForceipCmdHelper cmd(FORCEIP_CMD_LENGTH);
    cmd.setReqId(d->id);
    cmd.setMacHigh(0xFFFFu & (mac>>32));
    cmd.setMacLow(0xFFFFFFFFu & mac);
    cmd.setStaticIP(newIP);
    cmd.setStaticNetmask(newNetmask);
    cmd.setStaticDefaultGateway(newGateway);
    d->udp.writeDatagramFrom(src, cmd.data(), cmd.size(), QHostAddress(QHostAddress::Broadcast), GVCP_PORT);
}

void GvcpDiscoverer::readPendingDatagrams()
{
    while (d->udp.hasPendingDatagrams()) {
        Packet packet;
        quint64 size = d->udp.readDatagram(packet.buffer, sizeof(packet.buffer));
        if (size < sizeof(ACK_HEADER)) {
            continue;
        }

        if (HeaderAckHelper::status(packet.headerAck) != STATUS::SUCCESS) {
            qWarning("Discoverer error : %s", qPrintable(Status::toString(HeaderAckHelper::status(packet.headerAck))));
            continue;
        }

        if (HeaderAckHelper::ackId(packet.headerAck) != d->id) {
            continue;
        }

        if (HeaderAckHelper::acknowledge(packet.headerAck) == GVCP_ACK_DISCOVERY) {
            if ( (HeaderAckHelper::length(packet.headerAck) != DISCOVERY_ACK_LENGTH)
                 | (size != sizeof(DISCOVERY_ACK)) ) {
                qWarning("发现格式错误的数据包！");//Discover mal formatted packet !
                continue;
            }
            emit newDeviceDiscovered(packet.discoveryAck, HeaderAckHelper::ackId(packet.headerAck));
        }
        else if (HeaderAckHelper::acknowledge(packet.headerAck) == GVCP_ACK_FORCEIP) {
            if ( (HeaderAckHelper::length(packet.headerAck) != FORCEIP_ACK_LENGTH)
                 | (size != sizeof(FORCEIP_ACK)) ) {
                qWarning("发现格式错误的数据包!");
                continue;
            }
            emit forceIPsucces();
        }
    }
}
