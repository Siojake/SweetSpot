//#include "i2cpwm_board/Servo.h"
#include "i2cpwm_board/ServoArray.h"
#ifndef LEG_H 

#define LEG_H $id$ 
#pragma WARN("Leg.h")

#define TRANSIENTS 12

class Leg{
  int steps;
  int height;
  int width;
  int lift;
  
  float L1=80;
  float L2=102;
  
  struct point{
    float a;   // position 
    float b;   // position
    float px;
    float py;
    float t1;  // L1 angle
    float t2;  // L2 angle
    int s1;    // calucrated servo angle
    int s2;    // calucrated servo angle
  };
 public:
  Servo sv[3];
  i2cpwm_board::Servo absPos[3];
  i2cpwm_board::Servo transientBuf[TRANSIENTS][3];
  
  int phase=0;
  
  point * pos =NULL;
  char buf[256];
  int shoulder=0;               // shoulder shoud be -45 to 45
  int transient=0;
  
 public:

  void init(int _step);
  void setAbsolute( int s1, int s2,int s3);
  void floor(int _h,int _w, int _l);
  void setPhase(int deg);
  int currentPos(int clock);
  float link1( int pha );
  float link2( int pha );
  void updateClock(int tick);
  void position( float s1, float s2, float s3);
  
  
};

#endif 
