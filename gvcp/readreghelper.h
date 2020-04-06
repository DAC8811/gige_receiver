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

#ifndef READREGHELPER_H
#define READREGHELPER_H


#include "headerhelper.h"

const quint16 GVCP_CMD_READREG                  = 0x0080;
const quint16 GVCP_ACK_READREG                  = 0x0081;

struct READREG_CMD {
    CMD_HEADER header;
    quint8 addresses[];
};

struct READREG_ACK {
    ACK_HEADER header;
    quint8 registerData[];
};


template <class T>
class QList;

class ReadregCmdHelper : public HeaderCmdHelper
{
    READREG_CMD * const cmd;
public:
    ReadregCmdHelper(quint16 length);
    void setAddresses(const QList<quint32> &addresses);

    static QList<quint32> addresses(const READREG_CMD &cmd);
};


class ReadregAckHelper : public HeaderAckHelper
{
    READREG_ACK * const ack;
public:
    ReadregAckHelper(quint16 lenght);
    void setAnswers(const QList<quint32> &answers);

    static QList<quint32> aswers(const READREG_ACK &ack);

};


#endif // READREGHELPER_H
