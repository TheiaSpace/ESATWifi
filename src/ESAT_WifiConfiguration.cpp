/*
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

#include "ESAT_WifiConfiguration.h"
#include <EEPROM.h>

void ESAT_WifiConfigurationClass::begin()
{
  EEPROM.begin(CONFIGURATION_LENGTH);
}

void ESAT_WifiConfigurationClass::readConfiguration()
{
  readNetworkSSID();
  readNetworkPassphrase();
  readServerAddress();
  readServerPort();
}

void ESAT_WifiConfigurationClass::readNetworkPassphrase()
{
  for (byte i = 0; i < NETWORK_PASSPHRASE_LENGTH; i++)
  {
    networkPassphrase[i] = EEPROM.read(NETWORK_PASSPHRASE_OFFSET + i);
  }
}

void ESAT_WifiConfigurationClass::readNetworkSSID()
{
  for (byte i = 0; i < NETWORK_SSID_LENGTH; i++)
  {
    networkSSID[i] = EEPROM.read(NETWORK_SSID_OFFSET + i);
  }
}

void ESAT_WifiConfigurationClass::readServerAddress()
{
  for (byte i = 0; i < SERVER_ADDRESS_LENGTH; i++)
  {
    serverAddress[i] = EEPROM.read(SERVER_ADDRESS_OFFSET + i);
  }
}

void ESAT_WifiConfigurationClass::readServerPort()
{
  const byte highByte = EEPROM.read(SERVER_PORT_OFFSET);
  const byte lowByte = EEPROM.read(SERVER_PORT_OFFSET + 1);
  serverPort = word(highByte, lowByte);
}

void ESAT_WifiConfigurationClass::writeConfiguration()
{
  writeNetworkSSID();
  writeNetworkPassphrase();
  writeServerAddress();
  writeServerPort();
}

void ESAT_WifiConfigurationClass::writeNetworkPassphrase()
{
  for (byte i = 0; i < NETWORK_PASSPHRASE_LENGTH; i++)
  {
    EEPROM.write(NETWORK_PASSPHRASE_OFFSET + i,
                 networkPassphrase[i]);
  }
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeNetworkSSID()
{
  for (byte i = 0; i < NETWORK_SSID_LENGTH; i++)
  {
    EEPROM.write(NETWORK_SSID_OFFSET + i,
                 networkSSID[i]);
  }
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeServerAddress()
{
  for (byte i = 0; i < SERVER_ADDRESS_LENGTH; i++)
  {
    EEPROM.write(SERVER_ADDRESS_OFFSET + i, serverAddress[i]);
  }
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeServerPort()
{
  EEPROM.write(SERVER_PORT_OFFSET, highByte(serverPort));
  EEPROM.write(SERVER_PORT_OFFSET + 1, lowByte(serverPort));
  EEPROM.commit();
}

ESAT_WifiConfigurationClass ESAT_WifiConfiguration;
