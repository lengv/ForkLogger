#ifndef STRING_POT_H

#define STRING_POT_H

const float ad_resolution = 4.8821; //(mV/bit)
const float vcc = 5.0;

const float mast_height_res = 3.826/25.4*vcc; // Note 25.4 is for the inch conversion
const float mast_reach_res = 0.73433*vcc;

// Offset if calibration is required at beginning
float height_offset = 0;
float reach_offset  = 0;

float get_mast_height(int pin){
  int sensorValue = analogRead(pin);
  float distance = (float)(sensorValue+height_offset)*ad_resolution/mast_height_res;
  
  return distance;
}

float get_mast_reach(int pin){
  int sensorValue = analogRead(pin);
  float distance = (float)sensorValue*ad_resolution/mast_reach_res + reach_offset;
  
  return distance;
}

void generate_offset(int pin, float* offset, int counts){
  int i=0;
  
  unsigned int sum=0;
  int sV;
  
  for(i=0 ; i<counts ; i++){
    sV = analogRead(pin);
    Serial.println(sV);
    sum = sum+sV;
    
  }
  Serial.println(sum);
  *offset = -(float)sum/(float)i;
  
  return;
}

#endif
