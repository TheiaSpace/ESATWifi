/*
 * Copyright (C) 2017, 2018 Theia Space, Universidad Politécnica de Madrid
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

#ifndef ESAT_Wifi_h
#define ESAT_Wifi_h

#include <Arduino.h>
#include <ESAT_CCSDSPacket.h>
#include <ESAT_CCSDSPacketContents.h>
#include <ESAT_CCSDSPacketFromKISSFrameReader.h>
#include <ESAT_CCSDSTelemetryPacketBuilder.h>
#include <ESAT_FlagContainer.h>
#include <ESAT_SoftwareClock.h>
#include <ESP8266WiFi.h>

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
// Use the global ESAT_Wifi object to access the functionality of
// the Wifi board.  See the example program.
class ESAT_WifiClass
{
  public:
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
      CONNECTING_TO_NETWORK = 0x00,
      WAITING_FOR_NETWORK_CONNECTION = 0x01,
      CONNECTING_TO_SERVER = 0x02,
      CONNECTED = 0x03,
      DISCONNECTING = 0x04,
      DISCONNECTED = 0x05,
    };

    // Register a telemetry packet.
    // The telemetry packet will be disabled by default;
    // use enableTelemetry() to enable it.
    void addTelemetry(ESAT_CCSDSPacketContents& telemetry);

    // Set up the Wifi board.
    void begin(byte radioBuffer[],
               unsigned long radioBufferLength,
               byte serialBuffer[],
               unsigned long serialBufferLength,
               byte networkConnectionTimeoutSeconds);

    // Disable the generation of the telemetry packet with the given
    // identifier.
    void disableTelemetry(byte identifier);

    // Enable the generation of the telemetry packet with the given
    // identifier.
    void enableTelemetry(byte identifier);

    // Handle a telecommand.
    void handleTelecommand(ESAT_CCSDSPacket& packet);

    // Return the current connection state.
    ConnectionState readConnectionState() const;

    // Fill the packet with data read from the radio interface.
    // Return true if there was a new packet; otherwise return false.
    boolean readPacketFromRadio(ESAT_CCSDSPacket& packet);

    // Fill the packet with data read from the serial interface.
    // Return true if there was a new packet; otherwise return false.
    boolean readPacketFromSerial(ESAT_CCSDSPacket& packet);

    // Fill a new ESAT Wifi board telemetry packet.
    // Return true if there was a new packet; otherwise return false.
    boolean readTelemetry(ESAT_CCSDSPacket& packet);

    // Perform connection management and related tasks.
    void update();

    // Send a packet through the radio interface.
    void writePacketToRadio(ESAT_CCSDSPacket& packet);

    // Send a packet through the serial interface.
    void writePacketToSerial(ESAT_CCSDSPacket& packet);

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

    // Unique identifier of the Wifi board for telemetry and
    // telecommand purposes.
    static const word APPLICATION_PROCESS_IDENTIFIER = 3;

    // Version numbers.
    static const byte MAJOR_VERSION_NUMBER = 2;
    static const byte MINOR_VERSION_NUMBER = 1;
    static const byte PATCH_VERSION_NUMBER = 0;

    // Line for signaling that the Wifi board is not connected to the server.
    static const byte NOT_CONNECTED_SIGNAL_PIN = 0;

    // Line for signaling a telemetry queue reset condition.
    static const byte RESET_TELEMETRY_QUEUE_PIN = 2;

    // List of enabled telemetry packet identifiers.
    ESAT_FlagContainer enabledTelemetry;

    // Use this client to connect to the ground segment server.
    WiFiClient client;

    // Use this clock for timekeeping.
    ESAT_SoftwareClock clock;

    // Current state of the connection state machine.
    ConnectionState connectionState;

    // Time when the previous connection attempt started.
    unsigned long networkConnectionStartTimeMilliseconds;

    // Timeout in milliseconds for each network connection attempt.
    // Every time the connection attempt is unsuccessful for longer
    // than the timeout, try connect again.
    unsigned long networkConnectionTimeoutMilliseconds;

    // List of pending telemetry packet identifiers.
    ESAT_FlagContainer pendingTelemetry;

    // Use this to read CCSDS packets from KISS frames coming from
    // radio.
    ESAT_CCSDSPacketFromKISSFrameReader radioReader;

    // Use this to read CCSDS packets from KISS frames coming from
    // radio.
    ESAT_CCSDSPacketFromKISSFrameReader serialReader;

    // Use this for building telemetry packets.
    ESAT_CCSDSTelemetryPacketBuilder telemetryPacketBuilder =
      ESAT_CCSDSTelemetryPacketBuilder(APPLICATION_PROCESS_IDENTIFIER,
                                       MAJOR_VERSION_NUMBER,
                                       MINOR_VERSION_NUMBER,
                                       PATCH_VERSION_NUMBER,
                                       clock);

    // Connect to the wireless network.
    void connectToNetwork();

    // Connect to the ground segment server.
    void connectToServer();

    // Disconnect from the wireless network and ground station server.
    void disconnect();

    // Handle a telecommand for connecting to the network and server.
    void handleConnectCommand(ESAT_CCSDSPacket& packet);

    // Handle a telecommand for disconnecting from the network and server.
    void handleDisconnectCommand(ESAT_CCSDSPacket& packet);

    // Handle a telecommand for setting the SSID of the network.
    void handleSetNetworkSSIDCommand(ESAT_CCSDSPacket& packet);

    // Handle a telecommand for setting the passphrase of the network.
    void handleSetNetworkPassphraseCommand(ESAT_CCSDSPacket& packet);

    // Handle a telecommand for setting the address of the ground
    // segment server.
    void handleSetServerAddressCommand(ESAT_CCSDSPacket& packet);

    // Handle a telecommand for setting the port of the ground segment
    // server.
    void handleSetServerPortCommand(ESAT_CCSDSPacket& packet);

    // Handle a telecommand for reading the configuration.
    void handleReadConfigurationCommand(ESAT_CCSDSPacket& packet);

    // Handle a telecommand for writing the configuration.
    void handleWriteConfigurationCommand(ESAT_CCSDSPacket& packet);

    // Reconnect to the server if disconnected from the server or to
    // the network if disconnected from the network.
    void reconnectIfDisconnected();

    // Reset the telemetry queue.
    static void resetTelemetryQueue();

    // Check that the network connection is established.
    void waitForNetworkConnection();
};

extern ESAT_WifiClass ESAT_Wifi;

#endif /* ESAT_Wifi_h */
