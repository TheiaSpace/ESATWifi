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
  WifiConfiguration.begin();
  WifiConfiguration.readConfiguration();
}

void ESATWifi::handleTelecommand(ESATCCSDSPacket& packet)
{
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
