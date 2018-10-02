Copyright (C) 2018 Theia Space, Universidad Politécnica de Madrid

This file is part of Theia Space's ESAT Wifi library.

Theia Space's ESAT Wifi library is free software: you can
redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

Theia Space's ESAT Wifi library is distributed in the hope that it
will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Theia Space's ESAT Wifi library.  If not, see
<http://www.gnu.org/licenses/>.


Wifi telecommand handlers provide telecommand responses.  Each
telecommand handler implements the ESAT_CCSDSPacketConsumer interface
from ESATUtil and responds to one or several telecommands.  They are
used by ESAT_Wifi.


# ESAT_WifiConnectTelecommand

Telecommand handler for WIFI_CONNECT (0x00): connect to the network
and ground station server.


# ESAT_WifiDisconnectTelecommand

Telecommand handler for WIFI_DISCONNECT (0x01): disconnect from the
network and ground station server.


# ESAT_WifiSetNetworkSSIDTelecommand

Telecommand handler for WIFI_SET_NETWORK_SSID (0x10): set the network
SSID.


# ESAT_WifiSetNetworkPassphraseTelecommand

Telecommand handler for WIFI_SET_NETWORK_PASSPHRASE (0x11): set the
network passphrase.


# ESAT_WifiSetServerAddressTelecommand

Telecommand handler for WIFI_SET_SERVER_ADDRESS (0x12): set the
server address.


# ESAT_WifiSetServerPortTelecommand

Telecommand handler for WIFI_SET_SERVER_PORT (0x13): set the
server port.


# ESAT_WifiReadConfigurationTelecommand

Telecommand handler for WIFI_READ_CONFIGURATION (0x20): read the
configuration.


# ESAT_WifiWriteConfigurationTelecommand

Telecommand handler for WIFI_WRITE_CONFIGURATION (0x21): write the
configuration.


# ESAT_WifiSetTimeTelecommand

Telecommand handler for WIFI_SET_TIME (0x30): set the real-time clock.
