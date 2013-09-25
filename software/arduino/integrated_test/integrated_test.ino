//=== Configuration ==========================================================//
// Comment out unnecessary/unconnected components
// #defines used to remove components from compilation (should happen) rather 
// than a check which will leave code in place leading to a bloated file

#define DEBUG       // Open up USB serial connection and other debug info
//#define MEGA1280    // Broken SPI pins so turn off CAN and SD

#define CAN_ON        // Using the CAN bus
#define LCD_ON        // Using the serial LCD
#define IMU_ON        // Using the IMU/MPU
#define SD_ON         // Logging to SD
#define RTC_ON        // Using real time clock
#define JOYSTICK_ON   // Using joystick. Turn off if using a UNO
#define STRING_POT_ON //Use string pots

//============================================================================//

//=== Libraries ==============================================================//
// Include libraries of components being used.

#include "pin_defs.h"

#ifndef MEGA1280
    #ifdef CAN_ON
        #include <SPI.h>
    #endif
    
    #ifdef SD_ON
        #include <SPI.h>
        #include <SD.h>
    #endif
#endif

#ifdef LCD_ON
    #include <SoftwareSerial.h>
    #include "lcd.h"
#endif
    
#ifdef IMU_ON
    #include <Wire.h>
    #include "I2Cdev.h"
    #include "MPU9150Lib.h"
    #include "CalLib.h"
    #include <dmpKey.h>
    #include <dmpmap.h>
    #include <inv_mpu.h>
    #include <inv_mpu_dmp_motion_driver.h>
    #include <EEPROM.h>
    #include "mpu.h"
#endif

#ifdef RTC_ON
    #include <Wire.h>
    #include <Rtc_Pcf8563.h>
#endif

#ifdef STRING_POT_ON
    #include "string_pot.h"
#endif
//============================================================================//

//=== Init ===================================================================//
// Initialise vairables for each component

// On board LED
int LED1 = 13;

// Timing count
int count = 0;

// Prints to serial checking debug flag
void serialPrint(char* out);

//=== CAN ===//
#ifdef CAN_ON
    // CAN LEDs
#endif

//=== SD ===//
#ifdef SD_ON
    const int chipSelect = 9;

    File dataFile;
#endif

//=== RTC ===//
#ifdef RTC_ON
    //init the real time clock
    Rtc_Pcf8563 rtc;

    // Keep track of time, used to allow track time between seconds
    char *cur_time, *prev_time;
#endif

//=== LCD ===//
#ifdef LCD_ON
    int LCDOUT = 6;
    int LCDIN = 3; //unused but required for SoftwareSerial

    SoftwareSerial sLCD =  SoftwareSerial(LCDIN, LCDOUT);
    
    #define COMMAND 0xFE
    #define CLEAR   0x01
    #define LINE0   0x80
    #define LINE1   0xC0
    
#endif

//=== Joystick ===//
#ifdef JOYSTICK_ON
    #define UP     A1
    #define RIGHT  A2
    #define DOWN   A3
    #define CLICK  A4
    #define LEFT   A5
#endif

//=== IMU ===//
#ifdef IMU_ON
    //  DEVICE_TO_USE selects whether the IMU at address 0x68 (default) or 0x69 is used
    //    0 = use the device at 0x68
    //    1 = use the device at ox69

    #define  DEVICE_TO_USE    0

    MPU9150Lib MPU;                             // the MPU object
    float* pose;

    //  MPU_UPDATE_RATE defines the rate (in Hz) at which the MPU updates the sensor data and DMP output

    #define MPU_UPDATE_RATE  (20)

    //  MAG_UPDATE_RATE defines the rate (in Hz) at which the MPU updates the magnetometer data
    //  MAG_UPDATE_RATE should be less than or equal to the MPU_UPDATE_RATE

    #define MAG_UPDATE_RATE  (10)

    //  MPU_MAG_MIX defines the influence that the magnetometer has on the yaw output.
    //  The magnetometer itself is quite noisy so some mixing with the gyro yaw can help
    //  significantly. Some example values are defined below:

    #define  MPU_MAG_MIX_GYRO_ONLY          0   // just use gyro yaw
    #define  MPU_MAG_MIX_MAG_ONLY           1   // just use magnetometer and no gyro yaw
    #define  MPU_MAG_MIX_GYRO_AND_MAG       10  // a good mix value 
    #define  MPU_MAG_MIX_GYRO_AND_SOME_MAG  50  // mainly gyros with a bit of mag correction 

    //  MPU_LPF_RATE is the low pas filter rate and can be between 5 and 188Hz

    #define MPU_LPF_RATE   40

    //  SERIAL_PORT_SPEED defines the speed to use for the debug serial port

    #define  SERIAL_PORT_SPEED  9600

    // print quarternion to files
    //void printQuaternion(File input, long *quat);
    //void printQuaternion(File input, float *quat);
#endif

#ifdef STRING_POT_ON
  float mast_height=0;
  float mast_reach=0;
#endif

unsigned long cur_t;
unsigned long past_t;

//=== SETUP ==================================================================//

