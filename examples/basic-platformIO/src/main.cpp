#include <Arduino.h>
#include <HardwareSerial.h>
#include "VotronicSRDuoDig.h"
VotronicSRDuoDig solar;

#define VT_RXD 4
#define VT_TXD -1
HardwareSerial VRSerial(1);

void setup() {
  Serial.begin(115200);

  VRSerial.begin(_VOTRSERIALSPEED, SERIAL_8N1, VT_RXD, VT_TXD);
  VRSerial.setTimeout(2);

  delay(250);
}

void loop() {
  // give Votronic some time to send data
  for (int i = 0; i < 100; i++) {
    solar.handle(VRSerial);
    delay(2);
  }

  // in case a valid frame has been received, show it
  if (solar.isValid()) {
    solar.print2serial(Serial);
  }

  delay(1000);
}
