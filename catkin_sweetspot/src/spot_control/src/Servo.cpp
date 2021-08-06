#include "spotmicro.h"

static char id[]="$id$";

void Servo::init(int i2cno,float amin,float acen,float amax ,
		 float vmin ,float vcen,float vmax,float _rest){
    chNo=i2cno;
    angle.min=amin;
    angle.center=acen;
    angle.max=amax;
    config.min=vmin;
    config.center=vcen;
    config.max=vmax;
    config.step=(amax-amin)/(vmax-vmin);
    rest=_rest;
  }

int Servo::position(float _angle){
  float ret=0.0;
  if (_angle <= angle.min){
    _angle=angle.min;
  }
  if (angle.max <= _angle){
    _angle=angle.max;
  }
  ret=(config.max-config.min)/(angle.max-angle.min)*
    (_angle-angle.min)+config.min;
  absolute((int)ret);
  return int(ret);
}

int Servo::absolute(int val){
  value=val;
  printf("Sservo::absolute(%d:%d)\n",chNo,value);
}
