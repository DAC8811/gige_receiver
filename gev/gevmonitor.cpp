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

#include "gevmonitor.h"
#include "gevmonitor_p.h"

#include "../gvcp/bootstrapregisters.h"


GevMonitor::GevMonitor()
    : d(new GevMonitorPrivate)
{}

GevMonitor::~GevMonitor()
{}

bool GevMonitor::connectTo(const QHostAddress &monitor, const QHostAddress &device)
{
    return d->port.connectTo(monitor, device, CCP::OpenAcces);
}

QByteArray GevMonitor::getFirstUrl()
{
    return QByteArray(reinterpret_cast<const char *>(d->port.readMemory(MemoryBlocs::FirstURL, MemoryBlocs::FirstURLSize)),
                      MemoryBlocs::FirstURLSize);
}

QByteArray GevMonitor::getXml(const QByteArray &url)
{
    QByteArray content;
    if (url.startsWith("Local:")) {
        QList<QByteArray> firstURLFields = url.split(';');
        if (firstURLFields.count() == 3) {
            bool ok;
            quint32 address = firstURLFields.at(1).toUInt(&ok, 16);
            if (ok) {
                quint32 size = firstURLFields.at(2).toUInt(&ok, 16);
                if (ok) {
                    // le payload max
                    const int MEMREADPayload = 0x200;
                    // le nombre de packets identiques
                    const int packetCount = size / MEMREADPayload;
                    // la taille du dernier packet
                    const int lastPacketSize = BootstrapRegisters::align(size - (packetCount * MEMREADPayload));
                    // obtient tous les packets de taille identique
                    for (int i=0; i<packetCount; ++i)  {
                        content.append(reinterpret_cast<const char *>(d->port.readMemory(address + i * MEMREADPayload, MEMREADPayload)),
                                       MEMREADPayload);
                    }
                    // fini avec le dernier packet
                    content.append(reinterpret_cast<const char *>(d->port.readMemory(address + packetCount * MEMREADPayload, lastPacketSize))
                                   , lastPacketSize);
                }
            }
        }
    }
    return content;
}

QString GevMonitor::filenameFromFirstURL(const QByteArray &firstURL)
{
    const QByteArray begin = "Local:";
    QByteArray fileName;
    if (firstURL.startsWith(begin)) {
        QList<QByteArray> firstURLFields = firstURL.split(';');
        if (firstURLFields.count() == 3) {
            fileName = firstURLFields[0].remove(0, begin.length());
        }
    }
    return fileName;
}

