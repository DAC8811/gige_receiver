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

#include "writereghelper.h"

#include <QList>
#include <QPair>
#include <QtEndian>

struct ADDRESS_VALUE
{
    uchar address[4];
    uchar value[4];
};

WriteregCmdHelper::WriteregCmdHelper(quint16 length)
    : HeaderCmdHelper(GVCP_CMD_WRITEREG, length),
      cmd(reinterpret_cast<WRITEREG_CMD * const>(m_data))
{}

void WriteregCmdHelper::setRegsValList(const RegistersValueList &values)
{
    uint index = 0;
    for (int i=0; i<values.count(); ++i) {
        qToBigEndian<quint32>(values.at(i).first, &cmd->address_value[index]);
        index += sizeof(quint32);
        qToBigEndian<quint32>(values.at(i).second,& cmd->address_value[index]);
        index += sizeof(quint32);
    }
}

RegistersValueList WriteregCmdHelper::regsValList(const WRITEREG_CMD &cmd)
{
    RegistersValueList result;
    for (int i=0; i<qFromBigEndian<quint16>(cmd.header.length); i+=sizeof(ADDRESS_VALUE)) {
        const ADDRESS_VALUE *av = reinterpret_cast<const ADDRESS_VALUE *>(&cmd.address_value[i]);
        result.append(QPair<quint32,quint32>(qFromBigEndian<quint32>(av->address), qFromBigEndian<quint32>(av->value)));
    }
    return result;
}


WriteregAckHelper::WriteregAckHelper(quint16 lenght)
    : HeaderAckHelper(GVCP_ACK_WRITEREG, lenght),
      ack(reinterpret_cast<WRITEREG_ACK * const>(m_data))
{}

void WriteregAckHelper::setIndex(quint16 index)
{
    qToBigEndian<quint16>(index, ack->index);
}

