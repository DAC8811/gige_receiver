QT       += core gui \
            network \
            widgets \
            xml \
            dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 \
          link_pkgconfig

PKGCONFIG += glib-2.0 \
            gstreamer-1.0

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DiscoveryPage.cpp \
    NetworkPage.cpp \
    defaultbackend/defaultbackend.cpp \
    gvcp/bootstrapregisters.cpp \
    gvcp/broadcastudpsocket.cpp \
    gvcp/discoveryhelper.cpp \
    gvcp/forceiphelper.cpp \
    gvcp/gvcpclient.cpp \
    gvcp/gvcpdiscoverer.cpp \
    gvcp/gvcpserver.cpp \
    gvcp/headerhelper.cpp \
    gvcp/packethelper.cpp \
    gvcp/readmemhelper.cpp \
    gvcp/readreghelper.cpp \
    gvcp/writememhelper.cpp \
    gvcp/writereghelper.cpp \
    gvsp/gvsp.cpp \
    gvsp/gvspblock.cpp \
    gvsp/gvsppacket.cpp \
    gvsp/gvspreceiver.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DiscoveryPage.h \
    NetworkPage.h \
    defaultbackend/defaultbackend.h \
    defaultbackend/defaultbackend_p.h \
    gvcp/bootstrapregisters.h \
    gvcp/bootstrapregisters_p.h \
    gvcp/broadcastudpsocket.h \
    gvcp/discoveryhelper.h \
    gvcp/forceiphelper.h \
    gvcp/gvcp.h \
    gvcp/gvcpclient.h \
    gvcp/gvcpclient_p.h \
    gvcp/gvcpdiscoverer.h \
    gvcp/gvcpdiscoverer_p.h \
    gvcp/gvcpserver.h \
    gvcp/gvcpserver_p.h \
    gvcp/headerhelper.h \
    gvcp/packethelper.h \
    gvcp/readmemhelper.h \
    gvcp/readreghelper.h \
    gvcp/writememhelper.h \
    gvcp/writereghelper.h \
    gvsp/constmemoryblock.h \
    gvsp/gvsp.h \
    gvsp/gvspblock.h \
    gvsp/gvsppacket.h \
    gvsp/gvspreceiver.h \
    gvsp/gvspreceiver_p.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

QMAKE_CXXFLAGS += -fopenmp

LIBS += -fopenmp
        glib \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

