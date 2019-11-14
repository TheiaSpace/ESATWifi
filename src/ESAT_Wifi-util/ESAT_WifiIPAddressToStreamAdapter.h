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

#ifndef ESAT_WifiIPAddressToStreamAdapter_h
#define ESAT_WifiIPAddressToStreamAdapter_h

#include <Arduino.h>
#include <IPAddress.h>

// ESAT Wifi IP address to stream adapter.
// Used for writing and reading IP addreses.
// IP addresses are read and written from left (MSB) to right (LSB).
class ESAT_WifiIPAddressToStreamAdapterClass: public Stream
{
  public:
  
  // Number of byte chunks of an IP address.
  const uint8_t IP_ADDRESS_BYTES_COUNT = 4;
  
  // Constructor.
  // Requires an IPAddress object where to read from and write to.
  ESAT_WifiIPAddressToStreamAdapterClass(IPAddress ip);
  
  // Checks if there are any IP chunks available to be read.
  int available();   
  
  // Clears the reading and writing counter.
  void flush();  
  
  // Retrieves last unread IP chunk without wasting it.
  int peek();
  
  // Reads and wastes last unread IP chunk.
  int read();     
 
  using Print::write;  

  // Writes next IP chunk.
  // Returns how many bytes were written.
  size_t write(uint8_t IPChunk);

  private:
  
  // Pointer to IP address object where bytes are read from and written to.
  IPAddress* theIP;
  
  // Unread IP chunks counter.
  uint8_t readAvailable;
  
  // Written IP chunks counter.
  uint8_t writtenCount;

};

#endif /* ESAT_WifiIPAddressToStreamAdapter_h */
