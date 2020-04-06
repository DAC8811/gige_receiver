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

#ifndef BOOTSTRAPREGISTERS_H
#define BOOTSTRAPREGISTERS_H

#include <QScopedPointer>



namespace Register {
const quint32 Version                       = 0x0000;
const quint32 DeviceMode                    = 0x0004;
const quint32 DeviceMacAddressHigh_0        = 0x0008;
const quint32 DeviceMacAddressLow_0         = 0x000C;
const quint32 SupportedIPconfiguration_0    = 0x0010;
const quint32 CurrentIPconfiguration_0      = 0x0014;
const quint32 CurrentIPAddress_0            = 0x0024;
const quint32 CurrentSubnetMask_0           = 0x0034;
const quint32 CurrentDefaultGateway_0       = 0x0044;
// go to memory access
const quint32 NumberOfNetworkInterfaces     = 0x0600;
const quint32 PersistentIPAddress_0         = 0x064C;
const quint32 PersistentSubnetMask_0        = 0x065C;
const quint32 PersistentDefaultGateway_0    = 0x066C;
const quint32 LinkSpeed_0                   = 0x0670;
const quint32 DeviceMacAddressHigh_1        = 0x0680;
const quint32 DeviceMacAddressLow_1         = 0x0684;
const quint32 NetworkInterfaceCapability_1  = 0x0688;
const quint32 NetworkInterfaceConfiguration_1 = 0x068C;
const quint32 CurrentIPAddress_1            = 0x069C;
const quint32 CurrentSubnetMask_1           = 0x06AC;
const quint32 CurrentDefaultGateway_1       = 0x06BC;
const quint32 PersistentIPAddress_1         = 0x06CC;
const quint32 PersistentSubnetMask_1        = 0x06DC;
const quint32 PersistentDefaultGateway_1    = 0x06EC;
const quint32 LinkSpeed_1                   = 0x06F0;
const quint32 DeviceMacAddressHigh_2        = 0x0700;
const quint32 DeviceMacAddressLow_2         = 0x0704;
const quint32 NetworkInterfaceCapability_2  = 0x0708;
const quint32 NetworkInterfaceConfiguration_2 = 0x070C;
const quint32 CurrentIPAddress_2            = 0x071C;
const quint32 CurrentSubnetMask_2           = 0x072C;
const quint32 CurrentDefaultGateway_2       = 0x073C;
const quint32 PersistentIPAddress_2         = 0x074C;
const quint32 PersistentSubnetMask_2        = 0x075C;
const quint32 PersistentDefaultGateway_2    = 0x076C;
const quint32 LinkSpeed_2                   = 0x0770;
const quint32 DeviceMacAddressHigh_3        = 0x0780;
const quint32 DeviceMacAddressLow_3         = 0x0784;
const quint32 NetworkInterfaceCapability_3  = 0x0788;
const quint32 NetworkInterfaceConfiguration_3 = 0x078C;
const quint32 CurrentIPAddress_3            = 0x079C;
const quint32 CurrentSubnetMask_3           = 0x07AC;
const quint32 CurrentDefaultGateway_3       = 0x07BC;
const quint32 PersistentIPAddress_3         = 0x07CC;
const quint32 PersistentSubnetMask_3        = 0x07DC;
const quint32 PersistentDefaultGateway_3    = 0x07EC;
const quint32 LinkSpeed_3                   = 0x07F0;
const quint32 NumberOfMessageChannels       = 0x0900;
const quint32 NumberOfStreamChannel         = 0x0904;
const quint32 NumberOfActionSignals         = 0x0908;
const quint32 ActionDeviceKey               = 0x090C;
const quint32 NumberOfActiveLinks           = 0x0910;
const quint32 GVSPCapability                = 0x092C;
const quint32 MessageChannelCapability      = 0x0930;
const quint32 GVCPCapability                = 0x0934;
const quint32 HeartbeatTimeout              = 0x0938;
const quint32 TimestampTickFrequencyHigh    = 0x093C;
const quint32 TimestampTickFrequencyLow     = 0x0940;
const quint32 TimestampControl              = 0x0944;
const quint32 TimestampValueHigh            = 0x0948;
const quint32 TimestampValueLow             = 0x094C;
const quint32 DiscoveryACKDelay             = 0x0950;
const quint32 GVCPConfiguration             = 0x0954;
const quint32 PendingTimeout                = 0x0958;
const quint32 ControlSwitchoverKey          = 0x095C;
const quint32 GVSPConfiguration             = 0x0960;
const quint32 PhysicalLinkConfigurationCapability = 0x0964;
const quint32 PhysicalLinkConfiguration     = 0x0968;
const quint32 IEEE1588Status                = 0x096C;
const quint32 SheduledActionCommandQueueSize = 0x0970;
const quint32 ControlChannelPrivilege       = 0x0A00;
const quint32 PrimaryApplicationPort        = 0x0A04;
const quint32 PrimaryApplicationIPAddress   = 0x0A14;
const quint32 MessageChannelPort            = 0x0B00;
const quint32 MessageChannelDestinationAddress = 0x0B10;
const quint32 MessageChannelTransmissionTimeout = 0x0B14;
const quint32 MessageChannelRetryCount      = 0x0B18;
const quint32 MessageChannelSourcePort      = 0x0B1C;
// chaque canal stream possède ce schéma et prend 64 octets (0x40)
const int StreamChannelBlockSize = 0x40;
const quint32 StreamChannelPort             = 0x0D00;
const quint32 StreamChannelPacketSize       = 0x0D04;
const quint32 StreamChannelPacketDelay      = 0x0D08;
const quint32 StreamChannelDestinationAddress = 0x0D18;
const quint32 StreamChannelSourcePort       = 0x0D1C;
const quint32 StreamChannelCapability       = 0x0D20;
const quint32 StreamChannelConfiguration    = 0x0D24;
const quint32 StreamChannelZone             = 0x0D28;
const quint32 StreamChannelZoneDirection    = 0x0D2C;
// ---------------------------------------------------------------
// chaque action group possède ce schéma et prend 16 octets (0x10)
const quint32 ActionGroupKey                = 0x9800;
const quint32 ActionGroupMask               = 0x9814;
// ---------------------------------------------------------------
const quint32 SpecificManufacturerRegister = 0xA000;
}

