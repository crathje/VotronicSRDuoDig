/*
   Copyright 2019 Claas Anders Rathje
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

#ifndef epsolar_h_
#define epsolar_h_


#if defined(ESP8266) || defined(ESP31B) || defined(ESP32)
#include <HardwareSerial.h>
#else
#include <iostream>
#include <istream>
#include <ostream>
#include <chrono>
#endif


#if defined(ARDUINO)
#define _ISTREAM Stream
#define _OSTREAM Stream
#define _MILLIS millis()
#define _VTDEBUGSTREAM Serial
#elif __linux__
#define _ISTREAM istream
#define _OSTREAM ostream
#define _MILLIS system_clock::now().time_since_epoch()
#define _VTDEBUGSTREAM cout
#else
#define _ISTREAM istream
#define _OSTREAM ostream
#define _MILLIS  -1
#define _VTDEBUGSTREAM cout
#endif

// a single packet is 16bytes including header and checksum
#define _VOTRINDATAGRAMSIZE 16

// datagram should be smaller than this
#define _VOTRINBUFFSIZE _VOTRINDATAGRAMSIZE * 3

// technically it should only be valid for ~120ms
#define _VOTRMAXVALIDAGE 5000

// serial speed seems to be 1020 (~980ms bit length)
#define _VOTRSERIALSPEED 1020

// debug
#ifndef _VTDEBUG
#define _VTDEBUG 0
#endif



struct __attribute__((__packed__)) solardata_t
{
  uint8_t header1;
  uint8_t header2;
  uint8_t a01;
  uint8_t a02;
  uint8_t a03;
  uint8_t a04;
  uint16_t ampere;
  uint8_t a07;
  uint8_t a08;
  uint8_t a09;
  uint8_t a10;
  uint8_t a11;
  uint8_t a12;
  uint8_t a13;
  uint8_t a14;
};




class VotronicSRDuoDig {
  private:
    uint8_t vt_buff[_VOTRINBUFFSIZE];
    uint8_t vt_buffp = 0;
    unsigned long last_valid_packet;
  public:
    solardata_t solardata; // valid data will be held here
    float getCurrent();
    bool isValid();
    unsigned long getValidAge();

    void print2serial(_OSTREAM& serial);
    void request(_OSTREAM& serial);
    bool handle(uint8_t c);
    void handle(_ISTREAM &stream);
    void debug2Serial(_OSTREAM &serial);

  private:
    uint16_t VotronicSRDuoDigCRC(uint8_t* d, int start, uint8_t len);
};


#endif
