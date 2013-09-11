// lcd.h contains functions and definitions for use of a serial LCD. 
// Note: header only (all functionality in header)

#ifndef LCD_H

#define LCD_H 

#include <SoftwareSerial.h>

// Commands to be sent to LCD
#define COMMAND 0xFE	// Not sure what this is for
#define CLEAR   0x01	// Clear display
#define LINE0   0x80	// Go to first line
#define LINE1   0xC0	// Go to second line

// Clear the display
void clear_lcd(SoftwareSerial sLCD)
{
	sLCD.write(COMMAND);
	sLCD.write(CLEAR);
}  

// Reset the LCD if garbage is appearing on the display. Do this on set up. 
// Runs the command several times to ensure reset (assuming possible transmission failure)
void resetLCD(SoftwareSerial sLCD) { 
	int i=0;
	for(i=0 ; i < 100 ; i++){
		sLCD.write(0x07); 
		sLCD.write(18); 
	}
} 

#endif
