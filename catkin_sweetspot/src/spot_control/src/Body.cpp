
#include "spotmicro.h" 

static char id[]="$id$"; 

#include "Body.h"

#include <std_msgs/Float32MultiArray.h>


void Body::init(int _step){
  transient=0; 
  // Foots initialize
  steps=_step;
    
  RL.init(steps);
  RL.sv[0].init( 1, -45, 45,416,220, 0);
  RL.sv[1].init( 2, -20, 90,526,285,60);
  RL.sv[2].init( 3,  30,180,182,509,60);

  RR.init(steps);
  RR.sv[0].init( 5, -45, 45,217,414, 0);
  RR.sv[1].init( 6, -20, 90,124,364,60);
  RR.sv[2].init( 7,  30,180,455,128,60);
    
  FL.init(steps);
  FL.sv[0].init( 9, -45, 45,214,410, 0);
  FL.sv[1].init(10, -20, 90,514,274,60);
  FL.sv[2].init(11,  30,180,188,515,60);
    
  FR.init(steps);
  FR.sv[0].init(13, -45,45 ,408,212, 0);
  FR.sv[1].init(14, -20,90 ,138,379,60);
  FR.sv[2].init(15,  30,180,443,116,60);
    
  FR.floor(100,0,20);  // hight , width , lift 
  FL.floor(100,0,20);
  RR.floor(100,0,20);
  RL.floor(100,0,20);

  // foot rotation 
  FR.setPhase(0);
  RL.setPhase(90);
  FL.setPhase(180);
  FR.setPhase(240);
    
};


int Body::command(int cmd){
  switch(cmd){
  case CMD_STP:
    commandStep(); break;
  case CMD_SIT:
    commandSit();  break;
  case CMD_STD:
    commandStand();break;
  case CMD_FWD:
    commandForward(); break;
  case CMD_STO:
    commandStop(); break;
  case CMD_BCK:
    commandBack(); break;
  case CMD_LFT:
    commandLeft(); break;
  case CMD_CTR:
    commandCenter(); break;
  case CMD_RGT:
    commandRight(); break;
  case CMD_LIE:
    commandLieDown(); break;
    
  case CMD_END:
  default:
    break;
  }

}



void Body::commandStep(void){
  
}

//
void Body::commandSit(void){
}

//
void Body::commandStand(void){
}

//
void Body::commandForward(void){
}

//
void Body::commandStop(void){
}

//
void Body::commandBack(void){
}

//
void Body::commandLeft(void){
}

//
void Body::commandCenter(void){
}

//
void Body::commandRight(void){
}

// 
void Body::commandLieDown(void){
  // FR.setAbsolute(308,197,450);
  // RR.setAbsolute(316,187,487);
  // RL.setAbsolute(306,460,188);
  // RR.setAbsolute(316,461,178);
  FR.position(0,90,30);
  FL.position(0,90,30);
  RR.position(0,90,30);
  RL.position(0,90,30);
} 




int Body::setServoAbs( i2cpwm_board::ServoArray * pSrv){
  int cnt=0; 
  for (int i=0;i<pSrv->servos.size(); i++){
    if (pSrv->servos[i].servo > 1){
      cnt++;
    }
  }
  absPosition.servos.resize(cnt);
  int j=0;
  for (int i=0 ; i< pSrv->servos.size() ; i++){
    if (pSrv->servos[i].servo >1){
      absPosition.servos[j].servo=pSrv->servos[i].servo;
      absPosition.servos[j].value=pSrv->servos[i].value;
      j++;
    }
  }
}
      

void Body::updateClock(){
  i2cpwm_board::ServoArray * pSrv;
  
  if (transient>0){
    pSrv=&transientServos[transientStep-transient];
    transient++;
    setServoAbs(pSrv);
  }else{
    // update each servo values
    FR.updateClock(clocktick);
    FL.updateClock(clocktick);
    RR.updateClock(clocktick);
    RL.updateClock(clocktick);
    //setCurrent();
  }
  clocktick++;
  if (clocktick > transientStep*1024*1024 ){
    clocktick=0;
  }

}


i2cpwm_board::ServoArray * Body::getAbsolute(void){

  for (int i=0;i<3;i++){
    absPosition.servos[0+i].servo=FR.sv[0].chNo;
    absPosition.servos[0+i].value=FR.sv[i].value;
    absPosition.servos[3+i].servo=FL.sv[0].chNo;
    absPosition.servos[3+i].value=FL.sv[i].value;
    absPosition.servos[6+i].servo=RR.sv[0].chNo;
    absPosition.servos[6+i].value=RR.sv[i].value;
    absPosition.servos[9+i].servo=RL.sv[0].chNo;
    absPosition.servos[9+i].value=RL.sv[i].value;
  }
  return &absPosition;
}

void setAbsolute( i2cpwm_board::ServoArray *pSrv){
  pSrv->servos.resize(12);
  
}
