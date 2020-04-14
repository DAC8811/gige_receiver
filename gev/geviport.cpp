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

#include "geviport.h"
#include "geviport_p.h"
#include "../gvcp/bootstrapregisters.h"

#include <QPair>
#include <QHostAddress>
#include <cstring>

GevIPort::GevIPort()
    : JgvIport::Interface(),
      d_ptr(new GevIPortPrivate)
{}

GevIPort::~GevIPort()
{
    qDebug("Deleting GEV Iport");
}

bool GevIPort::connectAsController(const QHostAddress &controler, const QHostAddress &device)
{
    Q_D(GevIPort);
    return d->port.connectTo(controler, device, CCP::ControlAcces);
}

bool GevIPort::connectAsMonitor(const QHostAddress &monitor, const QHostAddress &device)
{
    Q_D(GevIPort);
    return d->port.connectTo(monitor, device, CCP::OpenAcces);
}

void GevIPort::addReceiver(int channel, const QHostAddress &address, quint16 port)
{
    Q_D(GevIPort);
    d->port.writeRegisters(
                AdressValueList()
                << AddressValue(Register::StreamChannelDestinationAddress + channel * Register::StreamChannelBlockSize, address.toIPv4Address())
                << AddressValue(Register::StreamChannelPort + channel * Register::StreamChannelBlockSize, port));

}

void GevIPort::stopReceiver(int channel)
{
    Q_D(GevIPort);
    d->port.writeRegisters(
                 AdressValueList() << AddressValue(Register::StreamChannelPort + channel * Register::StreamChannelBlockSize, 0));
}

void GevIPort::read(quint8 *pBuffer, qint64 address, qint64 length)
{
    Q_D(GevIPort);
    const quint8 *memory = d->port.readMemory(address, length);
    if (memory != 0) {
        std::memcpy(pBuffer, memory, length);
    }

}

void GevIPort::write(quint8 *pBuffer, qint64 address, qint64 length)
{
    Q_D(GevIPort);
    d->port.writeMemory(address, pBuffer, length);
}
