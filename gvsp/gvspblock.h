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

#ifndef GVSPBLOCK_H
#define GVSPBLOCK_H

#include "gvsp.h"

class ConstMemoryBlock;

class GvspBlock : public GvspImage
{
public:
    explicit GvspBlock(uint num, uint width, uint height, quint32 pixelFormat);
    void insert(quint16 segNum, const ConstMemoryBlock &mem);

    const uint num;                  // le numéro de bloc
private:
    size_t segmentSize;       // la taille d'un segment de données
    uint lastIndex;            // le dernier index des segments
    uint lastSegmentSize;      // la taille du dernier segment

};



#endif // GVSPBLOCK_H
