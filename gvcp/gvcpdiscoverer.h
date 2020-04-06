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

#ifndef GVCPDISCOVERER_H
#define GVCPDISCOVERER_H

#include <QObject>

class QHostAddress;
struct DISCOVERY_ACK;

class GvcpDiscovererPrivate;
class GvcpDiscoverer : public QObject
{
    Q_OBJECT

public:
    explicit GvcpDiscoverer(QObject *parent = 0);
    virtual ~GvcpDiscoverer();

    int sendDiscovery(const QHostAddress &src, const QHostAddress &dest, bool broadcastResponse = false);
    void forceIP(const QHostAddress &src, quint64 mac, quint32 newIP, quint32 newNetmask, quint32 newGateway);

signals:
    void newDeviceDiscovered(const DISCOVERY_ACK &device, int id);
    void forceIPsucces();

protected:
    const QScopedPointer<GvcpDiscovererPrivate> d;

private:
    Q_DISABLE_COPY(GvcpDiscoverer)

private slots:
    void readPendingDatagrams();

};

#endif // GVCPDISCOVERER_H
