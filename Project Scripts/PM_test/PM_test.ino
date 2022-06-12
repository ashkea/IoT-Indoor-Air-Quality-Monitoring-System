// Nag-modify ako ng konti based sa naging experience ko. I would recommend to use this code snippet.
// Ito yung online tutorial na pinagbasihan ko: https://www.hackster.io/shahizat005/building-an-esp32-based-iot-weather-station-with-ubidots-05adcc

#include <Arduino.h>

// To install this library, go to Sketch > Include Library > Manage Libraries tapos search niyo "software serial"
// then install yung "EspSoftwareSerial" by Dirk Kaar, Peter Lerup
#include "SoftwareSerial.h"

// To install this library, go to Sketch > Include Library > Manage Libraries tapos search niyo "pms library"
// then install yung "PMS Library" by Mariusz Kacki
#include "PMS.h"

// Kinabit ko yung PMS7003 RX saka TX sa pin 15 and 19 nung microprocessor, pero you can pick any GPIO pin
// Check here for pinout: https://www.upesy.com/blogs/tutorials/esp32-pinout-reference-gpio-pins-ultimate-guide
#define PMS7003_TX 33
#define PMS7003_RX 32

// Don't change this
#define PMS7003_BAUD 9600

SoftwareSerial pms_ss(PMS7003_TX, PMS7003_RX);
PMS pms(pms_ss);
PMS::DATA pms_data;

void setup() {
  Serial.begin(115200);
  pms_ss.begin(PMS7003_BAUD);
}

void loop() {
  while (pms_ss.available()) { 
    pms_ss.read(); 
  }
  pms.requestRead();
  if (pms.readUntil(pms_data)) {
    Serial.print("PM 1.0 (ug/m3): "); 
    Serial.println(pms_data.PM_AE_UG_1_0);
    Serial.print("PM 2.5 (ug/m3): "); 
    Serial.println(pms_data.PM_AE_UG_2_5);
    Serial.print("PM 10.0 (ug/m3): "); 
    Serial.println(pms_data.PM_AE_UG_10_0);
  }
}
