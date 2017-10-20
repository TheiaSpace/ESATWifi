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

#ifndef ESATWifiBoard_h
#define ESATWifiBoard_h

#include <Arduino.h>
#include <ESATCCSDSPacket.h>
#include <ESP8266WiFi.h>
#include <ESATKISSStream.h>

// Wifi board module.
// The Wifi board goes into a socket in the OBC board and has the
// following responsibilities:
// - Connect through the wireless interface to a ground segment
//   server.
// - Connect trhough the serial interface to the OBC.
// - Receive telecommand packets from the ground segment server and
//   forward them to the OBC.
// - Receive telecommand packets from the OBC and execute them.
// - Receive telemetry packets from the OBC and forward them to the
//   ground segment server.
// The telecommands handled by the Wifi board are for simple network
// connection configuration actions.
// Use the global Wifi object to access the functionality of the Wifi
// board.  See the example program.
class ESATWifiBoard
{
  public:
    // Set up the Wifi board.
    void begin(byte radioBuffer[],
               unsigned long radioBufferLength,
               byte serialBuffer[],
               unsigned long serialBufferLength,
               byte networkConnectionTimeoutSeconds);

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
      SET_SERVER_ADDRESS = 0x12,
      SET_SERVER_PORT = 0x13,
      READ_CONFIGURATION = 0x20,
      WRITE_CONFIGURATION = 0x21,
    };

    // Possible states of the connection state machine.
    // State transitions:
    // - From any state to CONNECTING_TO_NETWORK (when commanded to
    // - connect).
    // - From CONNECTING_TO_NETWORK to WAITING_FOR_NETWORK_CONNECTION
    //   (right after configuring the Wifi interface to connect to the
    //   network).
    // - From WAITING_FOR_NETWORK_CONNECTION to CONNECTING_TO_SERVER
    //   (when the network connection is established).
    // - From CONNECTING_TO_SERVER to CONNECTED (when the
    //   client-to-server connection is established).
    // - From CONNECTED to CONNECTING_TO_NETWORK (if the network
    //   connection drops).
    // - From CONNECTED to CONNECTING_TO_SERVER (if the network
    //   connection stays active, but the client-to-server connection
    //   drops).
    // - From any state to DISCONNECTING (when commanded to
    // - disconnect).
    // - From DISCONNECTING to DISCONNECTED (right after disconnecting
    // - from the network).
    enum ConnectionState
    {
      CONNECTING_TO_NETWORK,
      WAITING_FOR_NETWORK_CONNECTION,
      CONNECTING_TO_SERVER,
      CONNECTED,
      DISCONNECTING,
      DISCONNECTED,
    };

    // Unique identifier of the Wifi board for telemetry and
    // telecommand purposes.
    static const word APPLICATION_PROCESS_IDENTIFIER = 3;

    // Version numbers.
    static const byte MAJOR_VERSION_NUMBER = 2;
    static const byte MINOR_VERSION_NUMBER = 0;
    static const byte PATCH_VERSION_NUMBER = 0;

    // Line for signaling that the Wifi board is not connected to the server.
    static const byte NOT_CONNECTED_SIGNAL_PIN = 0;

    // Use this client to connect to the ground segment server.
    WiFiClient client;

    // Current state of the connection state machine.
    ConnectionState connectionState;

    // Time when the previous connection attempt started.
    unsigned long networkConnectionStartTimeMilliseconds;

    // Timeout in milliseconds for each network connection attempt.
    // Every time the connection attempt is unsuccessful for longer
    // than the timeout, try connect again.
    unsigned long networkConnectionTimeoutMilliseconds;

    // Decode incoming radio KISS frames with this stream.
    ESATKISSStream radioDecoder;

    // Decode incoming serial KISS frames with this stream.
    ESATKISSStream serialDecoder;

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

extern ESATWifiBoard WifiBoard;

#endif /* ESATWifiBoard_h */
