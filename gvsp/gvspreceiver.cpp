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
#include "gvspreceiver_p.h"
#include "gvsppacket.h"
#include "gvspblock.h"
#include "../gvcp/gvcpclient.h"
#include "../gvcp/gvcp.h"

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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef SCHED_RESET_ON_FORK
#define SCHED_RESET_ON_FORK 0x40000000
#endif

#ifndef RLIMIT_RTTIME
#define RLIMIT_RTTIME 15
#endif


struct BlockDesc {
    uint32_t version;
    uint32_t userOffset;
    tpacket_hdr_v1 header_v1;
};


GvspReceiverPrivate::GvspReceiverPrivate(GvspClient* client)
    : client(client),
      run(true),
      receiverPort(0),
      block(NULL)
{}

tpacket_req3 GvspReceiverPrivate::createRing()
{
    tpacket_req3 req = {0};
    req.tp_block_size = getpagesize() << 2;        // 16384 octets, soit 1 trame en MTU jumbo frame 16,384字节，即MTU巨型帧中的1帧
    req.tp_frame_size = TPACKET_ALIGNMENT << 6;    // 1024 octets, le plus petit packet GVSP étant 576 1024字节，最小的GVSP数据包为576
    req.tp_block_nr = 512;                         //
    req.tp_frame_nr = (req.tp_block_size * req.tp_block_nr) / req.tp_frame_size;
    req.tp_sizeof_priv = 0;
    req.tp_retire_blk_tov = 200;
    //ring.req.tp_feature_req_word = TP_FT_REQ_FILL_RXHASH;
    return req;
}

int GvspReceiverPrivate::setupSocket(const tpacket_req3 &req)
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

quint8 *GvspReceiverPrivate::mapRing(int sd, const tpacket_req3 &req)
{
    // mapping du ring
    quint8 *map = NULL;
    if ((map = static_cast<quint8 *>(mmap(NULL, req.tp_block_size * req.tp_block_nr, PROT_READ | PROT_WRITE,
                                          MAP_SHARED, sd, 0))) == MAP_FAILED )
    {
        const int errn = errno;
        qWarning("Erreur mapping %s", strerror(errn));
    }
    return map;
}

void GvspReceiverPrivate::setUdpPortFilter(int sd, quint16 port)
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

bool GvspReceiverPrivate::bindPacketSocket(int sd, int nicIndex)
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

void GvspReceiverPrivate::setRealtime()
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

