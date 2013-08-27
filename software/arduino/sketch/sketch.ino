#include <SPI.h>

// Updated SD library
#include <SD.h>
// From skpang http://www.skpang.co.uk/catalog/arduino-canbus-shield-with-usd-card-holder-p-706.html
#include <Canbus.h>

// http://playground.arduino.cc/Main/RotaryEncoders
// Not sure if this works details on issue:
//   http://forum.arduino.cc/index.php?PHPSESSID=m0a63pcq8l94uvk3j3a24e9ms4&topic=163847.msg1225369#msg1225369

#define ENCODER_OPTIMIZE_INTERRUPTS // extra speed
#include <Encoder.h>

#include "pin_defs.h"

// Set the baud rate
int can_speed = CANSPEED_250;

// Variables for SD card logging
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;


void setup() {

}

void loop() {
}

