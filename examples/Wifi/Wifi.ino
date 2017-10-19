#include "ESATWifiBoard.h"
#include <ESATCCSDSPacket.h>

const word PACKET_DATA_BUFFER_LENGTH = 256;
const word WHOLE_PACKET_BUFFER_LENGTH =
  (ESATCCSDSPacket::PRIMARY_HEADER_LENGTH + PACKET_DATA_BUFFER_LENGTH);

byte packetDataBuffer[PACKET_DATA_BUFFER_LENGTH];
byte radioBuffer[WHOLE_PACKET_BUFFER_LENGTH];
byte serialBuffer[WHOLE_PACKET_BUFFER_LENGTH];

void setup()
{
  (void) WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.begin(9600);
  WifiBoard.begin(radioBuffer,
                  WHOLE_PACKET_BUFFER_LENGTH,
                  serialBuffer,
                  WHOLE_PACKET_BUFFER_LENGTH);
}

void loop()
{
  ESATCCSDSPacket packet(packetDataBuffer, PACKET_DATA_BUFFER_LENGTH);
  while (WifiBoard.readPacketFromRadio(packet))
  {
    if (packet.readPacketType() == packet.TELECOMMAND)
    {
      WifiBoard.writePacketToSerial(packet);
    }
  }
  while (WifiBoard.readPacketFromSerial(packet))
  {
    if (packet.readPacketType() == packet.TELECOMMAND)
    {
      WifiBoard.handleTelecommand(packet);
    }
    else
    {
      WifiBoard.writePacketToRadio(packet);
    }
  }
  WifiBoard.update();
}
