/*
 * Copyright (C) 2019 Theia Space, Universidad Politécnica de Madrid
 *
 * This file is part of Theia Space's ESAT Wifi library.
 *
 * Theia Space's ESAT Wifi library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Theia Space's ESAT Wifi library is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Theia Space's ESAT Wifi library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ESAT_Wifi-telemetry/ESAT_WifiNetworkInformationTelemetry.h"
#include "ESAT_Wifi-hardware/ESAT_WifiRadio.h"

boolean ESAT_WifiNetworkInformationTelemetryClass::available()
{
  return true;
}

boolean ESAT_WifiNetworkInformationTelemetryClass::fillUserData(ESAT_CCSDSPacket& packet)
{
  writeIsConnected(packet);
  writeMACAddress(packet);
  writeSSID(packet);
  writeLocalIPAddress(packet);
  writeSubnetMask(packet);
  writeGatewayIPAddress(packet);
  writeDNSIPAddresses(packet);
  return true;
}

void ESAT_WifiNetworkInformationTelemetryClass::writeDNSIPAddresses(ESAT_CCSDSPacket& packet)
{
  const byte dnsIPAddresses = 2;
  for (int dns = 0; dns < dnsIPAddresses; dns = dns + 1)
  {
    writeIPAddress(WiFi.dnsIP(dns),
                   packet);
  }
}

void ESAT_WifiNetworkInformationTelemetryClass::writeGatewayIPAddress(ESAT_CCSDSPacket& packet)
{
  writeIPAddress(WiFi.gatewayIP(),
                 packet);
}

void ESAT_WifiNetworkInformationTelemetryClass::writeIPAddress(const IPAddress address,
                                                               ESAT_CCSDSPacket& packet)
{
  const byte ipAddressLength = 4;
  for (int index = 0; index < ipAddressLength; index = index + 1)
  {
    packet.writeByte(address[index]);
  }
}

void ESAT_WifiNetworkInformationTelemetryClass::writeIsConnected(ESAT_CCSDSPacket& packet)
{
  packet.writeBoolean(WiFi.isConnected());
}

void ESAT_WifiNetworkInformationTelemetryClass::writeLocalIPAddress(ESAT_CCSDSPacket& packet)
{
  writeIPAddress(WiFi.localIP(),
                 packet);
}

void ESAT_WifiNetworkInformationTelemetryClass::writeMACAddress(ESAT_CCSDSPacket& packet)
{
  const byte macAddressLength = 6;
  byte mac[macAddressLength];
  WiFi.macAddress(mac);
  for (int index = 0; index < macAddressLength; index = index + 1)
  {
    packet.writeByte(mac[index]);
  }
}

void ESAT_WifiNetworkInformationTelemetryClass::writeSSID(ESAT_CCSDSPacket& packet)
{
  const byte ssidLength = 32;
  const String ssid = WiFi.SSID();
  for (int index = 0; index < ssidLength; index = index + 1)
  {
    packet.writeChar(ssid.charAt(index));
  }
}

void ESAT_WifiNetworkInformationTelemetryClass::writeSubnetMask(ESAT_CCSDSPacket& packet)
{
  writeIPAddress(WiFi.subnetMask(),
                 packet);
}


ESAT_WifiNetworkInformationTelemetryClass ESAT_WifiNetworkInformationTelemetry;
