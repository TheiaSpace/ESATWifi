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

#ifndef ESATWifiConfiguration_h
#define ESATWifiConfiguration_h

#include <Arduino.h>

// Wifi configuration.
class ESATWifiConfiguration
{
  public:
    // Maximum length of the SSID of the wireless network.
    static const byte NETWORK_SSID_LENGTH = 32;

    // Maximum length of the passphrase of the wireless network.
    static const byte PASSPHRASE_LENGTH = 65;

    // Length of the ground segment server address.
    static const byte SERVER_ADDRESS_LENGTH = 4;

    // Wait this number of milliseconds between network connection attempts.
    word networkConnectionAttemptInterval;

    // Attempt to connect to the network up to this number of times.
    byte networkConnectionAttempts;

    // Connect to the wireless network using this passphrase.
    char passphrase[PASSPHRASE_LENGTH];

    // Connect to this ground segment server host address.
    byte serverAddress[SERVER_ADDRESS_LENGTH];

    // Connect to this port of the ground segment server.
    word serverPort;

    // Connect to the wireless network with this SSID.
    char networkSSID[NETWORK_SSID_LENGTH];

    // Set up the configuration storage.
    void begin();

    // Read the configuration.
    void readConfiguration();

    // Write the configuration.
    void writeConfiguration();

  private:
    // Offset of the network connection attempts parameter in the storage.
    static const byte NETWORK_CONNECTION_ATTEMPTS_OFFSET = 0;

    // Length of the network connection attempts parameter.
    static const byte NETWORK_CONNECTION_ATTEMPTS_LENGTH = 1;

    // Offset of the network connection attempt interval parameter
    // in the storage.
    static const byte NETWORK_CONNECTION_ATTEMPT_INTERVAL_OFFSET =
      NETWORK_CONNECTION_ATTEMPTS_OFFSET
      + NETWORK_CONNECTION_ATTEMPTS_LENGTH;

    // Length of the network connection interval parameter.
    static const byte NETWORK_CONNECTION_ATTEMPT_INTERVAL_LENGTH = 2;

    // Offset of the network SSID parameter in the storage.
    static const byte NETWORK_SSID_OFFSET =
      NETWORK_CONNECTION_ATTEMPTS_OFFSET
      + NETWORK_CONNECTION_ATTEMPTS_LENGTH;

    // Offset of the passphrase parameter in the storage.
    static const byte PASSPHRASE_OFFSET =
      NETWORK_SSID_OFFSET
      + NETWORK_SSID_LENGTH;

    // Offset of the server address parameter in the storage.
    static const byte SERVER_ADDRESS_OFFSET =
      PASSPHRASE_OFFSET
      + PASSPHRASE_LENGTH;

    // Offset of the server port parameter in the storage.
    static const byte SERVER_PORT_OFFSET =
      SERVER_ADDRESS_OFFSET
      + SERVER_ADDRESS_LENGTH;

    // Length of the server port parameter.
    static const byte SERVER_PORT_LENGTH = 2;

    // Total length of the configuration.
    static const byte CONFIGURATION_LENGTH =
      NETWORK_CONNECTION_ATTEMPT_INTERVAL_LENGTH
      + NETWORK_CONNECTION_ATTEMPTS_LENGTH
      + NETWORK_SSID_LENGTH
      + PASSPHRASE_LENGTH
      + SERVER_ADDRESS_LENGTH
      + SERVER_PORT_LENGTH;

    // Read the interval between network connection attempts.
    // Part of the configuration.
    void readNetworkConnectionAttemptInterval();

    // Read the number of network connection attempts.
    // Part of the configuration.
    void readNetworkConnectionAttempts();

    // Read the SSID of the wireless network.
    // Part of the configuration.
    void readNetworkSSID();

    // Read the passphrase of the wireless network.
    // Part of the configuration.
    void readPassphrase();

    // Read the address of the ground segment server.
    // Part of the configuration.
    void readServerAddress();

    // Read the port of the ground segment server.
    // Part of the configuration.
    void readServerPort();

    // Write the interval between network connection attempts.
    // Part of the configuration.
    void writeNetworkConnectionAttemptInterval();

    // Write the number of network connection attempts.
    // Part of the configuration.
    void writeNetworkConnectionAttempts();

    // Write the SSID of the wireless network.
    // Part of the configuration.
    void writeNetworkSSID();

    // Write the passphrase of the wireless network.
    // Part of the configuration.
    void writePassphrase();

    // Write the address of the ground segment server.
    // Part of the configuration.
    void writeServerAddress();

    // Write the port of the ground segment server.
    // Part of the configuration.
    void writeServerPort();
};

extern ESATWifiConfiguration WifiConfiguration;

#endif /* ESATWifiConfiguration_h */
