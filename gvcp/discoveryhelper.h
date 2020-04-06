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

#ifndef DISCOVERYHELPER_H
#define DISCOVERYHELPER_H


#include "headerhelper.h"

struct DISCOVERY_CMD
{
    CMD_HEADER header;
};

struct DISCOVERY_ACK
{
    ACK_HEADER header;
    quint8 specVersionMajor[2];
    quint8 specVersionMinor[2];
    quint8 deviceMode[4];
    quint8 reserved1[2];
    quint8 deviceMACAddressHigh[2];
    quint8 deviceMACAddressLow[4];
    quint8 ipConfigOptions[4];
    quint8 ipConfigCurrent[4];
    quint8 reserved2[12];
    quint8 currentIP[4];
    quint8 reserved3[12];
    quint8 currentSubnetMask[4];
    quint8 reserved4[12];
    quint8 defaultGateway[4];
    char manufacturerName[32];
    char modelName[32];
    char deviceVersion[32];
    char manufacturerSpecificInformation[48];
    char serialNumber[16];
    char userDefinedName[16];
};



const quint16 GVCP_CMD_DISCOVERY                = 0x0002;
const quint16 GVCP_ACK_DISCOVERY                = 0x0003;
const int DISCOVERY_CMD_LENGTH = sizeof(DISCOVERY_CMD) - sizeof(CMD_HEADER);
const int DISCOVERY_ACK_LENGTH = sizeof(DISCOVERY_ACK) - sizeof(ACK_HEADER);

class DiscoveryCmdHelper : public HeaderCmdHelper
{
    DISCOVERY_CMD * const cmd;
public:
    DiscoveryCmdHelper(quint16 length);
    void allowBroadcastAck(bool allow);


};


class DiscoveryAckHelper : public HeaderAckHelper
{
    DISCOVERY_ACK * const ack;
public:
    DiscoveryAckHelper(quint16 length);
    void setSpecVersion(quint32 version);
    void setDeviceMode(quint32 mode);
    void setDeviceMACaddressHigh(quint16 mac);
    void setDeviceMACaddressLow(quint32 mac);
    void setIPconfigOptions(quint32 options);
    void setIPconfigCurrent(quint32 current);
    void setCurrentIP(quint32 IP);
    void setCurrentSubnetMask(quint32 mask);
    void setDefaultGateway(quint32 gateway);
    void setManufacturerName(const char *name);
    void setModelName(const char *name);
    void setDeviceVersion(const char *name);
    void setManufacturerInformation(const char *name);
    void setSerialNumber(const char *name);
    void setUserName(const char *name);

    static quint16 specVersionMajor(const DISCOVERY_ACK &ack);
    static quint16 specVersionMinor(const DISCOVERY_ACK &ack);
    static quint32 deviceMode(const DISCOVERY_ACK &ack);
    static int deviceClass(quint32 deviceMode);
    static quint16 deviceMACaddressHigh(const DISCOVERY_ACK &ack);
    static quint32 deviceMACaddressLow(const DISCOVERY_ACK &ack);
    static quint32 IPconfigOptions(const DISCOVERY_ACK &ack);
    static quint32 IPconfigCurrent(const DISCOVERY_ACK &ack);
    static quint32 currentIP(const DISCOVERY_ACK &ack);
    static quint32 currentSubnetMask(const DISCOVERY_ACK &ack);
    static quint32 defaultGateway(const DISCOVERY_ACK &ack);
    static const char *manufacturerName(const DISCOVERY_ACK &ack);
    static const char *modelName(const DISCOVERY_ACK &ack);
    static const char *deviceVersion(const DISCOVERY_ACK &ack);
    static const char *manufacturerSpecificInformation(const DISCOVERY_ACK &ack);
    static const char *serialNumber(const DISCOVERY_ACK &ack);
    static const char *userDefinedName(const DISCOVERY_ACK &ack);
};


#endif // READREGHELPER_H
