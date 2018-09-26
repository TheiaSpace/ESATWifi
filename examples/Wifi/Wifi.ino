/*
 * ESAT Wifi Main Program version 2.0.1
 * Copyright (C) 2017 Theia Space, Universidad Politécnica de Madrid
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
 * along with Theia Space's ESAT EPS library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ESAT_Wifi.h"
#include <ESAT_CCSDSPacket.h>

// Main program of the Wifi board: respond to telecommands and send
// telemetry.

const byte NETWORK_CONNECTION_TIMEOUT_SECONDS = 60;
const word PACKET_DATA_BUFFER_LENGTH = 256;
const word WHOLE_PACKET_BUFFER_LENGTH =
  (ESAT_CCSDSPrimaryHeader::LENGTH + PACKET_DATA_BUFFER_LENGTH);

byte radioBuffer[WHOLE_PACKET_BUFFER_LENGTH];
byte serialBuffer[WHOLE_PACKET_BUFFER_LENGTH];

// Start the peripherals and do some bookkeeping work.
void setup()
{
  (void) WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.begin(9600);
  ESAT_Wifi.begin(radioBuffer,
                  sizeof(radioBuffer),
                  serialBuffer,
                  sizeof(serialBuffer),
                  NETWORK_CONNECTION_TIMEOUT_SECONDS);
}

// Body of the main loop of the program:
// - Try to read a packet from the radio interface.
//   - If there is a telecommand packet, send it to the On-Board
//     Computer through the serial interface.
// - Try to read a packet from the serial interface.
//   - If there is a telecommand packet, handle it.
//   - If there is a telemetry packet, send it through the radio
//     interface.
// - Update the state of the module.
void loop()
{
  byte packetDataBuffer[PACKET_DATA_BUFFER_LENGTH];
  ESAT_CCSDSPacket packet(packetDataBuffer, PACKET_DATA_BUFFER_LENGTH);
  if (ESAT_Wifi.readPacketFromRadio(packet))
  {
    const ESAT_CCSDSPrimaryHeader primaryHeader = packet.readPrimaryHeader();
    if (primaryHeader.packetType == primaryHeader.TELECOMMAND)
    {
      ESAT_Wifi.writePacketToSerial(packet);
    }
  }
  if (ESAT_Wifi.readPacketFromSerial(packet))
  {
    const ESAT_CCSDSPrimaryHeader primaryHeader = packet.readPrimaryHeader();
    if (primaryHeader.packetType == primaryHeader.TELECOMMAND)
    {
      ESAT_Wifi.handleTelecommand(packet);
    }
    else
    {
      ESAT_Wifi.writePacketToRadio(packet);
    }
  }
  ESAT_Wifi.update();
}
