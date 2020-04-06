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

#include "headerhelper.h"
#include "gvcp.h"

#include <QtEndian>

HeaderCmdHelper::HeaderCmdHelper(quint16 command, quint16 length)
    : PacketHelper(sizeof(CMD_HEADER) + length),
      header(reinterpret_cast<CMD_HEADER *const>(m_data))
{
    header->hardKey = GVCP_HARD_KEY;
    header->flag = 0x01;
    qToBigEndian<quint16>(command, header->command);
    qToBigEndian<quint16>(length, header->length);
}

void HeaderCmdHelper::setReqId(quint16 id)
{
    qToBigEndian<quint16>(id, header->reqId);
}

QString HeaderCmdHelper::toString() const
{
    QString command;
    switch (qFromBigEndian<quint16>(header->command)) {
    case GVCP_CMD_DISCOVERY: command = "DISCOVERY";break;
        //    case PACKETRESEND_CMD: command = "PACKETRESEND";break;
    case GVCP_CMD_FORCEIP: command = "FORCEIP";break;
    case GVCP_CMD_READREG: command = "READREG";break;
    case GVCP_CMD_WRITEREG: command = "WRITEREG";break;
    case GVCP_CMD_READMEM: command = "READMEM";break;
    case GVCP_CMD_WRITEMEM: command = "WRITEMEM";break;
    default:
        command = QString("UNKNOW 0x%0").arg(qFromBigEndian<quint16>(header->command), 4, 16, QLatin1Char('0'));
    }

    return QString("command[%0]").arg(command);
}

bool HeaderCmdHelper::isValid(const CMD_HEADER &header, quint16 packetSize)
{
    if (packetSize < sizeof(ACK_HEADER)) {
        qWarning("Ack header size too small: %d", packetSize);
        return false;
    }
    else if (header.flag != GVCP_HARD_KEY) {
        qWarning("Bad cmd header flag: %x (not 0x42)", header.flag);
        return false;
    }
    else if (packetSize != (sizeof(ACK_HEADER) + length(header))) {
        qWarning("Bad cmd header size: %d, expected: %lu", packetSize, sizeof(ACK_HEADER) + length(header));
        return false;
    }
    return true;
}

bool HeaderCmdHelper::acknowledgeIsSet(const CMD_HEADER &header)
{
    return (header.flag & 0x01) == 1;
}

quint16 HeaderCmdHelper::command(const CMD_HEADER &header)
{
    return qFromBigEndian<quint16>(header.command);
}

quint16 HeaderCmdHelper::length(const CMD_HEADER &header)
{
    return qFromBigEndian<quint16>(header.length);
}

quint16 HeaderCmdHelper::reqId(const CMD_HEADER &header)
{
    return qFromBigEndian<quint16>(header.reqId);
}

HeaderAckHelper::HeaderAckHelper(quint16 acknowledge, quint16 length)
    : PacketHelper(sizeof(ACK_HEADER) + length),
      header(reinterpret_cast<ACK_HEADER * const>(m_data))
{
    qToBigEndian<quint16>(acknowledge, header->acknowledge);
    qToBigEndian<quint16>(STATUS::SUCCESS, header->status);
    qToBigEndian<quint16>(length, header->length);
}

void HeaderAckHelper::changeStatus(quint16 newStatus)
{
    qToBigEndian<quint16>(newStatus, header->status);
}

void HeaderAckHelper::setAckID(quint16 id)
{
    qToBigEndian<quint16>(id, header->ackId);
}

bool HeaderAckHelper::isValid(const ACK_HEADER &header, quint16 packetSize)
{
    if (packetSize < sizeof(ACK_HEADER)) {
        qWarning("Ack header size too small: %d", packetSize);
        return false;
    }
    else if (packetSize != (sizeof(ACK_HEADER) + length(header))) {
        qWarning("Bad ack size: %d, expected: %lu", packetSize, sizeof(ACK_HEADER) + length(header));
        return false;
    }
    return true;
}

quint16 HeaderAckHelper::status(const ACK_HEADER &header)
{
    return qFromBigEndian<quint16>(header.status);
}

quint16 HeaderAckHelper::acknowledge(const ACK_HEADER &header)
{
    return qFromBigEndian<quint16>(header.acknowledge);
}

quint16 HeaderAckHelper::length(const ACK_HEADER &header)
{
    return qFromBigEndian<quint16>(header.length);
}

quint16 HeaderAckHelper::ackId(const ACK_HEADER &header)
{
    return qFromBigEndian<quint16>(header.ackId);
}






