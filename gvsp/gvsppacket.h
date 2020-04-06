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

#ifndef GVSPPACKET_H
#define GVSPPACKET_H

#include "constmemoryblock.h"
#include <QtEndian>

const size_t GVSP_BUFFER_SIZE = 10000;
const uint IP_HEADER_SIZE = 20;
const uint UDP_HEADER_SIZE = 8;


namespace E1 {
    const quint8 StandardID = 0x00;
    const quint8 ExtendedID = 0x80;
    const quint8 Mask = 0x80;
}

namespace PACKET_FORMAT {
    const quint16 DATA_LEADER = 1;
    const quint16 DATA_TRAILER = 2;
    const quint16 DATA_PAYLOAD = 3;
    const quint16 ALL_IN = 4;

}

namespace PAYLOAD_TYPE {
    const quint16 Unknow = 0x0000;
    const quint16 Image = 0x0001;
    const quint16 RawData = 0x0002;
    const quint16 File = 0x0003;
}

struct GVSP_HEADER {
    uchar status[2];
    uchar blockID[2];
    uchar packetFormat[1];
    uchar packetID[3];
};

struct GVSP_HEADER_EXT {
    uchar blockId64HighPart[4];
    uchar blockId64LowPart[4];
    uchar packetId32[4];
};

struct GVSP_DATALEADER {
    GVSP_HEADER header;
    uchar reserved[2];
    uchar payloadType[2];
    uchar timestampHighPart[4];
    uchar timestampLowPart[4];
};

struct GVSP_DATALEADER_IMAGE {
    GVSP_DATALEADER leader;
    uchar pixelFormat[4];
    uchar sizeX[4];
    uchar sizeY[4];
    uchar offsetX[4];
    uchar offsetY[4];
    uchar paddingX[2];
    uchar paddingY[2];
};

struct GVSP_PAYLOAD_IMAGE {
    GVSP_HEADER header;
    uchar data[];
};

struct GVSP_DATATRAILER {
    GVSP_HEADER header;
    uchar reserved[2];
    uchar payloadType[2];
};

struct GVSP_DATATRAILER_IMAGE {
    GVSP_DATATRAILER trailer;
    uchar sizeY[4];
};


union GVSP_DATA {
    GVSP_HEADER header;
    GVSP_DATALEADER leader;
    GVSP_DATALEADER_IMAGE leaderImage;
    GVSP_PAYLOAD_IMAGE payload;
    GVSP_DATATRAILER trailer;
    GVSP_DATATRAILER_IMAGE trailerImage;
};

#define GVSP(t) reinterpret_cast<const GVSP_DATA *const>(t)

class GvspPacket : private ConstMemoryBlock
{
public:
    explicit GvspPacket(const quint8 *const data, size_t size);
    int status() const { return qFromBigEndian<quint16>(GVSP(data)->header.status); }
    quint16 blockID() const { return qFromBigEndian<quint16>(GVSP(data)->header.blockID); }
    uint packetID() const { return 0x00FFFFFF & qFromBigEndian<quint32>(GVSP(data)->header.packetFormat); }
    int packetFormat() const { return GVSP(data)->header.packetFormat[0]; }
    int payloadType() const { return qFromBigEndian<quint16>(GVSP(data)->leader.payloadType); }
    quint64 timestamp() const { return qFromBigEndian<quint64>(GVSP(data)->leader.timestampHighPart); }
    uint pixelFormat() const { return qFromBigEndian<quint32>(GVSP(data)->leaderImage.pixelFormat); }
    uint width() const { return qFromBigEndian<quint32>(GVSP(data)->leaderImage.sizeX); }
    uint height() const { return qFromBigEndian<quint32>(GVSP(data)->leaderImage.sizeY); }
    quint16 paddingX() const { return qFromBigEndian<quint16>(GVSP(data)->leaderImage.paddingX); }
    quint16 paddingY() const { return qFromBigEndian<quint16>(GVSP(data)->leaderImage.paddingY); }
    const ConstMemoryBlock imagePayload;
};


#endif // GVSPPACKET_H
