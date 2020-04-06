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

#ifndef READMEMHELPER_H
#define READMEMHELPER_H


#include "headerhelper.h"

const quint16 GVCP_CMD_READMEM                  = 0x0084;
const quint16 GVCP_ACK_READMEM                  = 0x0085;

struct READMEM_CMD {
    CMD_HEADER header;
    quint8 address[4];
    quint8 reserved[2];
    quint8 count[2];
};

struct READMEM_ACK {
    ACK_HEADER header;
    quint8 address[4];
    quint8 datas[];
};


template <class T>
class QList;

const int READMEM_CMD_LENGTH = sizeof(READMEM_CMD) - sizeof(CMD_HEADER);

class ReadmemCmdHelper : public HeaderCmdHelper
{
    READMEM_CMD * const cmd;
public:
    ReadmemCmdHelper(quint32 length);
    void setAddress(quint32 address);
    void setDataCount(quint16 dataCount);

    static quint32 address(const READMEM_CMD &cmd);
    static quint16 dataCount(const READMEM_CMD &cmd);
};


class ReadmemAckHelper : public HeaderAckHelper
{
    READMEM_ACK * const ack;
public:
    ReadmemAckHelper(quint16 lenght);
    void setAddress(quint32 address);
    void addMem(const quint8 *mem, uint m_size);

    static quint32 address(const READMEM_ACK &ack);
    static const quint8 *dataPtr(const READMEM_ACK &ack);

};


#endif // READMEMHELPER_H
