//
// Body.h : spot body control 
//  calcurate servo position from state and clock
//
//

#ifndef __BODY_H__
#define __BODY_H__ "$id"
#pragma WARN("Body.h")

#include "spotmicro.h"
#include "i2cpwm_board/ServoArray.h"
#include "i2cpwm_board/Servo.h"
#include "std_msgs/String.h"
#include <string.h>


#define transientStep 24

#define Servo_FR 13
#define Servo_FL 9
#define Servo_RR 5
#define Servo_RL 1

class Body {
  
 private:
  Leg FR,FL,RR,RL;
  void commandStep(void);
  void commandSit(void);
  void commandStand(void);
  void commandForward(void);
  void commandStop(void);
  void commandBack(void);
  void commandLeft(void);
  void commandCenter(void);
  void commandRight(void);
  void commandLieDown(void);
  int transient;
  float battery;
  i2cpwm_board::ServoArray currentPosition;
  i2cpwm_board::ServoArray absPosition;
  i2cpwm_board::ServoArray transientServos[transientStep];
  unsigned  int clockTick;
  int steps;
  int clocktick;
 public:
  
  void init(int);
  enum state {ST_POWERD, ST_SIT, ST_LIE, ST_STAND, ST_STEP};
  int command(int state);
  void updateClock();
  int setServoAbs(i2cpwm_board::ServoArray * pSrv);
  int buildLcdMesssage(std_msgs::String *l1, std_msgs::String *l2);
  i2cpwm_board::ServoArray * setCurrent();
  i2cpwm_board::ServoArray * getAbsolute(void);

};


	      
    
	      
#endif 
