/*
 * Copyright (C) 2018 Theia Space, Universidad Politécnica de Madrid
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

#include "ESAT_Wifi-telecommands/ESAT_WifiSetTimeTelecommand.h"
#include "ESAT_Wifi.h"

const ESAT_SemanticVersionNumber ESAT_WifiSetTimeTelecommandClass::INTERFACE_VERSION_NUMBER(2, 1, 0);

boolean ESAT_WifiSetTimeTelecommandClass::accept(const ESAT_CCSDSSecondaryHeader secondaryHeader) const
{
  if (!INTERFACE_VERSION_NUMBER.isForwardCompatibleWith(secondaryHeader.majorVersionNumber,
                                                        secondaryHeader.minorVersionNumber,
                                                        secondaryHeader.patchVersionNumber))
  {
    return false;
  }
  if (secondaryHeader.packetIdentifier != WIFI_SET_TIME)
  {
    return false;
  }
  return true;
}

boolean ESAT_WifiSetTimeTelecommandClass::consume(ESAT_CCSDSPacket packet)
{
  const ESAT_CCSDSSecondaryHeader secondaryHeader =
    packet.readSecondaryHeader();
  if (accept(secondaryHeader))
  {
    return handle(packet);
  }
  else
  {
    return false;
  }
}

boolean ESAT_WifiSetTimeTelecommandClass::handle(ESAT_CCSDSPacket packet) const
{
  const ESAT_Timestamp timestamp = packet.readTimestamp();
  if (packet.triedToReadBeyondLength())
  {
    (void) timestamp; // Ignored.
    return false;
  }
  else
  {
    ESAT_Wifi.setTime(timestamp);
    return true;
  }
}

ESAT_WifiSetTimeTelecommandClass ESAT_WifiSetTimeTelecommand;
