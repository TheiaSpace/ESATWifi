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
  connect();
}

void ESATWifi::connect()
{
  for (int i = 0; i < WifiConfiguration.networkConnectionAttempts; i++)
  {
    WiFi.begin(WifiConfiguration.ssid, WifiConfiguration.passphrase);
    delay(WifiConfiguration.networkConnectionAttemptInterval);
    if (WiFi.status() == WL_CONNECTED)
    {
      client.connect(WifiConfiguration.serverAddress, WifiConfiguration.port);
    }
  }
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
    case SET_NETWORK_CONNECTION_ATTEMPTS:
      handleSetNetworkConnectionAttemptsCommand(packet);
      break;
    case SET_NETWORK_CONNECTION_ATTEMPT_INTERVAL:
      handleSetNetworkConnectionAttemptIntervalCommand(packet);
      break;
    case SET_SSID:
      handleSetSSIDCommand(packet);
      break;
    case SET_PASSPHRASE:
      handleSetPassphraseCommand(packet);
      break;
    case SET_SERVER_ADDRESS:
      handleSetServerAddressCommand(packet);
      break;
    case SET_PORT:
      handleSetPortCommand(packet);
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
  connect();
}

void ESATWifi::handleDisconnectCommand(ESATCCSDSPacket& packet)
{
  WiFi.disconnect();
}

void ESATWifi::handleSetNetworkConnectionAttemptsCommand(ESATCCSDSPacket& packet)
{
  WifiConfiguration.networkConnectionAttempts = packet.readByte();
}

void ESATWifi::handleSetNetworkConnectionAttemptIntervalCommand(ESATCCSDSPacket& packet)
{
  WifiConfiguration.networkConnectionAttemptInterval = packet.readWord();
}

void ESATWifi::handleSetSSIDCommand(ESATCCSDSPacket& packet)
{
  for (byte i = 0; i < WifiConfiguration.SSID_LENGTH; i++)
  {
    WifiConfiguration.ssid[i] = packet.readByte();
  }
}

void ESATWifi::handleSetPassphraseCommand(ESATCCSDSPacket& packet)
{
  for (byte i = 0; i < WifiConfiguration.PASSPHRASE_LENGTH; i++)
  {
    WifiConfiguration.passphrase[i] = packet.readByte();
  }
}

void ESATWifi::handleSetServerAddressCommand(ESATCCSDSPacket& packet)
{
  for (byte i = 0; i < WifiConfiguration.SERVER_ADDRESS_LENGTH; i++)
  {
    WifiConfiguration.serverAddress[i] = packet.readByte();
  }
}

void ESATWifi::handleSetPortCommand(ESATCCSDSPacket& packet)
{
  WifiConfiguration.port = packet.readWord();
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

void ESATWifi::writePacketToRadio(ESATCCSDSPacket& packet)
{
  (void) packet.writeTo(client);
}

void ESATWifi::writePacketToSerial(ESATCCSDSPacket& packet)
{
  (void) packet.writeTo(Serial);
}

ESATWifi Wifi;
