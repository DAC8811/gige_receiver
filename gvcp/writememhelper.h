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

#ifndef WRITEMEMHELPER_H
#define WRITEMEMHELPER_H


#include "headerhelper.h"

const quint16 GVCP_CMD_WRITEMEM                 = 0x0086;
const quint16 GVCP_ACK_WRITEMEM                 = 0x0087;

struct WRITEMEM_CMD {
    CMD_HEADER header;
    quint8 address[4];
    quint8 data[];
};

struct WRITEMEM_ACK {
    ACK_HEADER header;
    quint8 reserved[2];
    quint8 index[2];
};

class WritememCmdHelper : public HeaderCmdHelper
{
    WRITEMEM_CMD * const cmd;
public:
    WritememCmdHelper(quint16 length);
    void setAddress(quint32 address);
    void setCmdData(const quint8 *datas, uint m_size);

    static quint32 address(const WRITEMEM_CMD &cmd);
    static quint16 count(const WRITEMEM_CMD &cmd);
    static const quint8 *cmdData(const WRITEMEM_CMD &cmd);
};

const int WRITEMEM_ACK_LENGTH = sizeof(WRITEMEM_ACK) - sizeof(ACK_HEADER);

class WritememAckHelper : public HeaderAckHelper
{
    WRITEMEM_ACK * const ack;
public:
    WritememAckHelper(quint16 lenght);
    void setIndex(quint16 index);

};


#endif // WRITEMEMHELPER_H
