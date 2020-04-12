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

#ifndef GVSPRECEIVER_H
#define GVSPRECEIVER_H

#include <QThread>
#include <QScopedPointer>
#include <QHostAddress>
#include <QSharedPointer>

template <class T>
class QSharedPointer;
class GvspClient;
class QHostAddress;

class GvspClient;
class GvspPacket;
class GvspBlock;
struct BlockDesc;
struct tpacket_req3;

//class GvspReceiverPrivate;
class GvspReceiver : public QThread
{
    //        Q_OBJECT

public:
    //explicit GvspReceiver(QSharedPointer<GvspClient> client, QObject *parent = 0);
    explicit GvspReceiver(GvspClient* client, QObject *parent = 0);
    ~GvspReceiver();

    quint16 getFreePort(const QHostAddress &address);
    void listen(const QHostAddress &receiverAddress, quint16 receiverPort, const QHostAddress &transmitterAddress);

private:
    void run();

//protected:
//    const QScopedPointer<GvspReceiverPrivate> d;

private:
    Q_DISABLE_COPY(GvspReceiver)

public:
    GvspClient* client;  //数据包接收端
    volatile bool _run;                  //控制回路输出

    QHostAddress receiver;
    QHostAddress transmitter;
    quint16 receiverPort;

    GvspBlock *block;



    static int setupSocket(const tpacket_req3 &req);
//    static quint8 *mapRing(int sd, const tpacket_req3 &req);
    static void setUdpPortFilter(int sd, quint16 port);
    static bool bindPacketSocket(int sd, int nicIndex);
    static void setRealtime();
    static int nicIndexFromAddress(const QHostAddress &address);

    void userStack(int socketDescriptor);

    void doGvspPacket(const GvspPacket &gvsp);

};

#endif // GVSPRECEIVER_H
