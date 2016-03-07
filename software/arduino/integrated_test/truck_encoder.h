/** Encapsulates Encoder.h and provides functionality for two encoder reads and distance measure
 */

#ifndef TRUCK_ENC_H

#define TRUCK_ENC_H 

#include <Encoder.h>

#define ENC_COUNTS_PER_REV 400

#define PASSIVE_WHEEL_DIAMETER 285 // mm
#define DRIVEN_WHEEL_DIAMETER 330

#define PASSIVE_WHEEL_RADIUS PASSIVE_WHEEL_DIAMETER/2.0
#define DRIVEN_WHEEL_RADIUS DRIVEN_WHEEL_DIAMETER/2.0

#define PI 3.1415926535897932384626433

class Truck_Enc
{
  
  public:
    Truck_Enc(float _encoder_radius, uint8_t left1, uint8_t left2, uint8_t right1, uint8_t right2)
      :enc_left(left1,left2), enc_right(right1,right2){
      
      encoder_radius = _encoder_radius; // For gearing
      calc_counts_per_rev();
    }
    
    void calc_counts_per_rev(void){
      counts_per_rev = ENC_COUNTS_PER_REV*(PASSIVE_WHEEL_RADIUS/encoder_radius);
    }
    
    void calc_dist_per_count(void){
      dist_per_count = (PI*PASSIVE_WHEEL_DIAMETER)/counts_per_rev;
    }
    
    
    /** Read Encoder values
     *  
     */
    void read(void){
      left_position_new = enc_left.read();
      right_position_new = enc_right.read();
      
      left_diff = left_position_old - left_position_new;
      right_diff = right_position_old - right_position_new;
      
      left_acc += left_diff;
      right_acc += right_diff;
      
      if(left_diff != 0){
        left_position_old = left_position_new;
      }
      if(right_diff != 0){
        right_position_old = right_position_new;
      }
    }
    
    // Function to calculate average linear speedd (average of two values)
    
    // Function to calculate omega
    
    // Calculate theta
    
    // Calulate x, y velocties
    
    long getLeft(void){
      long ret_val = left_acc;
      left_acc = 0;
      return ret_val;
    }
    long getRight(void){
      long ret_val = right_acc;
      right_acc = 0;
      return ret_val;
    }
  
  private:
    Encoder enc_left;
    Encoder enc_right;
    
    long left_position_new;
    long left_position_old;
    long left_diff;
    long left_acc;
    
    long right_position_new;
    long right_position_old;
    long right_diff;
    long right_acc;
    
    float encoder_radius;
    float counts_per_rev;
    float dist_per_count;
    
};

#endif
