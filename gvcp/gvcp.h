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

#ifndef GVCP_H
#define GVCP_H


#include "bootstrapregisters.h"
#include "headerhelper.h"
#include "discoveryhelper.h"
#include "readreghelper.h"
#include "writereghelper.h"
#include "readmemhelper.h"
#include "writememhelper.h"
#include "forceiphelper.h"
#include "gvcpdiscoverer.h"

#include <QString>

const int GVCP_PORT = 3956;




const quint16 GVCP_CMD_PACKETRESEND             = 0x0040;



const quint16 GVCP_ACK_PENDING                  = 0x0089;

namespace STATUS
{
const quint16 SUCCESS                                 = 0x0000;
const quint16 PACKET_RESEND                           = 0x0100;
const quint16 NOT_IMPLEMENTED                         = 0x8001;
const quint16 INVALID_PARAMETER                       = 0x8002;
const quint16 INVALID_ADDRESS                         = 0x8003;
const quint16 WRITE_PROTECT                           = 0x8004;
const quint16 BAD_ALIGNMENT                           = 0x8005;
const quint16 ACCESS_DENIED                           = 0x8006;
const quint16 PACKET_UNAVAILABLE                      = 0x800C;
const quint16 DATA_OVERUN                             = 0x800D;
const quint16 INVALID_HEADER                          = 0x800E;
const quint16 PACKET_NOT_YET_AVAILABLE                = 0x8010;
const quint16 PACKET_AND_PREV_REMOVED_FROM_MEMORY     = 0x8011;
const quint16 PACKET_REMOVED_FROM_MEMORY              = 0x8012;
const quint16 NO_REF_TIME                             = 0x8013;
const quint16 PACKET_TEMPORARILY_UNAVAILABLE          = 0x0814;
const quint16 STATUS_OVERFLOW                         = 0x0815;
const quint16 ACTION_LATE                             = 0x0816;
const quint16 ERROR                                   = 0x8FFF;
}

class Status {
public:
    static QString toString(quint16 status) {
        switch (status) {
        case STATUS::SUCCESS: return "Succes";
        case STATUS::PACKET_RESEND: return "Packet resend";
        case STATUS::NOT_IMPLEMENTED: return "Not implemented";
        case STATUS::INVALID_PARAMETER: return "Invalid parameter";
        case STATUS::INVALID_ADDRESS: return "Invalid address";
        case STATUS::WRITE_PROTECT: return "Write protect";
        case STATUS::BAD_ALIGNMENT: return "Bad alignement";
        case STATUS::ACCESS_DENIED: return "Acces denied";
        case STATUS::PACKET_UNAVAILABLE: return "Packet unavailable";
        case STATUS::DATA_OVERUN: return "Data overun";
        case STATUS::INVALID_HEADER: return "Invalid header";
        case STATUS::PACKET_NOT_YET_AVAILABLE: return "Packet not yet available";
        case STATUS::PACKET_AND_PREV_REMOVED_FROM_MEMORY: return "Packet and prev removed from memory";
        case STATUS::PACKET_REMOVED_FROM_MEMORY: return "Packet removed from memory";
        case STATUS::NO_REF_TIME: return "No ref time";
        case STATUS::PACKET_TEMPORARILY_UNAVAILABLE: return "packet temporarily unavailable";
        case STATUS::STATUS_OVERFLOW: return "Status overflow";
        case STATUS::ACTION_LATE: return "Action late";
        case STATUS::ERROR: return "Error";
        default: return "Unknow error";
        }
    }
};





#endif // GVCP_H

