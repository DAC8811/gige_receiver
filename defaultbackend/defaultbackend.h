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

#ifndef DEFAULTBACKEND_H
#define DEFAULTBACKEND_H

#include <QObject>
#include <QScopedPointer>

class QWidget;
class QToolBar;
class QDockWidget;
template <class T>
class QList;
template <class T>
class QSharedPointer;
class GvspClient;

class BackendWidgets {
public:
    QWidget *main;
    QToolBar *toolbar;
    QList<QDockWidget *> configurations;
};



class DefaultBackendPrivate;
class DefaultBackend : public QObject
{
    Q_OBJECT

public:
    explicit DefaultBackend(QObject *parent = 0);
    virtual ~DefaultBackend();

    virtual QSharedPointer<GvspClient> gvspClient();
    virtual BackendWidgets widgets();

protected:
    DefaultBackend(DefaultBackendPrivate &dd, QObject *parent);
    const QScopedPointer<DefaultBackendPrivate> d_ptr;

private:
    Q_DISABLE_COPY(DefaultBackend)
    Q_DECLARE_PRIVATE(DefaultBackend)

};

#endif // DEFAULTBACKEND_H