namespace MemoryBlocs {
const quint32 ManufacturerName              = 0x0048;
const int ManufacturerNameSize = 32;
const quint32 ModelName                     = 0x0068;
const int ModelNameSize = 32;
const quint32 DeviceVersion                 = 0x0088;
const int DeviceVersionSize = 32;
const quint32 ManufacturerInfo              = 0x00A8;
const int ManufacturerInfoSize = 48;
const quint32 SerialNumber                  = 0x00D8;
const int SerialNumberSize = 16;
const quint32 UserDefinedName               = 0x00E8;
const int UserDefinedNameSize = 16;
const quint32 FirstURL                      = 0x0200;
const int FirstURLSize = 512;
const quint32 SecondURL                     = 0x0400;
const int SecondURLSize = 512;
const quint32 ManifestTable                 = 0x9000;
const int ManifestTableSize = 512;
}

namespace DeviceClass {
const quint8 TRANSMITTER = 0;
const quint8 RECEIVER = 1;
const quint8 TRANSCEIVER = 2;
const quint8 PERIPHERAL = 3;
}
namespace Endianness {
const quint8 LITTLE = 0;
const quint8 BIG = 1;
}
namespace CharacterSetIndex {
const quint8 UTF8 = 1;
}

const quint32 OUT_OF_RANGE_ADDRESS = 0xFFFFFFFFu;


template <class T, class U>
class QPair;
typedef QPair<quint16, quint8 *> WritememResult;
typedef QPair<quint16, const quint8 *> ReadmemResult;

class QNetworkInterface;
class BootstrapRegistersPrivate;
class BootstrapRegisters
{

public:
    BootstrapRegisters();
    ~BootstrapRegisters();
    void setNetworkInterface(const QNetworkInterface &netInterface);

    quint32 xmlAddress() const;
    void swapXmlFile(const QString &newFile);

    /*!
     * \brief addressIsValid
     * Contrôle que l'adresse est une adresse de base de registre.
     * \param address l'adresse à tester.
     * \return true si l'adresse est une adresse de base, false dans le cas contraire.
     */
    bool addressIsValid(quint32 address) const;

    /*!
     * \brief length
     * Obtient le nombre d'octet contenu par l'adresse de base.
     * \param address L'adresse de base du registre
     * \return le nombre d'octets contenus à cette adresse.
     */
    uint length(quint32 address) const;

    void extendRegisters(quint32 address, quint32 value);

    quint32 value(quint32 address) const;
    quint16 setValue(quint32 address, quint32 value);
    ReadmemResult readmemPtr(quint32 address, uint length) const;
    WritememResult writememPtr(quint32 address, uint length) const;

    const char *constBlock(quint32 address) const;
    static quint32 align(quint32 val);
private:
    const QScopedPointer<BootstrapRegistersPrivate> d;

};


#endif // BOOTSTRAPREGISTERS_H
