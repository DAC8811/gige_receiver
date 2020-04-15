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

#include "gvspreceiver.h"
//#include "gvspreceiver_p.h"
#include "gvsppacket.h"
#include "gvspblock.h"
//#include "../gvcp/gvcpclient.h"
#include "../gvcp/gvcp.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/resource.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/filter.h>
#include <cap-ng.h>
#include <boost/array.hpp>

#include <QDBusInterface>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QDebug>
#include <QMutex>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef SCHED_RESET_ON_FORK
#define SCHED_RESET_ON_FORK 0x40000000
#endif

#ifndef RLIMIT_RTTIME
#define RLIMIT_RTTIME 15
#endif
namespace Ui { class MainWindow; }

struct BlockDesc {
    uint32_t version;
    uint32_t userOffset;
    tpacket_hdr_v1 header_v1;
};


//GvspReceiver::GvspReceiverPrivate(GvspClient* client)
//    : client(client),
//      run(true),
//      receiverPort(0),
//      block(NULL)
//{}


int GvspReceiver::setupSocket(const tpacket_req3 &req)
{
    int sd;
    // création du socket packet
    if ((sd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0 ){
        const int errn = errno;
        qCritical("erreur create socket %s", strerror(errn));
        return sd;
    }

    // on utilise la version 3 des socket packet
    int version = TPACKET_V3;
    if (setsockopt(sd, SOL_PACKET, PACKET_VERSION, &version, sizeof(version)) < 0) {
        const int errn = errno;
        qCritical("erreur packet version %s", strerror(errn));
        return -1;
    }

    if (setsockopt(sd, SOL_PACKET, PACKET_RX_RING, &req, sizeof(req)) < 0) {
        const int errn = errno;
        qCritical("erreur packet ring %s", strerror(errn));
        return -1;
    }

    return sd;
}

void GvspReceiver::setUdpPortFilter(int sd, quint16 port)
{
    // filtre UDP and dst PORT
    sock_filter bpf_filter[] = {
        { 0x28, 0, 0, 0x0000000c },
        { 0x15, 0, 4, 0x000086dd },
        { 0x30, 0, 0, 0x00000014 },
        { 0x15, 0, 11, 0x00000011 },
        { 0x28, 0, 0, 0x00000038 },
        { 0x15, 8, 9, 0x000000ff },
        { 0x15, 0, 8, 0x00000800 },
        { 0x30, 0, 0, 0x00000017 },
        { 0x15, 0, 6, 0x00000011 },
        { 0x28, 0, 0, 0x00000014 },
        { 0x45, 4, 0, 0x00001fff },
        { 0xb1, 0, 0, 0x0000000e },
        { 0x48, 0, 0, 0x00000010 },
        { 0x15, 0, 1, static_cast<quint32>(port) },
        { 0x6, 0, 0, 0x00040000 },
        { 0x6, 0, 0, 0x00000000 }
    };
    sock_fprog bpf_prog;

    bpf_prog.filter = bpf_filter;
    bpf_prog.len = sizeof(bpf_filter) / sizeof(struct sock_filter);
    if (setsockopt(sd, SOL_SOCKET, SO_ATTACH_FILTER, &bpf_prog, sizeof(bpf_prog)) < 0) {
        const int errn = errno;
        qCritical("erreur setsockopt set filter: %s", strerror(errn));
    }
}

bool GvspReceiver::bindPacketSocket(int sd, int nicIndex)
{
    // bind
    sockaddr_ll localAddress = {0};
    localAddress.sll_family   = AF_PACKET;
    localAddress.sll_protocol = htons(ETH_P_ALL);
    localAddress.sll_ifindex  = nicIndex;
    localAddress.sll_hatype   = 0;
    localAddress.sll_pkttype  = 0;
    localAddress.sll_halen    = 0;
    int b = bind(sd, (struct sockaddr *)&localAddress, sizeof(localAddress));
    if (b == -1) {
        const int errn = errno;
        qWarning("erreur bind %s", strerror(errn));
    }
    return (b == 0);
}

void GvspReceiver::setRealtime()
{
    QDBusInterface rtkit("org.freedesktop.RealtimeKit1",
                         "/org/freedesktop/RealtimeKit1",
                         "org.freedesktop.RealtimeKit1",
                         QDBusConnection::systemBus());

    if (rtkit.isValid()) {

        quint32 val;
        quint64 time;

        QVariant prop = rtkit.property("MaxRealtimePriority");
        if (prop.isValid()) {
            val = prop.toUInt();
        }
        else {
            qWarning("%s", qPrintable(QObject::trUtf8("rtKit : la propriété MaxRealtimePriority est invalide.")));
            return;
        }

        prop = rtkit.property("RTTimeNSecMax");
        if (prop.isValid()) {
            time = prop.toULongLong();
        }
        else {
            prop = rtkit.property("RTTimeUSecMax");
            if (prop.isValid()) {
                time = prop.toULongLong();
            }
            else {
                qWarning("%s", qPrintable(QObject::trUtf8("rtKit : la propriété RTTime?SecMax est invalide.")));
                return;
            }
        }

        rlimit rlim = {0};
        rlim.rlim_cur = rlim.rlim_max = time;
        if ((setrlimit(RLIMIT_RTTIME, &rlim) < 0)) {
            qWarning("%s", qPrintable(QObject::trUtf8("rtKit : échec set RLIMIT_RTTIME. %1").arg(strerror(errno))));
            return;
        }

        quint64 tid = syscall(SYS_gettid);
        rtkit.call("MakeThreadRealtime", tid, val);
    }
    else {
        qWarning("%s", qPrintable(QObject::trUtf8("rtKit : service inaccessible.")));
    }
}

int GvspReceiver::nicIndexFromAddress(const QHostAddress &address)
{
    QList<QNetworkInterface> nis = QNetworkInterface::allInterfaces();
    //正在寻找与IP对应的接口
    int index = -1;
    QList<QNetworkInterface>::const_iterator ni = nis.constBegin();
    while (ni != nis.constEnd() && (index < 0)) {
        QList<QNetworkAddressEntry> naes = (*ni).addressEntries();
        QList<QNetworkAddressEntry>::const_iterator nae = naes.constBegin();
        while (nae != naes.constEnd()) {
            if ((*nae).ip() == address) {
                index = (*ni).index();
                break;
            }
            ++nae;
        }
        ++ni;
    }
    return index;
}


GvspReceiver::GvspReceiver(GvspClient* client, QObject *parent,Ui::MainWindow *ui)
    : QThread(parent),client(client),_run(true),receiverPort(0),block(NULL),ui(ui)
//      d(new GvspReceiverPrivate(client))
{}

GvspReceiver::~GvspReceiver()
{
    this->_run = false;
    wait();
    qDebug("Deleting Threaded Socket");
}

quint16 GvspReceiver::getFreePort(const QHostAddress &address)
{
    QUdpSocket udp;
    quint16 port = 0;
    if (udp.bind(address, 0, QUdpSocket::DontShareAddress)) {
        port = udp.localPort();
    }
    udp.abort();
    return port;
}

void GvspReceiver::listen(const QHostAddress &receiverAddress, quint16 receiverPort, const QHostAddress &transmitterAddress)
{
    this->receiver = receiverAddress;
    this->receiverPort = receiverPort;
    this->transmitter = transmitterAddress;
    qWarning("begin listen");
    start();
}

void GvspReceiver::run()
{
    //UDP套接字描述符
    int sd;
    if ( (sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
        qWarning("%s",qPrintable(trUtf8("Echec création socket GVSP")));
        return;
    }

    //UDP套接字绑定
    sockaddr_in localAddress = {0};
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(this->receiverPort);
    localAddress.sin_addr.s_addr = htonl(this->receiver.toIPv4Address());
    if ( bind(sd, (struct sockaddr *)&localAddress, sizeof(struct sockaddr_in) ) == -1 ) {
        qWarning("%s",qPrintable(trUtf8("Echec bind socket GVSP")));
        return;
    }

    //我们得到分配的端口号
    struct sockaddr_in bindAddress; 
    socklen_t bindAddressSize = sizeof(struct sockaddr_in);
    getsockname(sd, (struct sockaddr *)&bindAddress, &bindAddressSize);

    //获得实时调度
    this->setRealtime();
    this->userStack(sd);
    close(sd);
}

void GvspReceiver::userStack(int socketDescriptor)
{
    qDebug("Using user mode stack");

    //在套接字描述符上准备轮询
    pollfd pfd = {0};
    pfd.fd = socketDescriptor;
    pfd.events = POLLIN;

    // 循环中的全局变量
    int pollResult = 0;

    const int VLEN = 5;
    // 缓冲区的分配和初始化
    uint8_t bufs[VLEN][GVSP_BUFFER_SIZE] = {{0}};
    //传递给recvmmsg的结构数组
    mmsghdr msgs[VLEN] = {0};
    //用于缓冲区映射
    iovec iovecs[VLEN] = {0};
    //实内存映射
    for (int i = 0; i < VLEN; i++) {
        iovecs[i].iov_base         = bufs[i];           // 第n个缓冲区的地址
        iovecs[i].iov_len          = GVSP_BUFFER_SIZE;  // 大小
        msgs[i].msg_hdr.msg_iov    = &iovecs[i];        //相应的iovec的地址
        msgs[i].msg_hdr.msg_iovlen = 1;                 //向量中有1个元素
    }

    timespec timeout = {0};
    timeout.tv_sec = 0;
    timeout.tv_nsec = 500000000ll;

    while (_run) {
        // 等待事件阅读
        if ( (pollResult = poll(&pfd, 1, 200)) > 0 ) {
            const int retval = recvmmsg(socketDescriptor, msgs, VLEN, 0, &timeout);//retval对应的是接收的信息数，即报文数
            if (retval > 0) {
                for (int i=0; i < retval; ++i) {
                    GvspPacket gvsp(reinterpret_cast<const quint8 *>(iovecs[i].iov_base), msgs[i].msg_len);
                    //精确输出每个包的各种解析消息
                    //qWarning("status:%d blockID:%d packetID:%d packetFormat:%d payloadType:%d timestamp:%d pixelformat:%d width:%d height:%d",gvsp.status(),gvsp.blockID(),gvsp.packetID(),gvsp.packetFormat(),gvsp.payloadType(),gvsp.timestamp(),gvsp.pixelFormat(),gvsp.width(),gvsp.height());
                    doGvspPacket(gvsp);
                }
            }
            else if (retval == -1) {
                int errn = errno;
                qWarning("recvmmsg error %d %s", errn, strerror(errn));
                continue;
            }

        }
        else {
            if (pollResult < 0) {
                const int errn = errno;
                if (errn == EINTR) {
                }
                else {
                    qWarning("poll() error %d %s", errn, strerror(errn));
                    _run = false;
                }

            }
        }
    }
}

void GvspReceiver::doGvspPacket(const GvspPacket &gvsp)
{
    const int status = gvsp.status();
    const uint blockID = gvsp.blockID();
    packet_num = blockID;
    const int packetFormat = gvsp.packetFormat();
    const uint packetID = gvsp.packetID();

    if  (Q_LIKELY(status == STATUS::SUCCESS)) {
        if (packetFormat == PACKET_FORMAT::DATA_PAYLOAD) {
            if (block->num == blockID) {
                //qWarning("packetID%d",packetID);
                block->insert(packetID, gvsp.imagePayload);
            }
        }
        else if (packetFormat == PACKET_FORMAT::DATA_LEADER) {
            timespec start;
            // TODO mettre en place le mécanisme de datation
            clock_gettime(CLOCK_REALTIME_COARSE, &start);
            if (block != NULL) {
                delete block;
            }

            //d->block = new GvspBlock(blockID, d->payloadSize, gvsp.width(), gvsp.height(), gvsp.pixelFormat(), d->segmentSize);
            block = new GvspBlock(blockID, gvsp.width(), gvsp.height(), gvsp.pixelFormat(),gvsp.type());
            block->timestamp = static_cast<qint64>(start.tv_sec) * Q_INT64_C(1000000000) + start.tv_nsec;
            block->allocate();

        }
        else if (packetFormat == PACKET_FORMAT::DATA_TRAILER) {
            qWarning("blockID:%d",blockID);
            block->push(this->Save_path,this->File_type);
            if(this->ui)
            {
                QString for_tip = "正在传输,已传输"+QString::number(blockID)+"张图片";
                this->ui->tips->setText(for_tip);
            }
        }
        else {
            qWarning("packet format not handled");
        }
    } else qWarning("unsucces : %d", status);
}














