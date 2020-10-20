#include "Arduino.h"
#include "VotronicSRDuoDig.h"

uint8_t _SAMPLEDATA[] = {0x00 , 0x00 , 0x00 , 0x9B , 0x0A , 0x01 , 0x20 , 0x08 , 0x81 , 0xAA , 0x1A , 0xEF , 0x04 , 0xED , 0x04 , 0x20 , 0x00 , 0x00 , 0x00 , 0x9B , 0x0A , 0x01 , 0x20 , 0x08 , 0x80 , 0xAA , 0x1A , 0xEF , 0x04 , 0xED , 0x04 , 0x20 , 0x00 , 0x00 , 0x00 , 0x9B , 0x0A , 0x01 , 0x20 , 0x08 , 0x80 , 0xAA , 0x1A , 0xEE , 0x04 , 0xED , 0x04 , 0x20 , 0x00 , 0x00 , 0x00 , 0x9B , 0x0A , 0x01 , 0x20 , 0x08 , 0x81 , 0xAA , 0x1A , 0xEF , 0x04 , 0xED , 0x04 , 0x20 , 0x00 , 0x00 , 0x00 , 0x9B , 0x0A , 0x01 , 0x20 , 0x08 , 0x80 , 0xAA , 0x1A , 0xEF , 0x04 , 0xED , 0x04 , 0x20 , 0x00 , 0x00 , 0x00 , 0x9B , 0x0A , 0x01 , 0x20 , 0x08 , 0x80 , 0xAA , 0x1A , 0xEF , 0x04 , 0xED , 0x04 , 0x20 , 0x00 , 0x00 , 0x00 };

VotronicSRDuoDig solar;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.printf("Started\r\n");
}

void loop() {
  for (uint8_t i = 0; i < sizeof(_SAMPLEDATA); i++) {
	  
    // handle returns true in case a new packet has been received with the current character
    if (solar.handle(_SAMPLEDATA[i])) {
      solar.debug2Serial(Serial);
    }
  }

  delay(60000);
}