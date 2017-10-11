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
#include <EEPROM.h>

void ESATWifiConfiguration::begin()
{
  EEPROM.begin(CONFIGURATION_LENGTH);
}

void ESATWifiConfiguration::readConfiguration()
{
  readNetworkConnectionAttempts();
  readNetworkConnectionAttemptInterval();
  readSSID();
  readPassphrase();
  readServerAddress();
  readServerPort();
}

void ESATWifiConfiguration::readNetworkConnectionAttemptInterval()
{
  const byte highByte =
    EEPROM.read(NETWORK_CONNECTION_ATTEMPT_INTERVAL_OFFSET);
  const byte lowByte =
    EEPROM.read(NETWORK_CONNECTION_ATTEMPT_INTERVAL_OFFSET + 1);
  networkConnectionAttemptInterval = word(highByte, lowByte);
}

void ESATWifiConfiguration::readNetworkConnectionAttempts()
{
  networkConnectionAttempts =
    EEPROM.read(NETWORK_CONNECTION_ATTEMPTS_OFFSET);
}

void ESATWifiConfiguration::readPassphrase()
{
  for (byte i = 0; i < PASSPHRASE_LENGTH; i++)
  {
    passphrase[i] = EEPROM.read(PASSPHRASE_OFFSET + i);
  }
}

void ESATWifiConfiguration::readServerAddress()
{
  for (byte i = 0; i < SERVER_ADDRESS_LENGTH; i++)
  {
    serverAddress[i] = EEPROM.read(SERVER_ADDRESS_OFFSET + i);
  }
}

void ESATWifiConfiguration::readServerPort()
{
  const byte highByte = EEPROM.read(SERVER_PORT_OFFSET);
  const byte lowByte = EEPROM.read(SERVER_PORT_OFFSET + 1);
  serverPort = word(highByte, lowByte);
}

void ESATWifiConfiguration::readSSID()
{
  for (byte i = 0; i < SSID_LENGTH; i++)
  {
    ssid[i] = EEPROM.read(SSID_OFFSET + i);
  }
}

void ESATWifiConfiguration::writeConfiguration()
{
  writeNetworkConnectionAttempts();
  writeNetworkConnectionAttemptInterval();
  writeSSID();
  writePassphrase();
  writeServerAddress();
  writeServerPort();
}

void ESATWifiConfiguration::writeNetworkConnectionAttemptInterval()
{
  EEPROM.write(NETWORK_CONNECTION_ATTEMPT_INTERVAL_OFFSET,
               highByte(networkConnectionAttemptInterval));
  EEPROM.write(NETWORK_CONNECTION_ATTEMPT_INTERVAL_OFFSET + 1,
               lowByte(networkConnectionAttemptInterval));
  EEPROM.commit();
}

void ESATWifiConfiguration::writeNetworkConnectionAttempts()
{
  EEPROM.write(NETWORK_CONNECTION_ATTEMPTS_OFFSET,
               networkConnectionAttempts);
  EEPROM.commit();
}

void ESATWifiConfiguration::writePassphrase()
{
  for (byte i = 0; i < PASSPHRASE_LENGTH; i++)
  {
    EEPROM.write(PASSPHRASE_OFFSET + i,
                 passphrase[i]);
  }
  EEPROM.commit();
}

void ESATWifiConfiguration::writeServerAddress()
{
  for (byte i = 0; i < SERVER_ADDRESS_LENGTH; i++)
  {
    EEPROM.write(SERVER_ADDRESS_OFFSET + i, serverAddress[i]);
  }
  EEPROM.commit();
}

void ESATWifiConfiguration::writeServerPort()
{
  EEPROM.write(SERVER_PORT_OFFSET, highByte(serverPort));
  EEPROM.write(SERVER_PORT_OFFSET + 1, lowByte(serverPort));
  EEPROM.commit();
}

void ESATWifiConfiguration::writeSSID()
{
  for (byte i = 0; i < SSID_LENGTH; i++)
  {
    EEPROM.write(SSID_OFFSET + i,
                 ssid[i]);
  }
  EEPROM.commit();
}

ESATWifiConfiguration WifiConfiguration;
