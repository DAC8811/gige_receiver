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

#include "gvspblock.h"
#include "gvsppacket.h"
#include <sys/uio.h>
#include <cstring>

GvspBlock::GvspBlock(uint num, uint width, uint height, quint32 pixelFormat)
    : GvspImage(width, height, pixelFormat, (width * height * GVSP_PIX_PIXEL_SIZE(pixelFormat)) / 8),
      num(num),
      segmentSize(0),
      lastIndex(1),
      lastSegmentSize(0)
{}

void GvspBlock::insert(quint16 segNum, const ConstMemoryBlock &mem)
{
    // 没有段0
    if (Q_UNLIKELY(segNum == 0)) {
        qWarning("Buffer index == 0");
    }
    //  5个第一个细分，我们用它来修复细分数据
    else if (segNum<=5 && segmentSize==0)
    {
        segmentSize = mem.size;
        lastIndex = (datas.size%segmentSize == 0) ? datas.size/segmentSize : 1+ datas.size/segmentSize;
        lastSegmentSize = datas.size%segmentSize;
        std::memcpy(datas.p + ((segNum-1) * segmentSize), mem.data, segmentSize);
    }
    // 实心段
    else if (segNum < lastIndex && Q_LIKELY(mem.size == segmentSize)) {
        std::memcpy(datas.p + ((segNum-1) * segmentSize), mem.data, segmentSize);
    }
    // 最后一段的情况
    else if (segNum == lastIndex && Q_LIKELY(mem.size <= segmentSize)) {
        // le device peut faire du padding, la dernière taille peut
        // être de la taille normale
        std::memcpy(datas.p + ((segNum-1) * segmentSize), mem.data, lastSegmentSize);
    }
    else {
        qWarning("GvspBlock %d insert error segNum:%u lastIndex = %d segmentSize=%d mem.size=%d", num, segNum,lastIndex,segmentSize,mem.size);
    }
}

