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
 * along with Theia Space's ESAT Wifi library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

 #include <Arduino.h> 
#include "ESAT_WifiIPAddressToStreamAdapter.h"

boolean ESAT_WifiNetworkInformationTelemetryClass::available()
{
  return true;  
}

ESAT_WifiIPAddressToStreamAdapterClass::ESAT_WifiIPAddressToStreamAdapterClass(IPAddress& ip)
{
  // TODO
  // Check if this is a copy of the IP object
  theIP = &ip;
  readAvailable = IP_ADDRESS_BYTES_COUNT;
  writtenCount = 0;
}

int ESAT_WifiIPAddressToStreamAdapterClass::available()
{
  return (int) readAvailable;
}

void ESAT_WifiIPAddressToStreamAdapterClass::flush()
{
  readAvailable = IP_ADDRESS_BYTES_COUNT;
  writtenCount = 0;
}

int ESAT_WifiIPAddressToStreamAdapterClass::peek()
{
  if (available() > 0)
  {
    return (int) (*theIP)[IP_ADDRESS_BYTES_COUNT - readAvailable];
  }
  return -1;
}

int ESAT_WifiIPAddressToStreamAdapterClass::read()
{
  int readingBuffer = peek();
  if (readingBuffer >= 0)
  {
    --readAvailable;
  }
  return readingBuffer;
}

size_t ESAT_WifiIPAddressToStreamAdapterClass::write(uint8_t IPChunk)
{
  if (writtenCount < IP_ADDRESS_BYTES_COUNT)
  {
    (*theIP)[writtenCount] = IPChunk;
    ++writtenCount;    
    return 1;
  }
  return 0;
  
}
