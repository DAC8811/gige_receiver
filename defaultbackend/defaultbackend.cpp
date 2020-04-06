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

#include "defaultbackend.h"
#include "defaultbackend_p.h"
#include "../gvsp/gvsp.h"

#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QDockWidget>

class NullClient : public GvspClient
{
public:
    ~NullClient(){qDebug("Deleting NullClient");}
    void allocate(GvspImage &image){image.datas.p = new quint8[image.datas.size];}
    void push(GvspImage &image){delete[] image.datas.p;}
    void trash(GvspImage &image) const {delete[] image.datas.p;}
};

DefaultBackend::DefaultBackend(QObject *parent)
    : QObject(parent), d_ptr(new DefaultBackendPrivate)
{
    d_ptr->client = QSharedPointer<GvspClient>(new NullClient);
}

DefaultBackend::~DefaultBackend()
{
    qDebug("Deleting Default Backend");
}

QSharedPointer<GvspClient> DefaultBackend::gvspClient()
{
    Q_D(DefaultBackend);
    return d->client;
}

BackendWidgets DefaultBackend::widgets()
{
    BackendWidgets widgets;
    widgets.main = new QLabel(trUtf8("Default Backend"));
    widgets.toolbar = new QToolBar(trUtf8("Sortie par défaut"));
    QDockWidget *dock = new QDockWidget;
    dock->setWidget(new QLabel(trUtf8("Acune configuration")));
    dock->setVisible(false);
    widgets.configurations << dock;
    QAction *action = widgets.toolbar->addAction(QIcon("://ressources/icons/video-off.png"), trUtf8("Fake"));
    action->setCheckable(true);
    connect(action, SIGNAL(toggled(bool)), dock, SLOT(setVisible(bool)));
    return widgets;

}

DefaultBackend::DefaultBackend(DefaultBackendPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{}



