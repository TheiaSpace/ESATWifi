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

#ifndef ESAT_WifiConfiguration_h
#define ESAT_WifiConfiguration_h

#include <Arduino.h>

// Wifi module configuration.
// Use the global ESAT_WifiConfiguration object to access the
// configuration parameters.
// The configuration parameters are simple atributes that can
// be read and written freely:
// - networkSSID: the SSID of the wireless network.  Used
//   as an argument to WiFi.begin().
// - networkPassphrase: the passphrase of the wireless network.
//   Used as an argument to WiFi.begin().
// - serverAddress: the address of the ground segment server.
//   Used as an argument to WiFiClient.connect().
// - serverPort: the port of the ground segment server.
//   Used as an argument to WiFiClient.connect().
// The configuration parameters can be stored in the EEPROM in order
// to have persistency between reboots.
// Before loading the configuration parameters from storage or storing
// them, call ESAT_WifiConfiguration.begin() to up the EEPROM access.
// To load the stored configuration, call
// ESAT_WifiConfiguration.readConfiguration().
// To store the configuration, call
// ESAT_WifiConfiguration.writeConfiguration().
class ESAT_WifiConfigurationClass
{
  public:
    // Maximum length of the SSID of the wireless network.
    static const word NETWORK_SSID_LENGTH = 32;

    // Maximum length of the passphrase of the wireless network.
    static const word NETWORK_PASSPHRASE_LENGTH = 65;

    // Length of the ground segment server address.
    static const word SERVER_ADDRESS_LENGTH = 64;

    // Connect to the wireless network using this passphrase.
    char networkPassphrase[NETWORK_PASSPHRASE_LENGTH];

    // Connect to this ground segment server host address.
    char serverAddress[SERVER_ADDRESS_LENGTH];

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
    // Offset of the network SSID parameter in the storage.
    static const word NETWORK_SSID_OFFSET = 0;

    // Offset of the network passphrase parameter in the storage.
    static const word NETWORK_PASSPHRASE_OFFSET =
      NETWORK_SSID_OFFSET
      + NETWORK_SSID_LENGTH;

    // Offset of the server address parameter in the storage.
    static const word SERVER_ADDRESS_OFFSET =
      NETWORK_PASSPHRASE_OFFSET
      + NETWORK_PASSPHRASE_LENGTH;

    // Offset of the server port parameter in the storage.
    static const word SERVER_PORT_OFFSET =
      SERVER_ADDRESS_OFFSET
      + SERVER_ADDRESS_LENGTH;

    // Length of the server port parameter.
    static const word SERVER_PORT_LENGTH = 2;

    // Total length of the configuration.
    static const word CONFIGURATION_LENGTH =
      NETWORK_SSID_LENGTH
      + NETWORK_PASSPHRASE_LENGTH
      + SERVER_ADDRESS_LENGTH
      + SERVER_PORT_LENGTH;

    // Read the passphrase of the wireless network.
    // Part of the configuration.
    void readNetworkPassphrase();

    // Read the SSID of the wireless network.
    // Part of the configuration.
    void readNetworkSSID();

    // Read the address of the ground segment server.
    // Part of the configuration.
    void readServerAddress();

    // Read the port of the ground segment server.
    // Part of the configuration.
    void readServerPort();

    // Write the passphrase of the wireless network.
    // Part of the configuration.
    void writeNetworkPassphrase();

    // Write the SSID of the wireless network.
    // Part of the configuration.
    void writeNetworkSSID();

    // Write the address of the ground segment server.
    // Part of the configuration.
    void writeServerAddress();

    // Write the port of the ground segment server.
    // Part of the configuration.
    void writeServerPort();
};

// Global instance of the Wifi configuration library.
extern ESAT_WifiConfigurationClass ESAT_WifiConfiguration;

#endif /* ESAT_WifiConfiguration_h */
