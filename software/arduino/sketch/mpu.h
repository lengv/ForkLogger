#ifndef MPU_H

#define MPU_H 

#include "MPU9150Lib.h"


// Following functions are adapted from the serial print from MPU9150Lib.h

// Writes quaternion to file
void writeQuarternion(File input, long *quat)
{
  input.print("w: "); input.print(quat[QUAT_W]);  
  input.print(" x: "); input.print(quat[QUAT_X]);  
  input.print(" y: "); input.print(quat[QUAT_Y]);  
  input.print(" z: "); input.print(quat[QUAT_Z]);  
}

void writeQuarternion(File input, float *quat)
{
  input.print("w: "); input.print(quat[QUAT_W]);  
  input.print(" x: "); input.print(quat[QUAT_X]);  
  input.print(" y: "); input.print(quat[QUAT_Y]);  
  input.print(" z: "); input.print(quat[QUAT_Z]);  
}


#endif
