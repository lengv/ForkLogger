// Pin definitions - mirrored in ../../hardware/pin_defs.xls

#ifndef PIN_DEF_H

#define PIN_DEF_H 

/* Define Joystick connection */
#define UP     A1
#define RIGHT  A2
#define DOWN   A3
#define CLICK  A4
#define LEFT   A5

// SPI pins
#define CAN_CS  10
#define SD_CS   9
//#define IMU_CS   53

// Analog inputs
#define MAST_HEIGHT  A7
#define MAST_REACH   A6
#define LOAD         A7

// CAN Shield
//#define CAN_INT  2
#define CAN_LED1 7
#define CAN_LED2 8

// LCD
#define LCD 6

// Encoders
#define A1_INT 23
#define A2_INT 24
#define B1_INT 2
#define B2_INT 3
//#define B1_INT 27
//#define B2_INT 28

//#define COUNT_A 23
//#define COUNT_B 24
//#define DIR_A   27
//#define DIR_B   28

#endif
