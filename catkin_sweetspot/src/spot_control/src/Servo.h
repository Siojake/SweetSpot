
#ifndef SERVO_H
#define SERVO_H $id$
#pragma WARN("Servo.h")


#include "spotmicro.h" 

class Servo{
 public:
  struct _angle {
    float min;
    float center;
    float max;
  } angle;
  struct _val {
    float min;
    float center;
    float max;
    float step;
  } config;
  int value;
  int chNo;
  int rest;
  void init(int i2cno,float amin,float acen,float amax ,
	    float vmin ,float vcen,float vmax,float _rest);
  int position(float _angle);
  i2cpwm_board::Servo servo;
  int absolute(int absolute);
} ; // class
#endif 
