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

#ifndef ESATWifi_h
#define ESATWifi_h

#include <Arduino.h>
#include <ESATCCSDSPacket.h>

// Wifi module.
class ESATWifi
{
  public:
    // Set up the Wifi board.
    void begin();

    // Handle a telecommand.
    void handleTelecommand(ESATCCSDSPacket& packet);

    // Fill the packet with data read from the radio interface.
    // Return true if there was a new packet; otherwise return false.
    boolean readPacketFromRadio(ESATCCSDSPacket& packet);

    // Fill the packet with data read from the serial interface.
    // Return true if there was a new packet; otherwise return false.
    boolean readPacketFromSerial(ESATCCSDSPacket& packet);

    // Send a packet through the radio interface.
    void writePacketToRadio(ESATCCSDSPacket& packet);

    // Send a packet through the serial interface.
    void writePacketToSerial(ESATCCSDSPacket& packet);
};

extern ESATWifi Wifi;

#endif /* ESATWifi_h */
