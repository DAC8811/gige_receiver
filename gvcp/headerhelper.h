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

#ifndef HEADERHELPER_H
#define HEADERHELPER_H

#include "packethelper.h"

struct CMD_HEADER
{
    quint8 hardKey;
    quint8 flag;
    quint8 command[2];
    quint8 length[2];
    quint8 reqId[2];
};

struct ACK_HEADER
{
    quint8 status[2];
    quint8 acknowledge[2];
    quint8 length[2];
    quint8 ackId[2];
};

const quint8 GVCP_HARD_KEY = 0x42;
const uint GVCP_ACK_HEADER_SIZE = sizeof(ACK_HEADER);

class HeaderCmdHelper : public PacketHelper
{
    CMD_HEADER *const header;
public:
    HeaderCmdHelper(quint16 command, quint16 length);
    void setReqId(quint16 id);
    QString toString() const;

    static bool isValid(const CMD_HEADER &header, quint16 packetSize);
    static bool acknowledgeIsSet(const CMD_HEADER &header);
    static quint16 command(const CMD_HEADER &header);
    static quint16 length(const CMD_HEADER &header);
    static quint16 reqId(const CMD_HEADER &header);
};

class HeaderAckHelper : public PacketHelper
{
    ACK_HEADER *const header;
public:
    HeaderAckHelper(quint16 acknowledge, quint16 length);
    void changeStatus(quint16 newStatus);
    void setAckID(quint16 id);

    static bool isValid(const ACK_HEADER &header, quint16 packetSize);
    static quint16 status(const ACK_HEADER &header);
    static quint16 acknowledge(const ACK_HEADER &header);
    static quint16 length(const ACK_HEADER &header);
    static quint16 ackId(const ACK_HEADER &header);
};


#endif // HEADERHELPER_H
