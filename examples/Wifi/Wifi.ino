#include "ESATWifi.h"
#include <ESATCCSDSPacket.h>

void setup()
{
  Wifi.begin();
}

void loop()
{
  const word bufferLength = 256;
  byte buffer[bufferLength];
  ESATCCSDSPacket packet(buffer, bufferLength);
  while (Wifi.readPacketFromRadio(packet))
  {
    if (packet.readPacketType() == packet.TELECOMMAND)
    {
      Wifi.writePacketToSerial(packet);
    }
  }
  while (Wifi.readPacketFromSerial(packet))
  {
    if (packet.readPacketType() == packet.TELECOMMAND)
    {
      Wifi.handleTelecommand(packet);
    }
    else
    {
      Wifi.writePacketToRadio(packet);
    }
  }
}
