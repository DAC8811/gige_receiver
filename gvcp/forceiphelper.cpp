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

#include "forceiphelper.h"

#include <QtEndian>

ForceipCmdHelper::ForceipCmdHelper(quint16 length)
    : HeaderCmdHelper(GVCP_CMD_FORCEIP, length),
      cmd(reinterpret_cast<FORCEIP_CMD * const>(m_data))
{}

void ForceipCmdHelper::setMacHigh(quint16 mac)
{
    qToBigEndian<quint16>(mac, cmd->MACAddressHigh);
}

void ForceipCmdHelper::setMacLow(quint32 mac)
{
    qToBigEndian<quint32>(mac, cmd->MACAddressLow);
}

void ForceipCmdHelper::setStaticIP(quint32 ip)
{
    qToBigEndian<quint32>(ip, cmd->staticIP);
}

void ForceipCmdHelper::setStaticNetmask(quint32 netmask)
{
    qToBigEndian<quint32>(netmask, cmd->staticSubnetMask);
}

void ForceipCmdHelper::setStaticDefaultGateway(quint32 gateway)
{
    qToBigEndian<quint32>(gateway, cmd->staticDefaultGateway);
}

quint16 ForceipCmdHelper::macHigh(const FORCEIP_CMD &cmd)
{
    return qFromBigEndian<quint16>(cmd.MACAddressHigh);
}

quint32 ForceipCmdHelper::macLow(const FORCEIP_CMD &cmd)
{
    return qFromBigEndian<quint32>(cmd.MACAddressLow);
}

quint32 ForceipCmdHelper::staticIP(const FORCEIP_CMD &cmd)
{
    return qFromBigEndian<quint32>(cmd.staticIP);
}

quint32 ForceipCmdHelper::staticNetmask(const FORCEIP_CMD &cmd)
{
    return qFromBigEndian<quint32>(cmd.staticSubnetMask);
}

quint32 ForceipCmdHelper::staticDefaultGateway(const FORCEIP_CMD &cmd)
{
    return qFromBigEndian<quint32>(cmd.staticDefaultGateway);
}

ForceipAckHelper::ForceipAckHelper(quint16 length)
    : HeaderAckHelper(GVCP_CMD_FORCEIP, length),
      ack(reinterpret_cast<FORCEIP_ACK * const>(m_data))
{}
