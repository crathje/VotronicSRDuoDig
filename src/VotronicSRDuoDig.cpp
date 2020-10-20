/*
   Copyright 2020 Claas Anders Rathje
   Arduino / C++ Library

   https://github.com/crathje/VotronicSRDuoDig

   Permission is hereby granted, free of charge, to any person obtaining a copy of this
   software and associated documentation files (the "Software"), to deal in the Software
   without restriction, including without limitation the rights to use, copy, modify,
   merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to the following
   conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
   PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
   OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "VotronicSRDuoDig.h"



float VotronicSRDuoDig::getCurrent() {
  return solardata.ampere / 10.0;
}


void VotronicSRDuoDig::print2serial(_OSTREAM& serial)
{
  serial.printf("valid %lums ago\n", getValidAge());
  serial.printf("Ampere: %6.2f\r\n", getCurrent());
}



bool VotronicSRDuoDig::handle(uint8_t c)
{


  //  Serial.printf("handle %2x %3d \r\n", c, vt_buffp);

  // prevent overflow
  if (vt_buffp >= _VOTRINBUFFSIZE - 1)
  {
    vt_buffp = 0; // failed
    memset(vt_buff, 0, _VOTRINBUFFSIZE);
  }

  // save byte
  vt_buff[vt_buffp++] = c;



  uint16_t dataStart = vt_buffp < _VOTRINDATAGRAMSIZE ? 0 : (vt_buffp - (_VOTRINDATAGRAMSIZE));
  //Serial.printf("handle %2x %3d %d\r\n", c, vt_buffp, dataStart);
  if (vt_buffp >= _VOTRINDATAGRAMSIZE // at least a complete datagram is received
      && vt_buff[dataStart + 0] == 0xAA // header byte #1
      && vt_buff[dataStart + 1] == 0x1A // header byte #2
      && VotronicSRDuoDigCRC(vt_buff, dataStart, _VOTRINDATAGRAMSIZE) == 0)
  {
    last_valid_packet = _MILLIS;
    memcpy((void*)&solardata, vt_buff + dataStart, sizeof(solardata_t));

#if _VRDEBUG > 1
#if defined(ESP8266) || defined(ESP31B) || defined(ESP32)
    Serial.printf("Valid after %d\r\n", vt_buffp);
    Serial.printf("Header: %2X %2X\r\n", solardata.header1, solardata.header2);
    print2serial(_VTDEBUGSTREAM);
#endif
#endif

    // get ready for next packet
    memset(vt_buff, 0, _VOTRINBUFFSIZE);
    vt_buffp = 0;
    return true;
  }
  return false;
}

void VotronicSRDuoDig::handle(_ISTREAM &stream)
{
  while (stream.available()) {
    handle(stream.read());
  }
}


uint16_t VotronicSRDuoDig::VotronicSRDuoDigCRC(uint8_t* d, int start = 0, uint8_t len = 16)
{
  uint8_t b = 0x00;
  for (int i = start + 1; i < start + len; i++)
  {
    b ^= d[i];
  }
  return b;
}

bool VotronicSRDuoDig::isValid() {
  return (getValidAge() <= _VOTRMAXVALIDAGE);
}

unsigned long VotronicSRDuoDig::getValidAge() {
  return _MILLIS - last_valid_packet;
}

void VotronicSRDuoDig::debug2Serial(Stream& serial) {
  //  serial.printf("\n\nBUFFER:\n");
  //  for (int i = 0; i < _VOTRINBUFFSIZE; i++) {
  //    serial.printf("%02d: 0x%02X %3d %5d\n"
  //    , i
  //    , *(((uint8_t*)&vt_buff) + i)
  //    , *(((uint8_t*)&vt_buff) + i)
  //    , *(uint16_t*)(((uint8_t*)&vt_buff) + i));
  //  }
  serial.printf("\nSOLARDATA:\n");
  for (int i = 0; i < sizeof(solardata_t); i++) {
    serial.printf("%02d: 0x%02X %3d %5d\n"
                  , i
                  , *(((uint8_t*)&solardata) + i)
                  , *(((uint8_t*)&solardata) + i)
                  , *(uint16_t*)(((uint8_t*)&solardata) + i));
  }
  serial.printf("Data is ");
  if (isValid()) {
    serial.printf("VALID");
  } else {
    serial.printf("NOT VALID");
  }
  serial.println();
  print2serial(serial);
}
