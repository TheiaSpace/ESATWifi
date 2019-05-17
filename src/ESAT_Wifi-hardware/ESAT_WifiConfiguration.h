/*
 * Copyright (C) 2019 Theia Space, Universidad Politécnica de Madrid
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
// - hostIPAddress: the address of the ESAT.
//   Used as an argument to WiFi.config (only if DHCP mode is enabled).
// - subnetMask: the current subnetwork mask.
//   Used as an argument to WiFi.config (only if DHCP mode is enabled).
// - gatewayIPAddress: the address of the network default router.
//   Used as an argument to WiFi.config (only if DHCP mode is enabled).
// - hostConfigurationMode: the host IP configuration mode (static or DHCP).
//   Used by ESAT_WifiRadio.connectToNetwork to configure or not static IP.
// - hostname: the name provided to identify the connected ESAT.
//   Used as an argument to wiFi.hostname()
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
  
    // Enum type for storing the host nework configuration mode (static or dynamic).
    enum HostModeConfigurationType
    {
	    DYNAMIC_HOST_CONFIGURATION_MODE = 0, 
	    STATIC_HOST_CONFIGURATION_MODE
    };
	
    //TODO
	//SORT properly
  
    // Maximum length of the SSID of the wireless network.
    static const word NETWORK_SSID_LENGTH = 32;

    // Maximum length of the passphrase of the wireless network.
    static const word NETWORK_PASSPHRASE_LENGTH = 65;

    // Length of the ground segment server address.
    static const word SERVER_ADDRESS_LENGTH = 64;
	
	// Length of the server port parameter.
    static const word SERVER_PORT_LENGTH = 2;
	
	// Length of any IP address.
	static const word IP_ADDRESS_LENGTH = 4;
	
	// Length of the host configuration mode paramenter;
	static const word HOST_CONFIGURATION_MODE_LENGTH = 1;
	
	// Length of the ESAT hostname.
	static const word HOSTNAME_LENGTH = 32;

	// Connect to the wireless network with this SSID.
    char networkSSID[NETWORK_SSID_LENGTH+1];

    // Connect to the wireless network using this passphrase.
    char networkPassphrase[NETWORK_PASSPHRASE_LENGTH+1];

    // Connect to this ground segment server host address.
    char serverAddress[SERVER_ADDRESS_LENGTH+1];

    // Connect to this port of the ground segment server.
    word serverPort;
	
	// Use this IP adddress to manually connect to the wireless network.
	byte hostAddress[IP_ADDRESS_LENGTH];
	
	// Use this subnet mask to manually connect to the wireless network.
	byte subnetMask[IP_ADDRESS_LENGTH];
	
	// Use this IP address as default adddress for routing packets.
	byte gatewayAddress[IP_ADDRESS_LENGTH];
	
	// Use this variable to store whether host network parameters are manually or self configured.
	HostModeConfigurationType hostConfigurationMode; 
	
	// Use this name to identify the host.
	char hostname[HOSTNAME_LENGTH+1];	
	
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
	  
	// Offset of the host address paramenter in the storage.
	static const word HOST_ADDRESS_OFFSET =
	  SERVER_PORT_OFFSET
	  + SERVER_PORT_LENGTH;
	  
	// Offset of the subnet mask parameter in the storage.
	static const word SUBNET_MASK_OFFSET =
	  HOST_ADDRESS_OFFSET
	  + IP_ADDRESS_LENGTH;
	  
	// Offset of the default gateway address paramenter in the storage.
	static const word GATEWAY_ADDRESS_OFFSET =
	  SUBNET_MASK_OFFSET
	  + IP_ADDRESS_LENGTH;
	  
	// Ofset of the host configuration mode paramenter in the storage.
	static const word HOST_CONFIGURATION_MODE_OFFSET =
	  GATEWAY_ADDRESS_OFFSET
      + IP_ADDRESS_LENGTH;	  	
	  
	// Offset of the ESAT hostname address parameter in the storage.
	static const word HOSTNAME_OFFSET = 
	  HOST_CONFIGURATION_MODE_OFFSET
	  + HOST_CONFIGURATION_MODE_LENGTH;
	  
    // Total length of the configuration.
    static const word CONFIGURATION_LENGTH =
      NETWORK_SSID_LENGTH
      + NETWORK_PASSPHRASE_LENGTH
      + SERVER_ADDRESS_LENGTH
      + SERVER_PORT_LENGTH
	  + IP_ADDRESS_LENGTH
	  + IP_ADDRESS_LENGTH
	  + IP_ADDRESS_LENGTH
	  + HOST_CONFIGURATION_MODE_LENGTH
	  + HOSTNAME_LENGTH;

	// Read a multiple char configuration string.
	void readString(char* outputBuffer, word length, word offset);

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
	
	// Read the address of the host.
	// Part of the configuration.
	void readHostAddress();
	
	// Read the subnetwork mask.
	// Part of the configuration.
	void readSubnetMask();
	
	// Read the address of the default gateway.
	// Part of the configuration.
	void readGatewayAddress();
	
	// Read the host configuration mode parameter.
	// Part of the configuration.
	void readHostConfigurationMode();
	
	// Read the hostname.
	// Part of the configuration.
	void readHostname();
	
	// Write a multiple char configuration string.
	void writeString(char* inputBuffer, word length, word offset);
	
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
	
	// Write the address of the host.
	// Part of the configuration.
	void writeHostAddress();
	
	// Write the subnetwork mask.
	// Part of the configuration.
	void writeSubnetMask();
	
	// Write the address of the default gateway.
	// Part of the configuration.
	void writeGatewayAddress();
	
	// Write the host configuration mode parameter.
	// Part of the configuration.
	void writeHostConfigurationMode();
	
	// Write the hostname.
	// Part of the configuration.
	void writeHostname();
	
};

// Global instance of the Wifi configuration library.
extern ESAT_WifiConfigurationClass ESAT_WifiConfiguration;

#endif /* ESAT_WifiConfiguration_h */
