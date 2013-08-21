

#include <SPI.h>

// Updated SD library
#include <SD.h>
// From skpang http://www.skpang.co.uk/catalog/arduino-canbus-shield-with-usd-card-holder-p-706.html
#include <Canbus.h>

#include "pin_defs.h"

// Set the baud rate
can_speed = CANSPEED_250;

// Variables for SD card logging
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;


void setup() {

}

void loop() {
}

