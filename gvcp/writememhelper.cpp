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

#include "writememhelper.h"

#include <QtEndian>
#include <cstring>

WritememCmdHelper::WritememCmdHelper(quint16 length)
    : HeaderCmdHelper(GVCP_CMD_WRITEMEM, length),
      cmd(reinterpret_cast<WRITEMEM_CMD * const>(m_data))
{}

void WritememCmdHelper::setAddress(quint32 address)
{
    qToBigEndian<quint32>(address, cmd->address);
}

void WritememCmdHelper::setCmdData(const quint8 *datas, uint size)
{
    std::memcpy(cmd->data, datas, size);
}

quint32 WritememCmdHelper::address(const WRITEMEM_CMD &cmd)
{
    return qFromBigEndian<quint32>(cmd.address);
}

quint16 WritememCmdHelper::count(const WRITEMEM_CMD &cmd)
{
    return HeaderCmdHelper::length(cmd.header) - sizeof(cmd.address);
}

const quint8 *WritememCmdHelper::cmdData(const WRITEMEM_CMD &cmd)
{
    return cmd.data;
}

WritememAckHelper::WritememAckHelper(quint16 lenght)
    : HeaderAckHelper(GVCP_ACK_WRITEMEM, lenght),
      ack(reinterpret_cast<WRITEMEM_ACK * const>(m_data))
{}

void WritememAckHelper::setIndex(quint16 index)
{
    qToBigEndian<quint16>(index, ack->index);

}




