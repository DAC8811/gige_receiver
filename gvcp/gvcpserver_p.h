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

#ifndef GVCPSERVER_P_H
#define GVCPSERVER_P_H

#include <QUdpSocket>
#include <QScopedPointer>
#include <QTimer>

#include "bootstrapregisters.h"
#include "gvcp.h"



class GvcpServerPrivate
{
public:
    ~GvcpServerPrivate();
    QUdpSocket udp;
    BootstrapRegisters bootstrap;
    QTimer heartbeat;
    union {
        char packet[GVCP_PACKET_MAX_SIZE];
        quint8 packet_u[GVCP_PACKET_MAX_SIZE];
        CMD_HEADER headerCmd;
        ACK_HEADER headerAck;
        DISCOVERY_ACK discoveryAck;
        READREG_CMD readregCmd;
        READREG_ACK readregAck;
        WRITEREG_CMD writeregCmd;
        WRITEREG_ACK writeregAck;
        READMEM_CMD readmemCmd;
        WRITEMEM_CMD writememCmd;
    };
    

};

#endif // GVCPSERVER_P_H
