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

#include "readreghelper.h"

#include <QList>
#include <QtEndian>

ReadregCmdHelper::ReadregCmdHelper(quint16 length)
    : HeaderCmdHelper(GVCP_CMD_READREG, length),
      cmd(reinterpret_cast<READREG_CMD * const>(m_data))
{}

void ReadregCmdHelper::setAddresses(const QList<quint32> &addresses)
{
    for (int i=0; i<addresses.count(); ++i) {
        qToBigEndian<quint32>(addresses.at(i), &cmd->addresses[i * sizeof(quint32)]);
    }
}


QList<quint32> ReadregCmdHelper::addresses(const READREG_CMD &cmd)
{
    QList<quint32> result;
    for (int i=0; i<qFromBigEndian<quint16>(cmd.header.length); i+=sizeof(quint32)) {
        result.append(qFromBigEndian<quint32>(&cmd.addresses[i]));
    }
    return result;
}


ReadregAckHelper::ReadregAckHelper(quint16 lenght)
    : HeaderAckHelper(GVCP_ACK_READREG, lenght),
      ack(reinterpret_cast<READREG_ACK * const>(m_data))
{}

void ReadregAckHelper::setAnswers(const QList<quint32> &answers)
{
    for (int i=0; i<answers.count(); ++i) {
        qToBigEndian<quint32>(answers.at(i), &ack->registerData[i * sizeof(quint32)]);
    }
}

QList<quint32> ReadregAckHelper::aswers(const READREG_ACK &ack)
{
    QList<quint32> result;
    for (int i=0; i<HeaderAckHelper::length(ack.header); i+=sizeof(quint32)) {
        result.append(qFromBigEndian<quint32>(&ack.registerData[i]));
    }
    return result;
}




