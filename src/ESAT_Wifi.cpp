/*
 * Copyright (C) 2017-2018 Theia Space, Universidad Politécnica de Madrid
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
 * along with Theia Space's ESAT Util library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ESAT_Wifi.h"
#include "ESAT_WifiConfiguration.h"
#include <ESAT_Buffer.h>
#include <ESAT_CCSDSPacketToKISSFrameWriter.h>

void ESAT_WifiClass::begin(byte radioBuffer[],
                           unsigned long radioBufferLength,
                           byte serialBuffer[],
                           unsigned long serialBufferLength,
                           const byte networkConnectionTimeoutSeconds)
{
  ESAT_WifiConfiguration.begin();
  ESAT_WifiConfiguration.readConfiguration();
  connectionState = DISCONNECTED;
  networkConnectionTimeoutMilliseconds =
    1000 * ((unsigned long) networkConnectionTimeoutSeconds);
  radioReader = ESAT_CCSDSPacketFromKISSFrameReader(client,
                                                    radioBuffer,
                                                    radioBufferLength);
  serialReader = ESAT_CCSDSPacketFromKISSFrameReader(Serial,
                                                     serialBuffer,
                                                     serialBufferLength);
  pinMode(NOT_CONNECTED_SIGNAL_PIN, OUTPUT);
  digitalWrite(NOT_CONNECTED_SIGNAL_PIN, HIGH);
}

void ESAT_WifiClass::connectToNetwork()
{
  disconnect();
  (void) WiFi.begin(ESAT_WifiConfiguration.networkSSID,
                    ESAT_WifiConfiguration.networkPassphrase);
  connectionState = WAITING_FOR_NETWORK_CONNECTION;
  networkConnectionStartTimeMilliseconds = millis();
}

void ESAT_WifiClass::connectToServer()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (client.connect(ESAT_WifiConfiguration.serverAddress,
                       ESAT_WifiConfiguration.serverPort))
    {
      connectionState = CONNECTED;
    }
    else
    {
      connectionState = CONNECTING_TO_SERVER;
    }
  }
  else
  {
    connectionState = CONNECTING_TO_NETWORK;
  }
}

void ESAT_WifiClass::disconnect()
{
  if (client.connected())
  {
    client.stop();
  }
  (void) WiFi.disconnect(true);
  connectionState = DISCONNECTED;
}

void ESAT_WifiClass::handleTelecommand(ESAT_CCSDSPacket& packet)
{
  packet.rewind();
  const ESAT_CCSDSPrimaryHeader primaryHeader = packet.readPrimaryHeader();
  if (primaryHeader.packetType != primaryHeader.TELECOMMAND)
  {
    return;
  }
  if (primaryHeader.applicationProcessIdentifier
      != APPLICATION_PROCESS_IDENTIFIER)
  {
    return;
  }
  if (primaryHeader.packetDataLength < ESAT_CCSDSSecondaryHeader::LENGTH)
  {
    return;
  }
  const ESAT_CCSDSSecondaryHeader secondaryHeader =
    packet.readSecondaryHeader();
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

void ESAT_WifiClass::handleConnectCommand(ESAT_CCSDSPacket& packet)
{
  (void) packet; // Unused.
  connectionState = CONNECTING_TO_NETWORK;
}

void ESAT_WifiClass::handleDisconnectCommand(ESAT_CCSDSPacket& packet)
{
  (void) packet; // Unused;
  connectionState = DISCONNECTING;
}

void ESAT_WifiClass::handleSetNetworkSSIDCommand(ESAT_CCSDSPacket& packet)
{
  ESAT_Buffer networkSSID((byte*) ESAT_WifiConfiguration.networkSSID,
                          sizeof(ESAT_WifiConfiguration.networkSSID));
  (void) networkSSID.readFrom(packet, networkSSID.capacity());
}

void ESAT_WifiClass::handleSetNetworkPassphraseCommand(ESAT_CCSDSPacket& packet)
{
  ESAT_Buffer networkPassphrase((byte*) ESAT_WifiConfiguration.networkPassphrase,
                                sizeof(ESAT_WifiConfiguration.networkPassphrase));
  (void) networkPassphrase.readFrom(packet, networkPassphrase.capacity());
}

void ESAT_WifiClass::handleSetServerAddressCommand(ESAT_CCSDSPacket& packet)
{
  ESAT_Buffer serverAddress((byte*) ESAT_WifiConfiguration.serverAddress,
                            sizeof(ESAT_WifiConfiguration.serverAddress));
  (void) serverAddress.readFrom(packet, serverAddress.capacity());
}

void ESAT_WifiClass::handleSetServerPortCommand(ESAT_CCSDSPacket& packet)
{
  ESAT_WifiConfiguration.serverPort = packet.readWord();
}

void ESAT_WifiClass::handleReadConfigurationCommand(ESAT_CCSDSPacket& packet)
{
  (void) packet; // Unused;
  ESAT_WifiConfiguration.readConfiguration();
}

void ESAT_WifiClass::handleWriteConfigurationCommand(ESAT_CCSDSPacket& packet)
{
  (void) packet; // Unused;
  ESAT_WifiConfiguration.writeConfiguration();
}

boolean ESAT_WifiClass::readPacketFromRadio(ESAT_CCSDSPacket& packet)
{
  if (connectionState != CONNECTED)
  {
    return false;
  }
  return radioReader.read(packet);
}

boolean ESAT_WifiClass::readPacketFromSerial(ESAT_CCSDSPacket& packet)
{
  return serialReader.read(packet);
}

void ESAT_WifiClass::reconnectIfDisconnected()
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

void ESAT_WifiClass::update()
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
  if (connectionState == CONNECTED)
  {
    digitalWrite(NOT_CONNECTED_SIGNAL_PIN, LOW);
  }
  else
  {
    digitalWrite(NOT_CONNECTED_SIGNAL_PIN, HIGH);
  }
}

void ESAT_WifiClass::waitForNetworkConnection()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    connectionState = CONNECTING_TO_SERVER;
  }
  else
  {
    connectionState = WAITING_FOR_NETWORK_CONNECTION;
    const unsigned long currentTime = millis();
    const unsigned long ellapsedTime =
      currentTime - networkConnectionStartTimeMilliseconds;
    if (ellapsedTime > networkConnectionTimeoutMilliseconds)
    {
      connectionState = CONNECTING_TO_NETWORK;
    }
  }
}

void ESAT_WifiClass::writePacketToRadio(ESAT_CCSDSPacket& packet)
{
  if (connectionState == CONNECTED)
  {
    ESAT_CCSDSPacketToKISSFrameWriter radioWriter(client);
    (void) radioWriter.bufferedWrite(packet);
  }
}

void ESAT_WifiClass::writePacketToSerial(ESAT_CCSDSPacket& packet)
{
  ESAT_CCSDSPacketToKISSFrameWriter serialWriter(Serial);
  (void) serialWriter.unbufferedWrite(packet);
}

ESAT_WifiClass ESAT_Wifi;
