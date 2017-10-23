#include "ESAT_Wifi.h"
#include <ESAT_CCSDSPacket.h>

const byte NETWORK_CONNECTION_TIMEOUT_SECONDS = 60;
const word PACKET_DATA_BUFFER_LENGTH = 256;
const word WHOLE_PACKET_BUFFER_LENGTH =
  (ESAT_CCSDSPrimaryHeader::LENGTH + PACKET_DATA_BUFFER_LENGTH);

byte radioBuffer[WHOLE_PACKET_BUFFER_LENGTH];
byte serialBuffer[WHOLE_PACKET_BUFFER_LENGTH];

void setup()
{
  (void) WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.begin(9600);
  ESAT_Wifi.begin(radioBuffer,
                  sizeof(radioBuffer),
                  serialBuffer,
                  sizeof(serialBuffer),
                  NETWORK_CONNECTION_TIMEOUT_SECONDS);
}

void loop()
{
  byte packetDataBuffer[PACKET_DATA_BUFFER_LENGTH];
  ESAT_CCSDSPacket packet(packetDataBuffer, PACKET_DATA_BUFFER_LENGTH);
  if (ESAT_Wifi.readPacketFromRadio(packet))
  {
    const ESAT_CCSDSPrimaryHeader primaryHeader = packet.readPrimaryHeader();
    if (primaryHeader.packetType == primaryHeader.TELECOMMAND)
    {
      ESAT_Wifi.writePacketToSerial(packet);
    }
  }
  if (ESAT_Wifi.readPacketFromSerial(packet))
  {
    const ESAT_CCSDSPrimaryHeader primaryHeader = packet.readPrimaryHeader();
    if (primaryHeader.packetType == primaryHeader.TELECOMMAND)
    {
      ESAT_Wifi.handleTelecommand(packet);
    }
    else
    {
      ESAT_Wifi.writePacketToRadio(packet);
    }
  }
  ESAT_Wifi.update();
}
