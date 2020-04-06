/***************************************************************************
 *   Copyright (C) 2014-2015 by Cyril BALETAUD                                  *
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

#include "broadcastudpsocket.h"

#include <sys/socket.h>
#include <netinet/in.h>


BroadcastUdpSocket::BroadcastUdpSocket(QObject *parent)
    : QUdpSocket(parent)
{
    int on = 1;
    int fd = socketDescriptor();
    setsockopt(fd, SOL_IP, IP_PKTINFO, &on, sizeof(on));
}

BroadcastUdpSocket::~BroadcastUdpSocket()
{
    qDebug("Deleting BroadcastUdpSocket");
}


qint64 BroadcastUdpSocket::writeDatagramFrom(const QHostAddress &fromAddress, const char *data, qint64 size, const QHostAddress &address, quint16 port)
{
    iovec iov = { (char *)data, (size_t)size };

    struct sockaddr_in addrTo;
    addrTo.sin_family = AF_INET;
    addrTo.sin_port = htons(port);
    addrTo.sin_addr.s_addr = htonl(address.toIPv4Address());

    // construction du tampon des données de service
    char buffer[CMSG_SPACE(sizeof(in_pktinfo))];
    memset(&buffer, 0, sizeof(buffer));

    // construction du message
    msghdr msg;
    msg.msg_name = &addrTo;
    msg.msg_namelen = sizeof(addrTo);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = &buffer;
    msg.msg_controllen = sizeof(buffer);
    msg.msg_flags = 0;

    // le pointeur sur les données de service
    cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_IP;
    cmsg->cmsg_type = IP_PKTINFO;
    cmsg->cmsg_len = CMSG_LEN(sizeof(in_pktinfo));
    in_pktinfo *pktinfo = reinterpret_cast<in_pktinfo *>(CMSG_DATA(cmsg));
    pktinfo->ipi_ifindex = 0;
    pktinfo->ipi_spec_dst.s_addr = htonl(fromAddress.toIPv4Address());

    ssize_t sentBytes = sendmsg(socketDescriptor(), &msg, 0);
    if (sentBytes >= 0) {
        emit bytesWritten(sentBytes);
    }
    else {
        emit error(QAbstractSocket::UnknownSocketError);

    }
    return qint64(sentBytes);
}
