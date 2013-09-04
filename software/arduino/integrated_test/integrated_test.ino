//=== Configuration ==========================================================//
// Comment out unecessary/unconnected components
// #defines used to remove components from compilation (should happen) rather 
// than a check which will leave code in place leading to a bloated file

#define DEBUG       // Open up USB serial connection and other debug info
#define MEGA1280    // Broken SPI pins so turn off CAN and SD

#define CAN_ON      // Using the CAN bus
#define LCD_ON      // Using the serial LCD
#define IMU_ON      // Using the IMU/MPU
#define SD_ON       // Logging to SD
#define RTC_ON      // Using real time clock
#define JOYSTICK_ON // Using joystick. Turn off if using a UNO

//============================================================================//

//=== Libraries ==============================================================//
// Include libraries of components being used.

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
#endif

#ifdef RTC_ON
    #include <Wire.h>
    #include <Rtc_Pcf8563.h>
#endif
//============================================================================//

//=== Init ===================================================================//
// Initialise vairables for each component

// On board LED
int LED1 = 13;

// Timing count
int count = 0;

#ifdef MEGA1280
    //=== CAN ===//
    #ifdef CAN_ON
        // CAN LEDs
        int LED2 = 8;
        int LED3 = 7;
    #endif
    
    //=== SD ===//
    #ifdef SD_ON
        const int chipSelect = 9;

        File dataFile;
    #endif
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

    void clear_lcd(void);    
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
    void printQuaternion(File input, long *quat);
    void printQuaternion(File input, float *quat);
#endif

//=== SETUP ==================================================================//

void setup()
{
  // Serial debugging port 
  Serial.begin(9600);
  Wire.begin();
  
  //=============RTC===============//
  //clear out all the registers
  rtc.initClock();
  //set a time to start with.
  //day, weekday, month, century, year
  rtc.setDate(14, 6, 3, 20, 10);
  //hr, min, sec
  rtc.setTime(1, 15, 40);
  
  //==
  MPU.selectDevice(DEVICE_TO_USE);                        // only really necessary if using device 1
  MPU.init(MPU_UPDATE_RATE, MPU_MAG_MIX_GYRO_AND_MAG, MAG_UPDATE_RATE, MPU_LPF_RATE);   // start the MPU
  
  //=============SD CARD===============//
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
  
  pinMode(LCDOUT, OUTPUT);
  
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT); 
 
  digitalWrite(LED2, LOW);
  pinMode(UP,INPUT);
  pinMode(DOWN,INPUT);
  //pinMode(LEFT,INPUT);
  pinMode(RIGHT,INPUT);
  //pinMode(CLICK,INPUT);
  
  digitalWrite(UP, HIGH);       /* Enable internal pull-ups */
  digitalWrite(DOWN, HIGH);
  //digitalWrite(LEFT, HIGH);
  digitalWrite(RIGHT, HIGH);
  //digitalWrite(CLICK, HIGH);
  
  sLCD.begin(9600);              /* Setup serial LCD and clear the screen */
  clear_lcd();
  sLCD.print("Starting Log");
  
  delay(1500);
}
//============================================================================//

//=== Main Loop ==============================================================//
void loop()
{
  
  if( count >= 16000){
    digitalWrite(LED3, HIGH);
    
    cur_time = rtc.formatTime();
  
    Serial.print("[");
    Serial.print(cur_time);
    Serial.print("]: ");
    
    dataFile.print("[");
    dataFile.print(cur_time);
    dataFile.print("]: ");
    
    clear_lcd();
    sLCD.print(COMMAND);
    sLCD.print(LINE0);
    sLCD.print(cur_time);
    
    MPU.selectDevice(DEVICE_TO_USE);                         // only needed if device has changed since init but good form anyway
    if (MPU.read()) {                                        // get the latest data if ready yet
  //  MPU.printQuaternion(MPU.m_rawQuaternion);              // print the raw quaternion from the dmp
  //  MPU.printVector(MPU.m_rawMag);                         // print the raw mag data
  //  MPU.printVector(MPU.m_rawAccel);                       // print the raw accel data
  //  MPU.printAngles(MPU.m_dmpEulerPose);                   // the Euler angles from the dmp quaternion
  //  MPU.printVector(MPU.m_calAccel);                       // print the calibrated accel data
  //  MPU.printVector(MPU.m_calMag);                         // print the calibrated mag data
      MPU.printAngles(MPU.m_fusedEulerPose);                 // print the output of the data fusion
      
      printQuaternion(dataFile,MPU.m_fusedEulerPose);
      
    }
    Serial.println();
    dataFile.println();

    count = 0;
    
     
    digitalWrite(LED3, LOW);
  }
  
  //Serial.println(count);
  
  count++;
  if (digitalRead(UP) == 0){
    Serial.println("Stopping functionality");
    clear_lcd();
    
    dataFile.flush();
      
    sLCD.print("OFF");
    dataFile.close();
    while(1);
  }
  
//  delay(1000);

}
//============================================================================//

//== Aux functions ===========================================================//

#ifdef LCD_ON
    void clear_lcd(void)
    {
      sLCD.write(COMMAND);
      sLCD.write(CLEAR);
    }
#endif

#ifdef IMU
    void printQuaternion(File input, long *quat)
    {
      input.print("w: "); input.print(quat[QUAT_W]);  
      input.print(" x: "); input.print(quat[QUAT_X]);  
      input.print(" y: "); input.print(quat[QUAT_Y]);  
      input.print(" z: "); input.print(quat[QUAT_Z]);  
    }

    void printQuaternion(File input, float *quat)
    {
      input.print("w: "); input.print(quat[QUAT_W]);  
      input.print(" x: "); input.print(quat[QUAT_X]);  
      input.print(" y: "); input.print(quat[QUAT_Y]);  
      input.print(" z: "); input.print(quat[QUAT_Z]);  
    }
#endif 
//============================================================================//