int GvspReceiverPrivate::nicIndexFromAddress(const QHostAddress &address)
{
    QList<QNetworkInterface> nis = QNetworkInterface::allInterfaces();
    // on cherche l'interface correspondant à l'IP 我们正在寻找与IP对应的接口
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


GvspReceiver::GvspReceiver(GvspClient* client, QObject *parent)
    : QThread(parent),
      d(new GvspReceiverPrivate(client))
{}

GvspReceiver::~GvspReceiver()
{
    d->run = false;
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
    d->receiver = receiverAddress;
    d->receiverPort = receiverPort;
    d->transmitter = transmitterAddress;
    start();
}

void GvspReceiver::run()
{
    // descripteur du socket UDP UDP套接字描述符
    int sd;
    if ( (sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
        qWarning("%s",qPrintable(trUtf8("Echec création socket GVSP")));
        return;
    }

    // bind du socket UDP UDP套接字绑定
    sockaddr_in localAddress = {0};
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(d->receiverPort);
    localAddress.sin_addr.s_addr = htonl(d->receiver.toIPv4Address());
    if ( bind(sd, (struct sockaddr *)&localAddress, sizeof(struct sockaddr_in) ) == -1 ) {
        qWarning("%s",qPrintable(trUtf8("Echec bind socket GVSP")));
        return;
    }

    // on récupère le numéro de port affecté 我们得到分配的端口号
    struct sockaddr_in bindAddress; 
    socklen_t bindAddressSize = sizeof(struct sockaddr_in);
    getsockname(sd, (struct sockaddr *)&bindAddress, &bindAddressSize);

    // obteint l'ordonnancement temps réel 获得实时调度
    d->setRealtime();

    // si on a la capacity on démarre un socket mmap ring buffer 如果有能力，我们启动一个mmap环形缓冲区插槽
//    if (capng_have_capability(CAPNG_EFFECTIVE, CAP_NET_RAW)) {
//        d->ringStack(d->receiverPort);
//    }
//    else {
        d->userStack(sd);
//    }

    close(sd);
}

void GvspReceiverPrivate::userStack(int socketDescriptor)
{
    qDebug("Using user mode stack");

    // préparation du polling sur le descripteur de socket  在套接字描述符上准备轮询
    pollfd pfd = {0};
    pfd.fd = socketDescriptor;
    pfd.events = POLLIN;

    // variables globales à la boucle   循环中的全局变量
    int pollResult = 0;

    const int VLEN = 8;
    // allocation et initialisation réelle des tampons 缓冲区的分配和初始化
    uint8_t bufs[VLEN][GVSP_BUFFER_SIZE] = {{0}};
    // tableau de structures à passer à recvmmsg 传递给recvmmsg的结构数组
    mmsghdr msgs[VLEN] = {0};
    // pour le mappage des tampons 用于缓冲区映射
    iovec iovecs[VLEN] = {0};
    // mappage de la mémoire réelle 实内存映射
    for (int i = 0; i < VLEN; i++) {
        iovecs[i].iov_base         = bufs[i];           // l'adresse du nième tampon 第n个图章的地址
        iovecs[i].iov_len          = GVSP_BUFFER_SIZE;  // sa taille 大小
        msgs[i].msg_hdr.msg_iov    = &iovecs[i];        // l'adresse du iovec correspondant 相应的iovec的地址
        msgs[i].msg_hdr.msg_iovlen = 1;                 // 1 seul élément dans le vecteur 向量中有1个元素
    }

    timespec timeout = {0};
    timeout.tv_sec = 0;
    timeout.tv_nsec = 500000000ll;

    while (run) {
        // attend un évènement à lire 等待事件阅读
        if ( (pollResult = poll(&pfd, 1, 200)) > 0 ) {
            const int retval = recvmmsg(socketDescriptor, msgs, VLEN, 0, &timeout);
            if (retval > 0) {
                for (int i=0; i < retval; ++i) {
                    GvspPacket gvsp(reinterpret_cast<const quint8 *>(iovecs[i].iov_base), msgs[i].msg_len);
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
                    // surement gprof
                }
                else {
                    qWarning("poll() error %d %s", errn, strerror(errn));
                    run = false;
                }

            }
        }
    }
}


void GvspReceiverPrivate::ringStack(quint16 port)
{
    qDebug("Using ring buffer mode stack");

    // dimensionne le ring  调整环的尺寸
    tpacket_req3 req = createRing();

    // creation du socket
    int sd = setupSocket(req);

    // mapping du ring
    quint8 *map = mapRing(sd, req);
    if (map == MAP_FAILED) {
        return;
    }

    setUdpPortFilter(sd, port);

    const int nicIndex = nicIndexFromAddress(QHostAddress(receiver));

    if (!bindPacketSocket(sd, nicIndex)) {
        qWarning("Failed to bind packet socket");
        return;
    }

    // allocation des descripteurs de block
    iovec vecs[req.tp_block_nr];
    iovec zero = {0};
    std::fill(vecs, vecs + req.tp_block_nr, zero);
    for (uint i=0; i<req.tp_block_nr;++i) {
        vecs[i].iov_base = map + (i * req.tp_block_size);
        vecs[i].iov_len = req.tp_block_size;
    }

    // préparation du polling sur le descripteur de socket
    pollfd pfd = {0};
    pfd.fd = sd;
    pfd.events = POLLIN;

    // on commence par le premier block
    unsigned int currentBlockNum = 0;

    while (run) {
        // on caste le bloc
        BlockDesc *bd = reinterpret_cast<BlockDesc *>(vecs[currentBlockNum].iov_base);

        // sil le bloc est indisponible
        if ((bd->header_v1.block_status & TP_STATUS_USER) == 0) {
            // on attends un évènement sur le socket packet
            poll(&pfd, 1, 100);
            // le poll est déclenché on recommence la procédure
            continue;
        }
        doBlock(bd);
        bd->header_v1.block_status = TP_STATUS_KERNEL;
        //__sync_synchronize();

        currentBlockNum = (currentBlockNum + 1) % req.tp_block_nr;
    }

    munmap(map, req.tp_block_size * req.tp_block_nr);
    close(sd);
}












void GvspReceiverPrivate::doBlock(const BlockDesc *block)
{
    // le premier header de type 3 est à l'offset indiqué dans le header tpacket_hdr_v1
    //tpacket3_hdr *header = reinterpret_cast<tpacket3_hdr*>((uint8_t*)block + block->header_v1.offset_to_first_pkt);
    const tpacket3_hdr *header = reinterpret_cast<const tpacket3_hdr*>(reinterpret_cast<const quint8 *>(block) + block->header_v1.offset_to_first_pkt);

    // on parcourt toutes les grames décrites dans le bloc
    for (__u32 i=0; i<block->header_v1.num_pkts; ++i) {
        // on a besoin de l'ente IP pour connaitre la taille du paquet
        const iphdr *ip = reinterpret_cast<const iphdr *>(reinterpret_cast<const uint8_t *>(header) + header->tp_mac + ETH_HLEN); // on saute l'entete ethernet
        // quelques vérifivations pour le debug
        Q_ASSERT(ip->protocol == IPPROTO_UDP);
        // GiGE vision interdit les options d'entetes IP, la taille de l'entête doit être de 5 (32 bits)
        Q_ASSERT(ip->ihl == 5);


        GvspPacket gvsp(reinterpret_cast<const quint8 *>(ip) + IP_HEADER_SIZE + UDP_HEADER_SIZE,
                        ntohs(ip->tot_len) - IP_HEADER_SIZE - UDP_HEADER_SIZE);

        doGvspPacket(gvsp);


        // l'offset de l'header suivant est indiqué dans l'header courant
        header = reinterpret_cast<const tpacket3_hdr*>(reinterpret_cast<const quint8 *>(header) + header->tp_next_offset);
        //        __sync_synchronize();
    }
}

void GvspReceiverPrivate::doGvspPacket(const GvspPacket &gvsp)
{
    const int status = gvsp.status();
    const uint blockID = gvsp.blockID();
    const int packetFormat = gvsp.packetFormat();
    const uint packetID = gvsp.packetID();

    if  (Q_LIKELY(status == STATUS::SUCCESS)) {
        if (packetFormat == PACKET_FORMAT::DATA_PAYLOAD) {
            if (block->num == blockID) {
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
            block = new GvspBlock(blockID, gvsp.width(), gvsp.height(), gvsp.pixelFormat());
            block->timestamp = static_cast<qint64>(start.tv_sec) * Q_INT64_C(1000000000) + start.tv_nsec;
            client->allocate(*block);

        }
        else if (packetFormat == PACKET_FORMAT::DATA_TRAILER) {
            client->push(*block);
        }
        else {
            qWarning("packet format not handled");
        }
    } else qWarning("unsucces : %d", status);
}














