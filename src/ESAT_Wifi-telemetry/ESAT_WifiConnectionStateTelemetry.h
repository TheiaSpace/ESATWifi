/*
 * Copyright (C) 2018 Theia Space, Universidad Politécnica de Madrid
 *
 * This file is part of Theia Space's ESAT OBC library.
 *
 * Theia Space's ESAT OBC library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Theia Space's ESAT OBC library is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Theia Space's ESAT OBC library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef ESAT_WifiConnectionStateTelemetry_h
#define ESAT_WifiConnectionStateTelemetry_h

#include <Arduino.h>
#include <ESAT_CCSDSPacketContents.h>

// ESAT Wifi board connection state telemetry packet contents.
// ESAT_Wifi uses this.
class ESAT_WifiConnectionStateTelemetryClass: public ESAT_CCSDSPacketContents
{
  public:
    // Return true: the ESAT Wifi board connection state telemetry packet
    // is always available.
    boolean available();

    // Return the packet identifier.
    byte packetIdentifier();

    // Fill the user data field of the given packet.
    // The write pointer of the packet is already at the start
    // of the user data field.
    // Return true on success; otherwise return false.
    boolean fillUserData(ESAT_CCSDSPacket& packet);

  private:
    // ESAT Wifi board connection state telemetry packet identifier.
    static const byte PACKET_IDENTIFIER = 0;
};

// Global instance of ESAT_WifiConnectionStateTelemetry.  ESAT_Wifi uses this
// to fill the ESAT Wifi board connection state telemetry packet.
extern ESAT_WifiConnectionStateTelemetryClass ESAT_WifiConnectionStateTelemetry;

#endif /* ESAT_WifiConnectionStateTelemetry_h */
