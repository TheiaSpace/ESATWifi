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
#include <ESP8266WiFi.h>

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

    // Perform connection management and related tasks.
    void update();

    // Send a packet through the radio interface.
    void writePacketToRadio(ESATCCSDSPacket& packet);

    // Send a packet through the serial interface.
    void writePacketToSerial(ESATCCSDSPacket& packet);

  private:
    // Commands handled by the Wifi board.
    enum CommandCode
    {
      CONNECT = 0x00,
      DISCONNECT = 0x01,
      SET_NETWORK_SSID = 0x10,
      SET_NETWORK_PASSPHRASE = 0x11,
      SET_SERVER_ADDRESS = 0x012,
      SET_SERVER_PORT = 0x13,
      READ_CONFIGURATION = 0x20,
      WRITE_CONFIGURATION = 0x21,
    };

    // Possible states of the connection state machine.
    enum ConnectionState
    {
      CONNECTING_TO_NETWORK,
      WAITING_FOR_NETWORK_CONNECTION,
      CONNECTING_TO_SERVER,
      CONNECTED,
      DISCONNECTING,
      DISCONNECTED,
    };

    static const word APPLICATION_PROCESS_IDENTIFIER = 4;

    // Version numbers.
    static const byte MAJOR_VERSION_NUMBER = 2;
    static const byte MINOR_VERSION_NUMBER = 0;
    static const byte PATCH_VERSION_NUMBER = 0;

    // Minimum telecommand packet data length.
    const byte MINIMUM_TELECOMMAND_PACKET_DATA_LENGTH = 4;

    // Use this client to connect to the ground segment server.
    WiFiClient client;

    // Current state of the connection state machine.
    ConnectionState connectionState;

    // Connect to the wireless network.
    void connectToNetwork();

    // Connect to the ground segment server.
    void connectToServer();

    // Disconnect from the wireless network and ground station server.
    void disconnect();

    // Handle a telecommand for connecting to the network and server.
    void handleConnectCommand(ESATCCSDSPacket& packet);

    // Handle a telecommand for disconnecting from the network and server.
    void handleDisconnectCommand(ESATCCSDSPacket& packet);

    // Handle a telecommand for setting the SSID of the network.
    void handleSetNetworkSSIDCommand(ESATCCSDSPacket& packet);

    // Handle a telecommand for setting the passphrase of the network.
    void handleSetNetworkPassphraseCommand(ESATCCSDSPacket& packet);

    // Handle a telecommand for setting the address of the ground
    // segment server.
    void handleSetServerAddressCommand(ESATCCSDSPacket& packet);

    // Handle a telecommand for setting the port of the ground segment
    // server.
    void handleSetServerPortCommand(ESATCCSDSPacket& packet);

    // Handle a telecommand for reading the configuration.
    void handleReadConfigurationCommand(ESATCCSDSPacket& packet);

    // Handle a telecommand for writing the configuration.
    void handleWriteConfigurationCommand(ESATCCSDSPacket& packet);

    // Reconnect to the server if disconnected from the server or to
    // the network if disconnected from the network.
    void reconnectIfDisconnected();

    // Check that the network connection is established.
    void waitForNetworkConnection();
};

extern ESATWifi Wifi;

#endif /* ESATWifi_h */
