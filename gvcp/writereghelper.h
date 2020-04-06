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

#ifndef WRITEREGHELPER_H
#define WRITEREGHELPER_H


#include "headerhelper.h"

const quint16 GVCP_CMD_WRITEREG                 = 0x0082;
const quint16 GVCP_ACK_WRITEREG                 = 0x0083;

template <class T>
class QList;

template <class T, class U>
class QPair;

typedef QList<QPair<quint32,quint32> > RegistersValueList;

struct WRITEREG_CMD {
    CMD_HEADER header;
    quint8 address_value[];
};

struct WRITEREG_ACK {
    ACK_HEADER header;
    uchar reserved[2];
    uchar index[2];
};

const int WRITEREG_ACK_LENGTH = sizeof(WRITEREG_ACK) - sizeof(ACK_HEADER);

class WriteregCmdHelper : public HeaderCmdHelper
{
    WRITEREG_CMD * const cmd;
public:
    WriteregCmdHelper(quint16 length);
    void setRegsValList(const RegistersValueList &values);

    static RegistersValueList regsValList(const WRITEREG_CMD &cmd);
};

class WriteregAckHelper : public HeaderAckHelper
{
    WRITEREG_ACK * const ack;
public:
    WriteregAckHelper(quint16 lenght);
    void setIndex(quint16 index);

};


#endif // WRITEREGHELPER_H
