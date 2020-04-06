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

#ifndef FORCEIPHELPER_H
#define FORCEIPHELPER_H


#include "headerhelper.h"

struct FORCEIP_CMD {
    CMD_HEADER header;
    quint8 reserved1[2];
    quint8 MACAddressHigh[2];
    quint8 MACAddressLow[4];
    quint8 reserved2[12];
    quint8 staticIP[4];
    quint8 reserved3[12];
    quint8 staticSubnetMask[4];
    quint8 reserved4[12];
    quint8 staticDefaultGateway[4];
};

struct FORCEIP_ACK {
    ACK_HEADER header;
};

const quint16 GVCP_CMD_FORCEIP                  = 0x0004;
const quint16 GVCP_ACK_FORCEIP                  = 0x0005;
const int FORCEIP_CMD_LENGTH = sizeof(FORCEIP_CMD) - sizeof(CMD_HEADER);
const int FORCEIP_ACK_LENGTH = sizeof(FORCEIP_ACK) - sizeof(ACK_HEADER);


class ForceipCmdHelper : public HeaderCmdHelper
{
    FORCEIP_CMD * const cmd;
public:
    ForceipCmdHelper(quint16 length);
    void setMacHigh(quint16 mac);
    void setMacLow(quint32 mac);
    void setStaticIP(quint32 ip);
    void setStaticNetmask(quint32 netmask);
    void setStaticDefaultGateway(quint32 gateway);

    static quint16 macHigh(const FORCEIP_CMD &cmd);
    static quint32 macLow(const FORCEIP_CMD &cmd);
    static quint32 staticIP(const FORCEIP_CMD &cmd);
    static quint32 staticNetmask(const FORCEIP_CMD &cmd);
    static quint32 staticDefaultGateway(const FORCEIP_CMD &cmd);

};


class ForceipAckHelper : public HeaderAckHelper
{
    FORCEIP_ACK * const ack;
public:
    ForceipAckHelper(quint16 length);

};


#endif // FORCEIPHELPER_H
