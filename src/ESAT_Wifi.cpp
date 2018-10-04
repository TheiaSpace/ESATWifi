/*
 * Copyright (C) 2017, 2018 Theia Space, Universidad Politécnica de Madrid
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
#include "ESAT_Wifi-hardware/ESAT_WifiConfiguration.h"
#include "ESAT_Wifi-hardware/ESAT_WifiRadio.h"
#include "ESAT_Wifi-telecommands/ESAT_WifiConnectTelecommand.h"
#include "ESAT_Wifi-telecommands/ESAT_WifiDisconnectTelecommand.h"
#include "ESAT_Wifi-telecommands/ESAT_WifiReadConfigurationTelecommand.h"
#include "ESAT_Wifi-telecommands/ESAT_WifiSetNetworkPassphraseTelecommand.h"
#include "ESAT_Wifi-telecommands/ESAT_WifiSetNetworkSSIDTelecommand.h"
#include "ESAT_Wifi-telecommands/ESAT_WifiSetServerAddressTelecommand.h"
#include "ESAT_Wifi-telecommands/ESAT_WifiSetServerPortTelecommand.h"
#include "ESAT_Wifi-telecommands/ESAT_WifiSetTimeTelecommand.h"
#include "ESAT_Wifi-telecommands/ESAT_WifiWriteConfigurationTelecommand.h"
#include "ESAT_Wifi-telemetry/ESAT_WifiConnectionStateTelemetry.h"
#include <ESAT_Buffer.h>
#include <ESAT_CCSDSPacketToKISSFrameWriter.h>

void ESAT_WifiClass::addTelecommand(ESAT_CCSDSTelecommandPacketHandler& telecommand)
{
  telecommandPacketDispatcher.add(telecommand);
}

void ESAT_WifiClass::addTelemetry(ESAT_CCSDSTelemetryPacketContents& telemetry)
{
  telemetryPacketBuilder.add(telemetry);
  enabledTelemetry.clear(telemetry.packetIdentifier());
}

void ESAT_WifiClass::begin(byte radioBuffer[],
                           unsigned long radioBufferLength,
                           byte serialBuffer[],
                           unsigned long serialBufferLength,
                           const byte networkConnectionTimeoutSeconds)
{
  enabledTelemetry.clearAll();
  pendingTelemetry.clearAll();
  addTelemetry(ESAT_WifiConnectionStateTelemetry);
  enableTelemetry(ESAT_WifiConnectionStateTelemetry.packetIdentifier());
  addTelecommand(ESAT_WifiConnectTelecommand);
  addTelecommand(ESAT_WifiDisconnectTelecommand);
  addTelecommand(ESAT_WifiSetNetworkSSIDTelecommand);
  addTelecommand(ESAT_WifiSetNetworkPassphraseTelecommand);
  addTelecommand(ESAT_WifiSetServerAddressTelecommand);
  addTelecommand(ESAT_WifiSetServerPortTelecommand);
  addTelecommand(ESAT_WifiReadConfigurationTelecommand);
  addTelecommand(ESAT_WifiWriteConfigurationTelecommand);
  addTelecommand(ESAT_WifiSetTimeTelecommand);
  ESAT_WifiConfiguration.begin();
  ESAT_WifiConfiguration.readConfiguration();
  ESAT_WifiRadio.begin(radioBuffer,
                       radioBufferLength,
                       networkConnectionTimeoutSeconds);
  serialReader = ESAT_CCSDSPacketFromKISSFrameReader(Serial,
                                                     serialBuffer,
                                                     serialBufferLength);
  pinMode(NOT_CONNECTED_SIGNAL_PIN, OUTPUT);
  digitalWrite(NOT_CONNECTED_SIGNAL_PIN, HIGH);
  pinMode(RESET_TELEMETRY_QUEUE_PIN, INPUT_PULLUP);
  attachInterrupt(RESET_TELEMETRY_QUEUE_PIN, resetTelemetryQueue, FALLING);
}

void ESAT_WifiClass::disableTelemetry(const byte identifier)
{
  enabledTelemetry.clear(identifier);
}

void ESAT_WifiClass::enableTelemetry(const byte identifier)
{
  enabledTelemetry.set(identifier);
}

void ESAT_WifiClass::handleTelecommand(ESAT_CCSDSPacket& packet)
{
  packet.rewind();
  (void) telecommandPacketDispatcher.dispatch(packet);
}

boolean ESAT_WifiClass::readPacketFromRadio(ESAT_CCSDSPacket& packet)
{
  return ESAT_WifiRadio.read(packet);
}

boolean ESAT_WifiClass::readPacketFromSerial(ESAT_CCSDSPacket& packet)
{
  return serialReader.read(packet);
}

boolean ESAT_WifiClass::readTelemetry(ESAT_CCSDSPacket& packet)
{
  if (pendingTelemetry.available())
  {
    const byte identifier = byte(pendingTelemetry.readNext());
    pendingTelemetry.clear(identifier);
    return telemetryPacketBuilder.build(packet, identifier);
  }
  else
  {
    return false;
  }
}

void ESAT_WifiClass::resetTelemetryQueue()
{
  ESAT_Wifi.pendingTelemetry =
    (ESAT_Wifi.pendingTelemetry | ESAT_Wifi.telemetryPacketBuilder.available())
    & ESAT_Wifi.enabledTelemetry;
}

void ESAT_WifiClass::setTime(const ESAT_Timestamp timestamp)
{
  clock.write(timestamp);
}

void ESAT_WifiClass::update()
{
  ESAT_WifiRadio.update();
  if (ESAT_WifiRadio.readConnectionState() == ESAT_WifiRadio.CONNECTED)
  {
    digitalWrite(NOT_CONNECTED_SIGNAL_PIN, LOW);
  }
  else
  {
    digitalWrite(NOT_CONNECTED_SIGNAL_PIN, HIGH);
  }
}

void ESAT_WifiClass::writePacketToRadio(ESAT_CCSDSPacket& packet)
{
  ESAT_WifiRadio.write(packet);
}

void ESAT_WifiClass::writePacketToSerial(ESAT_CCSDSPacket& packet)
{
  ESAT_CCSDSPacketToKISSFrameWriter serialWriter(Serial);
  (void) serialWriter.unbufferedWrite(packet);
}

ESAT_WifiClass ESAT_Wifi;
