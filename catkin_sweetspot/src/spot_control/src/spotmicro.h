
#ifndef SPOTMICRO_H
#define SPOTMICRO_H "$id"


#include <stdio.h>
#include <math.h>
#include "i2cpwm_board/Servo.h"
#include "i2cpwm_board/ServoArray.h"
#include "i2cpwm_board/SpotStatus.h"

#include "Servo.h"
#include "Leg.h"
#include "Body.h"






enum spotCommand   {
  CMD_STP,CMD_SIT,CMD_STD,CMD_FWD,CMD_STO,
  CMD_BCK,CMD_LFT,CMD_CTR,CMD_RGT,CMD_LIE,CMD_END};

//#define transientStep 24
#define transientStep 24

#endif 
