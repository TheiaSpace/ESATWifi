/*
 * This file is part of Theia Space's ESAT Util library.
 *
 * Theia Space's ESAT Util library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Theia Space's ESAT Util library is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Theia Space's ESAT Util library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ESATWifi.h"
#include "ESATWifiConfiguration.h"

void ESATWifi::begin()
{
  WiFi.mode(WIFI_STA);
  WifiConfiguration.begin();
  WifiConfiguration.readConfiguration();
  Serial.begin(115200);
}

void ESATWifi::connectToNetwork()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    connectionState = CONNECTING_TO_SERVER;
  }
  else
  {
    (void) WiFi.begin(WifiConfiguration.networkSSID,
                      WifiConfiguration.networkPassphrase);
    connectionState = CONNECTING_TO_NETWORK;
  }
}

void ESATWifi::connectToServer()
{
  if (client.connect(WifiConfiguration.serverAddress,
                     WifiConfiguration.serverPort))
  {
    connectionState = CONNECTED;
  }
  else
  {
    connectionState = CONNECTING_TO_SERVER;
  }
}

void ESATWifi::disconnect()
{
  (void) WiFi.disconnect(true);
  connectionState = DISCONNECTED;
}

void ESATWifi::handleTelecommand(ESATCCSDSPacket& packet)
{
  if (packet.readPacketType() != packet.TELECOMMAND)
  {
    return;
  }
  if (packet.readApplicationProcessIdentifier()
      != APPLICATION_PROCESS_IDENTIFIER)
  {
    return;
  }
  if (packet.readPacketDataLength() < MINIMUM_TELECOMMAND_PACKET_DATA_LENGTH)
  {
    return;
  }
  const byte majorVersionNumber = packet.readByte();
  const byte minorVersionNumber = packet.readByte();
  const byte patchVersionNumber = packet.readByte();
  if (majorVersionNumber < MAJOR_VERSION_NUMBER)
  {
    return;
  }
  const byte commandCode = packet.readByte();
  switch (commandCode)
  {
    case CONNECT:
      handleConnectCommand(packet);
      break;
    case DISCONNECT:
      handleDisconnectCommand(packet);
      break;
    case SET_NETWORK_SSID:
      handleSetNetworkSSIDCommand(packet);
      break;
    case SET_NETWORK_PASSPHRASE:
      handleSetNetworkPassphraseCommand(packet);
      break;
    case SET_SERVER_ADDRESS:
      handleSetServerAddressCommand(packet);
      break;
    case SET_SERVER_PORT:
      handleSetServerPortCommand(packet);
      break;
    case READ_CONFIGURATION:
      handleReadConfigurationCommand(packet);
      break;
    case WRITE_CONFIGURATION:
      handleWriteConfigurationCommand(packet);
      break;
    default:
      break;
  }
}

void ESATWifi::handleConnectCommand(ESATCCSDSPacket& packet)
{
  connectionState = CONNECTING_TO_NETWORK;
}

void ESATWifi::handleDisconnectCommand(ESATCCSDSPacket& packet)
{
  connectionState = DISCONNECTING;
}

void ESATWifi::handleSetNetworkSSIDCommand(ESATCCSDSPacket& packet)
{
  for (byte i = 0; i < WifiConfiguration.NETWORK_SSID_LENGTH; i++)
  {
    WifiConfiguration.networkSSID[i] = packet.readByte();
  }
}

void ESATWifi::handleSetNetworkPassphraseCommand(ESATCCSDSPacket& packet)
{
  for (byte i = 0; i < WifiConfiguration.NETWORK_PASSPHRASE_LENGTH; i++)
  {
    WifiConfiguration.networkPassphrase[i] = packet.readByte();
  }
}

void ESATWifi::handleSetServerAddressCommand(ESATCCSDSPacket& packet)
{
  for (byte i = 0; i < WifiConfiguration.SERVER_ADDRESS_LENGTH; i++)
  {
    WifiConfiguration.serverAddress[i] = packet.readByte();
  }
}

void ESATWifi::handleSetServerPortCommand(ESATCCSDSPacket& packet)
{
  WifiConfiguration.serverPort = packet.readWord();
}

void ESATWifi::handleReadConfigurationCommand(ESATCCSDSPacket& packet)
{
  WifiConfiguration.readConfiguration();
}

void ESATWifi::handleWriteConfigurationCommand(ESATCCSDSPacket& packet)
{
  WifiConfiguration.writeConfiguration();
}

boolean ESATWifi::readPacketFromRadio(ESATCCSDSPacket& packet)
{
  if (connectionState != CONNECTED)
  {
    return false;
  }
  if (client.available() > 0)
  {
    return packet.readFrom(client);
  }
  else
  {
    return false;
  }
}

boolean ESATWifi::readPacketFromSerial(ESATCCSDSPacket& packet)
{
  if (Serial.available())
  {
    return packet.readFrom(Serial);
  }
  else
  {
    return false;
  }
}

void ESATWifi::update()
{
  switch (connectionState)
  {
    case CONNECTING_TO_NETWORK:
      connectToNetwork();
      break;
    case CONNECTING_TO_SERVER:
      connectToServer();
      break;
    case DISCONNECTING:
      disconnect();
      break;
    default:
      break;
  }
}

void ESATWifi::writePacketToRadio(ESATCCSDSPacket& packet)
{
  if (connectionState == CONNECTED)
  {
    (void) packet.writeTo(client);
  }
}

void ESATWifi::writePacketToSerial(ESATCCSDSPacket& packet)
{
  (void) packet.writeTo(Serial);
}

ESATWifi Wifi;