void setup()
{
  // Serial debugging port 
  Serial.begin(9600);
  Wire.begin();
  
  //===RTC===//
  // DON'T DO IF RTC ALREADY CONFIGURED
  
  //clear out all the registers
  //rtc.initClock();
  //set a time to start with.
  //day, weekday, month, century, year
  //rtc.setDate(14, 6, 3, 20, 10);
  //hr, min, sec
  //rtc.setTime(1, 15, 40);
  
  #ifdef IMU_ON
  //===MPU===//
  // Only necessary if using multiple IMU
  MPU.selectDevice(DEVICE_TO_USE); 
  // Start MPU
  MPU.init(MPU_UPDATE_RATE, MPU_MAG_MIX_GYRO_AND_MAG,
    MAG_UPDATE_RATE, MPU_LPF_RATE);
  #endif
  
  #ifdef SD_ON
  //===SD CARD====//
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");
  
  // Open up the file we're going to log to!
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }
  #endif
  
  #ifdef CAN_ON
  //===CAN===//
  pinMode(CAN_LED1, OUTPUT); 
  pinMode(CAN_LED2, OUTPUT); 
  #endif
  
  #ifdef LCD_ON
  //===LCD===//
  pinMode(LCDOUT, OUTPUT);
  sLCD.begin(9600);              /* Setup serial LCD and clear the screen */
  clear_lcd(sLCD);
  sLCD.print("Starting Log");
  #endif
  
  #ifdef JOYSTICK_ON
  //===JOYSTICK===//
  // Set joystick inputs
  pinMode(UP,INPUT);
  pinMode(DOWN,INPUT);
  pinMode(LEFT,INPUT);          //Clash on the UNO with I2C pins
  pinMode(RIGHT,INPUT);
  pinMode(CLICK,INPUT);         //Clash on the UNO with I2C pins
  
  // Enable internal pull-ups (active low)
  digitalWrite(UP, HIGH);       
  digitalWrite(DOWN, HIGH);
  digitalWrite(LEFT, HIGH);     //Clash on the UNO with I2C pins
  digitalWrite(RIGHT, HIGH);
  digitalWrite(CLICK, HIGH);    //Clash on the UNO with I2C pins
  #endif
  
  #ifdef STRING_POT_ON
    generate_offset(MAST_HEIGHT, &height_offset, 500);
//    generate_offset(MAST_REACH, reach_offset);
    Serial.print("Calibrating mast height - offset: ");
    //Serial.println(MAST_HEIGHT_RES);
    Serial.println(height_offset);
  #endif
  
  // Wait before starting
  delay(2000);
}
//============================================================================//

//=== Main Loop ==============================================================//
void loop()
{

  // Slowing down poll rate
  if( count >= 100){
    // Light an LED
    digitalWrite(LED1, HIGH);
    
    // Get all data
    // Get Time
    #ifdef RTC_ON
        cur_time = rtc.formatTime();
    #else
        cur_time = "---";
    #endif
    
    #ifdef IMU_ON
    // Get pose
        if (MPU.read()) {                                   // get the latest data if ready yet
            pose = MPU.m_fusedEulerPose;
        }
    #else
        pose = -1;
    #endif
    
    #ifdef STRING_POT_ON
      mast_height = get_mast_height(MAST_HEIGHT);
    #endif
    
    // Turn off LED
    digitalWrite(LED1, LOW);
  
//===========Display==========================================================//
    // Print data
    #ifdef DEBUG
        Serial.print("[");
        Serial.print(cur_time);
        Serial.print("]:: ");
        Serial.print(" Pose:");
        MPU.printAngles(pose);  // print to Serial (no endl)
        Serial.print(" time: ");
        past_t=cur_t;
        cur_t=millis();
        Serial.print(cur_t - past_t);
        Serial.print(" Height: ");
        Serial.print(mast_height);
        Serial.println();
    #endif
    
    #ifdef SD_ON
        dataFile.print("[");
        dataFile.print(cur_time);
        dataFile.print("]:: ");
        dataFile.print(" Pose:");
        writeAngle(dataFile,pose);  // print to file (no endl)
        dataFile.print(" Height: ");
        dataFile.print(mast_height);
        dataFile.println();
    #endif
    
    #ifdef LCD_ON
        //resetLCD(sLCD);
        clear_lcd(sLCD);
        sLCD.write(COMMAND);
        sLCD.write(LINE0);
        sLCD.print(cur_time);
    #endif 

    count = 0;
    
    // Remember last time 
    prev_time = cur_time;
  }
  
  count++;
  
  #ifdef JOYSTICK_ON
      if (digitalRead(CLICK) == 0){
        #ifdef DEBUG
            Serial.println("Stopping functionality");
        #endif
        
        #ifdef SD_ON
            dataFile.flush();
            dataFile.close();        
        #endif 
        
        #ifdef LCD_ON
            clear_lcd(sLCD);
            sLCD.print("OFF");
        #endif
        
        // Do nothing
        // TODO: change this to click on/off
        while(1);
      }
  #endif
  
}
//============================================================================//

//== Aux functions ===========================================================//

void serialPrint(char* out){
   #ifdef DEBUG 
    Serial.print(out);
   #endif
}
//============================================================================//
