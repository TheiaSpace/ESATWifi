#include "ESATWifiBoard.h"
#include <ESATCCSDSPacket.h>

void setup()
{
  WifiBoard.begin();
}

void loop()
{
  const word bufferLength = 256;
  byte buffer[bufferLength];
  ESATCCSDSPacket packet(buffer, bufferLength);
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
