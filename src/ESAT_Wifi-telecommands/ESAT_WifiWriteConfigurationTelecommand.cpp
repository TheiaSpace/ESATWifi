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

#include "ESAT_Wifi-telecommands/ESAT_WifiWriteConfigurationTelecommand.h"
#include "ESAT_WifiConfiguration.h"

const ESAT_SemanticVersionNumber ESAT_WifiWriteConfigurationTelecommandClass::INTERFACE_VERSION_NUMBER(2, 0, 0);

boolean ESAT_WifiWriteConfigurationTelecommandClass::accept(const ESAT_CCSDSSecondaryHeader secondaryHeader) const
{
  if (!INTERFACE_VERSION_NUMBER.isForwardCompatibleWith(secondaryHeader.majorVersionNumber,
                                                        secondaryHeader.minorVersionNumber,
                                                        secondaryHeader.patchVersionNumber))
  {
    return false;
  }
  if (secondaryHeader.packetIdentifier != WIFI_WRITE_CONFIGURATION)
  {
    return false;
  }
  return true;
}

boolean ESAT_WifiWriteConfigurationTelecommandClass::consume(ESAT_CCSDSPacket packet)
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

boolean ESAT_WifiWriteConfigurationTelecommandClass::handle(ESAT_CCSDSPacket packet) const
{
  (void) packet; // Unused.
  ESAT_WifiConfiguration.writeConfiguration();
  return true;
}

ESAT_WifiWriteConfigurationTelecommandClass ESAT_WifiWriteConfigurationTelecommand;
