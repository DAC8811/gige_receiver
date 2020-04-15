/***************************************************************************
 *   Copyright (C) 2014-2015 by Cyril BALETAUD                                  *
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

#ifndef GEVIPORT_H
#define GEVIPORT_H

#include "../genicam/iport.h"
#include <QScopedPointer>

class QHostAddress;
class GevIPortPrivate;
class GevIPort : public JgvIport::Interface
{
public:
    GevIPort();
    virtual ~GevIPort();

    bool connectAsController(const QHostAddress &controler, const QHostAddress &device);
    bool connectAsMonitor(const QHostAddress &monitor, const QHostAddress &device);
    void addReceiver(int channel, const QHostAddress &address, quint16 port);
    void stopReceiver(int channel);
    void SetPacketSize(int channel,uint Size);
    void SetPacketDelay(int channel,uint Delay);

    void read(quint8 *pBuffer, qint64 address, qint64 length);
    void write(quint8 *pBuffer, qint64 address, qint64 length);


private:
    const QScopedPointer<GevIPortPrivate> d_ptr;
    Q_DECLARE_PRIVATE(GevIPort)
    Q_DISABLE_COPY(GevIPort)

}; // GevIPort


#endif // GEVIPORT_H
