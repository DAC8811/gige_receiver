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

#ifndef GVCPCLIENT_H
#define GVCPCLIENT_H

#include <QObject>

template <class T>
class QList;
template <class T, class U>
class QPair;

class QHostAddress;

typedef QList<quint32> AddressList;
typedef QList<quint32> ValuesList;
typedef QPair<quint32, quint32> AddressValue;
typedef QList<AddressValue> AdressValueList;



namespace CCP {
enum Privilege {
    OpenAcces,
    ControlAcces,
    ControlAccesWithSwitchoverEnabled,
    ExclusiveAcces
};
}

class GvcpClientPrivate;
class GvcpClient : public QObject
{
    Q_OBJECT

public:
    explicit GvcpClient(QObject *parent = 0);
    virtual ~GvcpClient();

    bool connectTo(const QHostAddress &client, const QHostAddress &server, CCP::Privilege privilege = CCP::ControlAcces);

    ValuesList readRegisters(const AddressList &addresses);
    bool writeRegisters(const AdressValueList &values);

    const quint8 *readMemory(quint32 address, quint16 count);
    bool writeMemory(quint32 address, const quint8 *data, uint size);


protected:
    const QScopedPointer<GvcpClientPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(GvcpClient)
    Q_DISABLE_COPY(GvcpClient)

private slots:
    void heartbeat();


};

#endif // GVCPCLIENT_H
