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

#include "readmemhelper.h"

#include <QtEndian>
#include <cstring>

ReadmemCmdHelper::ReadmemCmdHelper(quint32 length)
    : HeaderCmdHelper(GVCP_CMD_READMEM, length),
      cmd(reinterpret_cast<READMEM_CMD * const>(m_data))
{}

void ReadmemCmdHelper::setAddress(quint32 address)
{
    qToBigEndian<quint32>(address, cmd->address);
}

void ReadmemCmdHelper::setDataCount(quint16 count)
{
    qToBigEndian<quint16>(count, cmd->count);
}

quint32 ReadmemCmdHelper::address(const READMEM_CMD &cmd)
{
    return qFromBigEndian<quint32>(cmd.address);
}

quint16 ReadmemCmdHelper::dataCount(const READMEM_CMD &cmd)
{
    return qFromBigEndian<quint16>(cmd.count);
}

ReadmemAckHelper::ReadmemAckHelper(quint16 lenght)
    : HeaderAckHelper(GVCP_ACK_READMEM, lenght),
      ack(reinterpret_cast<READMEM_ACK * const>(m_data))
{}

void ReadmemAckHelper::setAddress(quint32 address)
{
    qToBigEndian<quint32>(address, ack->address);
}

void ReadmemAckHelper::addMem(const quint8 *mem, uint size)
{
    std::memcpy(ack->datas, mem, size);
}

quint32 ReadmemAckHelper::address(const READMEM_ACK &ack)
{
    return qFromBigEndian<quint32>(ack.address);
}

const quint8 *ReadmemAckHelper::dataPtr(const READMEM_ACK &ack)
{
    return ack.datas;
}





