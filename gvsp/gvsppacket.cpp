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

#include "gvsppacket.h"

GvspPacket::GvspPacket(const quint8 *const data, size_t size)
    : ConstMemoryBlock(data, size),
      imagePayload(data + sizeof(GVSP_HEADER), size - sizeof(GVSP_HEADER))
{
    this->packetformat = this->packetFormat();
}

int GvspPacket::payloadType() const
{
    switch(this->packetformat)
    {
    case PACKET_FORMAT::DATA_LEADER:return qFromBigEndian<quint16>(GVSP_L(data)->leader.payloadType);
    case PACKET_FORMAT::DATA_PAYLOAD:qWarning("type payload have no part payloadtype");break;
    case PACKET_FORMAT::DATA_TRAILER:return qFromBigEndian<quint16>(GVSP_T(data)->trailer.payloadType);
    }
    return 0;
}

quint64 GvspPacket::timestamp() const
{
    switch(this->packetformat)
    {
    case PACKET_FORMAT::DATA_LEADER:return qFromBigEndian<quint64>(GVSP_L(data)->leader.timestampHighPart);
    case PACKET_FORMAT::DATA_PAYLOAD:qWarning("type payload have no part timestamp");break;
    case PACKET_FORMAT::DATA_TRAILER:qWarning("type trailer have no part timestamp");break;
    }
    return 0;
}
uint GvspPacket::pixelFormat() const
{
    switch(this->packetformat)
    {
    case PACKET_FORMAT::DATA_LEADER:return qFromBigEndian<quint32>(GVSP_L(data)->pixelFormat);
    case PACKET_FORMAT::DATA_PAYLOAD:qWarning("type payload have no part pixelformat");break;
    case PACKET_FORMAT::DATA_TRAILER:qWarning("type trailer have no part pixelformat");break;
    }
    return 0;
}
uint GvspPacket::width() const
{
    switch(this->packetformat)
    {
    case PACKET_FORMAT::DATA_LEADER:return qFromBigEndian<quint32>(GVSP_L(data)->sizeX);
    case PACKET_FORMAT::DATA_PAYLOAD:qWarning("type payload have no part width");break;
    case PACKET_FORMAT::DATA_TRAILER:qWarning("type trailer have no part width");break;
    }
    return 0;
}
uint GvspPacket::height() const
{
    switch(this->packetformat)
    {
    case PACKET_FORMAT::DATA_LEADER:return qFromBigEndian<quint32>(GVSP_L(data)->sizeY);
    case PACKET_FORMAT::DATA_PAYLOAD:qWarning("type payload have no part height");break;
    case PACKET_FORMAT::DATA_TRAILER:qWarning("type trailer have no part height");break;
    }
    return 0;
}
quint16 GvspPacket::paddingX() const
{
    switch(this->packetformat)
    {
    case PACKET_FORMAT::DATA_LEADER:return qFromBigEndian<quint16>(GVSP_L(data)->paddingX);
    case PACKET_FORMAT::DATA_PAYLOAD:qWarning("type payload have no part paddingX");break;
    case PACKET_FORMAT::DATA_TRAILER:qWarning("type trailer have no part paddingX");break;
    }
    return 0;
}
quint16 GvspPacket::paddingY() const
{
    switch(this->packetformat)
    {
    case PACKET_FORMAT::DATA_LEADER:return qFromBigEndian<quint16>(GVSP_L(data)->paddingY);
    case PACKET_FORMAT::DATA_PAYLOAD:qWarning("type payload have no part paddingY");break;
    case PACKET_FORMAT::DATA_TRAILER:qWarning("type trailer have no part paddingY");break;
    }
    return 0;
}

quint32 GvspPacket::type() const
{
    switch(this->packetformat)
    {
    case PACKET_FORMAT::DATA_LEADER:return qFromBigEndian<quint32>(GVSP_L(data)->offsetX);
    case PACKET_FORMAT::DATA_PAYLOAD:qWarning("type payload have no part type");break;
    case PACKET_FORMAT::DATA_TRAILER:qWarning("type trailer have no part type");break;
    }
    return 0;
}

