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
#include <opencv2/opencv.hpp>

GvspBlock::GvspBlock(uint num, uint width, uint height, quint32 pixelFormat,quint32 type)
    : //GvspImage(width, height, pixelFormat, (width * height * GVSP_PIX_PIXEL_SIZE(pixelFormat)) / 8),
     // GvspImage(width, height, pixelFormat, width * height * pixelFormat,type),//原本这里pixelFormat应该指的是像素格式，这里直接换成opencv格式中的深度，即每个像素占的字节数
      num(num),
      segmentSize(0),
      lastIndex(1),
      lastSegmentSize(0),
      datas(NULL,width * height * pixelFormat),
      type(type),
      width(width),
      height(height)

{}

void GvspBlock::insert(quint16 segNum, const ConstMemoryBlock &mem)
{
    // 包0应该是leader，所以出现包0是有问题的
    if (Q_UNLIKELY(segNum == 0)) {
        qWarning("Buffer index == 0");
    }
    //  用前5个包来计算总的包个数以及每个标准包的数据长度
    else if (segNum<=5 && segmentSize==0)
    {
        segmentSize = mem.size;
        lastIndex = (datas.size%segmentSize == 0) ? datas.size/segmentSize : 1+ datas.size/segmentSize;
        lastSegmentSize = datas.size%segmentSize;
        std::memcpy(datas.p + ((segNum-1) * segmentSize), mem.data, segmentSize);
    }
    // 标准包
    else if (segNum < lastIndex && Q_LIKELY(mem.size == segmentSize)) {
        std::memcpy(datas.p + ((segNum-1) * segmentSize), mem.data, segmentSize);
    }
    // 最后一个包
    else if (segNum == lastIndex && Q_LIKELY(mem.size <= segmentSize)) {
        // le device peut faire du padding, la dernière taille peut
        // être de la taille normale
        std::memcpy(datas.p + ((segNum-1) * segmentSize), mem.data, lastSegmentSize);
    }
    else {
        qWarning("GvspBlock %d insert error segNum:%u lastIndex = %d segmentSize=%d mem.size=%d", num, segNum,lastIndex,segmentSize,mem.size);
    }
}

void GvspBlock::allocate()
{
    this->datas.p = new quint8[this->datas.size];
}
void GvspBlock::push()
{
    cv::Mat file(this->height, this->width, this->type, this->datas.p);
    cv::imwrite("/home/ash-1/qt_pj/data_save/test3.jpg",file);
    qWarning("ok");
}
void GvspBlock::trash()
{
    delete [] this->datas.p;
}

