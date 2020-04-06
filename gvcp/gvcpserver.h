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

#ifndef GVCPSERVER_H
#define GVCPSERVER_H

#include <QObject>

class QNetworkInterface;
class QHostAddress;

struct READREG_CMD;

class GvcpServerPrivate;
class GvcpServer : public QObject
{
    Q_OBJECT

public:
    explicit GvcpServer(QObject *parent = 0);
    virtual ~GvcpServer();

    void listen(const QNetworkInterface &netInterface);

protected:
    const QScopedPointer<GvcpServerPrivate> d_ptr;
    GvcpServer(GvcpServerPrivate &dd, QObject *parent);

    virtual void readRegister(quint32 address, const QHostAddress &sender);
    virtual void writeRegister(quint32 address, quint32 value);

private slots:
    void readPendingDatagrams();
    void onHeartbeat();

private:
    Q_DECLARE_PRIVATE(GvcpServer)
    Q_DISABLE_COPY(GvcpServer)

};

#endif // GVCPSERVER_H
