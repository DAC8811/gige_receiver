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

#ifndef PACKETHELPER_H
#define PACKETHELPER_H

#include <QtGlobal>
#include <cstring>

const uint GVCP_PACKET_MAX_SIZE = 548;

class PacketHelper
{
protected:
    char m_data[GVCP_PACKET_MAX_SIZE];
    uint m_size;

public:
    PacketHelper(uint size = GVCP_PACKET_MAX_SIZE):m_size(size){std::memset(m_data,0,size);}
    const char *data() {return m_data;}
    uint size() {return m_size;}
};

#endif // PACKETHELPER_H
