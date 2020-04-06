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

#include "discoveryhelper.h"

#include <QByteArray>
#include <QString>
#include <QtEndian>

DiscoveryCmdHelper::DiscoveryCmdHelper(quint16 length)
    : HeaderCmdHelper(GVCP_CMD_DISCOVERY, length),
      cmd(reinterpret_cast<DISCOVERY_CMD * const>(m_data))
{}

void DiscoveryCmdHelper::allowBroadcastAck(bool allow)
{
    cmd->header.flag = allow ? (0x10u | cmd->header.flag) : (0xEFu & cmd->header.flag);
}


DiscoveryAckHelper::DiscoveryAckHelper(quint16 length)
    : HeaderAckHelper(GVCP_ACK_DISCOVERY, length),
      ack(reinterpret_cast<DISCOVERY_ACK * const>(m_data))
{}

void DiscoveryAckHelper::setSpecVersion(quint32 version)
{
    qToBigEndian<quint32>(version, ack->specVersionMajor);
}

void DiscoveryAckHelper::setDeviceMode(quint32 mode)
{
    qToBigEndian<quint32>(mode, ack->deviceMode);
}

void DiscoveryAckHelper::setDeviceMACaddressHigh(quint16 mac)
{
    qToBigEndian<quint16>(mac, ack->deviceMACAddressHigh);
}

void DiscoveryAckHelper::setDeviceMACaddressLow(quint32 mac)
{
    qToBigEndian<quint32>(mac, ack->deviceMACAddressLow);
}

void DiscoveryAckHelper::setIPconfigOptions(quint32 options)
{
    qToBigEndian<quint32>(options, ack->ipConfigOptions);
}

void DiscoveryAckHelper::setIPconfigCurrent(quint32 current)
{
    qToBigEndian<quint32>(current, ack->ipConfigCurrent);
}

void DiscoveryAckHelper::setCurrentIP(quint32 IP)
{
    qToBigEndian<quint32>(IP, ack->currentIP);
}

void DiscoveryAckHelper::setCurrentSubnetMask(quint32 mask)
{
    qToBigEndian<quint32>(mask, ack->currentSubnetMask);
}

void DiscoveryAckHelper::setDefaultGateway(quint32 gateway)
{
    qToBigEndian<quint32>(gateway, ack->defaultGateway);
}

//void DiscoveryAckHelper::setManufacturerName(const QString &name)
//{
//    QByteArray n = name.left(32).toUtf8();
//    memcpy(ack.manufacturerName, n.constData(), n.size());
//}

void DiscoveryAckHelper::setManufacturerName(const char *name)
{
    std::memcpy(ack->manufacturerName, name, sizeof(ack->manufacturerName));
}

void DiscoveryAckHelper::setModelName(const char *name)
{
    std::memcpy(ack->modelName, name, sizeof(ack->modelName));
}

void DiscoveryAckHelper::setDeviceVersion(const char *name)
{
    std::memcpy(ack->deviceVersion, name, sizeof(ack->deviceVersion));
}

void DiscoveryAckHelper::setManufacturerInformation(const char *name)
{
    std::memcpy(ack->manufacturerSpecificInformation, name, sizeof(ack->manufacturerSpecificInformation));
}

void DiscoveryAckHelper::setSerialNumber(const char *name)
{
    std::memcpy(ack->serialNumber, name, sizeof(ack->serialNumber));
}

void DiscoveryAckHelper::setUserName(const char *name)
{
    std::memcpy(ack->userDefinedName, name, sizeof(ack->userDefinedName));
}

quint16 DiscoveryAckHelper::specVersionMajor(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint16>(ack.specVersionMajor);
}

quint16 DiscoveryAckHelper::specVersionMinor(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint16>(ack.specVersionMinor);
}

quint32 DiscoveryAckHelper::deviceMode(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint32>(ack.deviceMode);
}

int DiscoveryAckHelper::deviceClass(quint32 deviceMode)
{
    return (0x70000000u & deviceMode) >> 24;
}

quint16 DiscoveryAckHelper::deviceMACaddressHigh(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint16>(ack.deviceMACAddressHigh);
}

quint32 DiscoveryAckHelper::deviceMACaddressLow(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint32>(ack.deviceMACAddressLow);
}

quint32 DiscoveryAckHelper::IPconfigOptions(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint32>(ack.ipConfigOptions);
}

quint32 DiscoveryAckHelper::IPconfigCurrent(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint32>(ack.ipConfigCurrent);
}

quint32 DiscoveryAckHelper::currentIP(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint32>(ack.currentIP);
}

quint32 DiscoveryAckHelper::currentSubnetMask(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint32>(ack.currentSubnetMask);
}

quint32 DiscoveryAckHelper::defaultGateway(const DISCOVERY_ACK &ack)
{
    return qFromBigEndian<quint32>(ack.defaultGateway);
}

const char *DiscoveryAckHelper::manufacturerName(const DISCOVERY_ACK &ack)
{
    return reinterpret_cast<const char *>(ack.manufacturerName);
}

const char *DiscoveryAckHelper::modelName(const DISCOVERY_ACK &ack)
{
    return reinterpret_cast<const char *>(ack.modelName);
}

const char *DiscoveryAckHelper::deviceVersion(const DISCOVERY_ACK &ack)
{
    return reinterpret_cast<const char *>(ack.deviceVersion);
}

const char *DiscoveryAckHelper::manufacturerSpecificInformation(const DISCOVERY_ACK &ack)
{
    return reinterpret_cast<const char *>(ack.manufacturerSpecificInformation);
}

const char *DiscoveryAckHelper::serialNumber(const DISCOVERY_ACK &ack)
{
    return reinterpret_cast<const char *>(ack.serialNumber);
}

const char *DiscoveryAckHelper::userDefinedName(const DISCOVERY_ACK &ack)
{
    return reinterpret_cast<const char *>(ack.userDefinedName);
}


