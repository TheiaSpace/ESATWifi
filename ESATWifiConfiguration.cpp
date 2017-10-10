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

#include "ESATWifiConfiguration.h"

void ESATWifiConfiguration::begin()
{
}

void ESATWifiConfiguration::readConfiguration()
{
  readNetworkConnectionAttempts();
  readNetworkConnectionAttemptInterval();
  readSSID();
  readPassphrase();
  readAddress();
  readPort();
}

void ESATWifiConfiguration::readAddress()
{
}

void ESATWifiConfiguration::readNetworkConnectionAttemptInterval()
{
}

void ESATWifiConfiguration::readNetworkConnectionAttempts()
{
}

void ESATWifiConfiguration::readPassphrase()
{
}

void ESATWifiConfiguration::readPort()
{
}

void ESATWifiConfiguration::readSSID()
{
}

void ESATWifiConfiguration::writeConfiguration()
{
  writeNetworkConnectionAttempts();
  writeNetworkConnectionAttemptInterval();
  writeSSID();
  writePassphrase();
  writeAddress();
  writePort();
}

void ESATWifiConfiguration::writeAddress()
{
}

void ESATWifiConfiguration::writeNetworkConnectionAttemptInterval()
{
}

void ESATWifiConfiguration::writeNetworkConnectionAttempts()
{
}

void ESATWifiConfiguration::writePassphrase()
{
}

void ESATWifiConfiguration::writePort()
{
}

void ESATWifiConfiguration::writeSSID()
{
}

ESATWifiConfiguration WifiConfiguration;
