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

#include "ESATWifiBoard.h"
#include "ESATWifiConfiguration.h"
#include <ESATKISSStream.h>

void ESATWifiBoard::begin()
{
  WiFi.mode(WIFI_STA);
  WifiConfiguration.begin();
  WifiConfiguration.readConfiguration();
  Serial.begin(115200);
  connectionState = DISCONNECTED;
}

void ESATWifiBoard::connectToNetwork()
{
  (void) WiFi.begin(WifiConfiguration.networkSSID,
                    WifiConfiguration.networkPassphrase);
  connectionState = WAITING_FOR_NETWORK_CONNECTION;
}

void ESATWifiBoard::connectToServer()
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

void ESATWifiBoard::disconnect()
{
  (void) WiFi.disconnect(true);
  connectionState = DISCONNECTED;
}

void ESATWifiBoard::handleTelecommand(ESATCCSDSPacket& packet)
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
  if (packet.readPacketDataLength() < ESATCCSDSSecondaryHeader::LENGTH)
  {
    return;
  }
  const ESATCCSDSSecondaryHeader secondaryHeader = packet.readSecondaryHeader();
  if (secondaryHeader.majorVersionNumber < MAJOR_VERSION_NUMBER)
  {
    return;
  }
  switch (secondaryHeader.packetIdentifier)
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

void ESATWifiBoard::handleConnectCommand(ESATCCSDSPacket& packet)
{
  connectionState = CONNECTING_TO_NETWORK;
}

void ESATWifiBoard::handleDisconnectCommand(ESATCCSDSPacket& packet)
{
  connectionState = DISCONNECTING;
}

void ESATWifiBoard::handleSetNetworkSSIDCommand(ESATCCSDSPacket& packet)
{
  for (byte i = 0; i < WifiConfiguration.NETWORK_SSID_LENGTH; i++)
  {
    WifiConfiguration.networkSSID[i] = packet.readByte();
  }
}

void ESATWifiBoard::handleSetNetworkPassphraseCommand(ESATCCSDSPacket& packet)
{
  for (byte i = 0; i < WifiConfiguration.NETWORK_PASSPHRASE_LENGTH; i++)
  {
    WifiConfiguration.networkPassphrase[i] = packet.readByte();
  }
}

void ESATWifiBoard::handleSetServerAddressCommand(ESATCCSDSPacket& packet)
{
  for (byte i = 0; i < WifiConfiguration.SERVER_ADDRESS_LENGTH; i++)
  {
    WifiConfiguration.serverAddress[i] = packet.readByte();
  }
}

void ESATWifiBoard::handleSetServerPortCommand(ESATCCSDSPacket& packet)
{
  WifiConfiguration.serverPort = packet.readWord();
}

void ESATWifiBoard::handleReadConfigurationCommand(ESATCCSDSPacket& packet)
{
  WifiConfiguration.readConfiguration();
}

void ESATWifiBoard::handleWriteConfigurationCommand(ESATCCSDSPacket& packet)
{
  WifiConfiguration.writeConfiguration();
}

boolean ESATWifiBoard::readPacketFromRadio(ESATCCSDSPacket& packet)
{
  if (connectionState != CONNECTED)
  {
    return false;
  }
  if (client.available() > 0)
  {
    const unsigned long bufferLength =
      packet.PRIMARY_HEADER_LENGTH + packet.packetDataBufferLength;
    byte buffer[bufferLength];
    ESATKISSStream decoder(client, buffer, bufferLength);
    return packet.readFrom(decoder);
  }
  else
  {
    return false;
  }
}

boolean ESATWifiBoard::readPacketFromSerial(ESATCCSDSPacket& packet)
{
  if (Serial.available())
  {
    const unsigned long bufferLength =
      packet.PRIMARY_HEADER_LENGTH + packet.packetDataBufferLength;
    byte buffer[bufferLength];
    ESATKISSStream decoder(Serial, buffer, bufferLength);
    return packet.readFrom(decoder);
  }
  else
  {
    return false;
  }
}

void ESATWifiBoard::reconnectIfDisconnected()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connectionState = CONNECTING_TO_NETWORK;
    return;
  }
  if (!client.connected())
  {
    connectionState = CONNECTING_TO_SERVER;
    return;
  }
  connectionState = CONNECTED;
}

void ESATWifiBoard::update()
{
  switch (connectionState)
  {
    case CONNECTING_TO_NETWORK:
      connectToNetwork();
      break;
    case WAITING_FOR_NETWORK_CONNECTION:
      waitForNetworkConnection();
      break;
    case CONNECTING_TO_SERVER:
      connectToServer();
      break;
    case CONNECTED:
      reconnectIfDisconnected();
      break;
    case DISCONNECTING:
      disconnect();
      break;
    case DISCONNECTED:
      break;
    default:
      break;
  }
}

void ESATWifiBoard::waitForNetworkConnection()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    connectionState = CONNECTING_TO_SERVER;
  }
  else
  {
    connectionState = WAITING_FOR_NETWORK_CONNECTION;
  }
}

void ESATWifiBoard::writePacketToRadio(ESATCCSDSPacket& packet)
{
  if (connectionState == CONNECTED)
  {
    ESATKISSStream encoder(client, nullptr, 0);
    (void) packet.writeTo(encoder);
  }
}

void ESATWifiBoard::writePacketToSerial(ESATCCSDSPacket& packet)
{
  ESATKISSStream encoder(Serial, nullptr, 0);
  (void) packet.writeTo(encoder);
}

ESATWifiBoard WifiBoard;
