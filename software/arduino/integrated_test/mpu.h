#ifndef MPU_H

#define MPU_H 

#include "MPU9150Lib.h"
#include <SD.h>


// Following functions are adapted from the serial print from MPU9150Lib.h

// Writes the pose to a file

void writeAngle(File input, float *vec)
{
  input.print("x: "); input.print(vec[VEC3_X] * RAD_TO_DEGREE);  
  input.print(" y: "); input.print(vec[VEC3_Y] * RAD_TO_DEGREE);  
  input.print(" z: "); input.print(vec[VEC3_Z] * RAD_TO_DEGREE);    
}

#endif
