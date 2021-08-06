#include "spotmicro.h"

static char id[] ="$id$";

void Leg::init(int _s){
  if (pos !=NULL){
    delete[] pos;
  }
  steps=_s;
  pos= new point[steps];

  for (int i=0 ; i< steps ; i++){
    pos[i].a=0;
    pos[i].b=0;
    pos[i].t1=0;
    pos[i].t2=0;
    pos[i].s1=0;
    pos[i].s2=0;
  }
  shoulder=0;
  transient=0;
}

void Leg::floor(int _h, int _w ,int _l){
  width  =_w;
  height =_h;
  lift   =_l;

  float wstep=(steps/4.0*3.0);
  float mstep=steps/4.0/3;
  float lstep=steps/4.0/3;
  
  float widthdiff= width/(float)wstep;
  float movediff = width/(float)mstep;
  float liftdiff = lift/(float)lstep;

  float x,y;
  for (int i=0;i<steps; i++){

    // slide division
    if (i<wstep){
      if (i==0){
	x=-width/2;
	y=0;
      }
      pos[i].a=x;
      pos[i].b=y;
      x=x+widthdiff;
    }

    // lift division
    if ((wstep<=i) && ( i < (wstep +lstep) )){
      pos[i].a=x;
      pos[i].b=y;
      y=y+liftdiff;
    }

    // move division
    if (((wstep+lstep)<=i )&&( i < (wstep +2*lstep) )){
      pos[i].a=x;
      pos[i].b=y;
      x=x-movediff;
    }

    // drop division
    if (((wstep +2*lstep)<=i) && ( i < steps )){
      pos[i].a=x;
      pos[i].b=y;
      y=y-liftdiff;
    }
    float px,py;
    float a=pos[i].a,b=pos[i].b;
    
    px=(pow(a,3)+sqrt(-1*pow((b-height),2)*(pow(a,4)+2*pow(a,2)*pow(b,2)-4*pow(a,2)*b*height+2*pow(a,2)*pow(height,2)-2*pow(a,2)*pow(L1,2)-2*pow(a,2)*pow(L2,2)+pow(b,4)-4*pow(b,3)*height+6*pow(b,2)*pow(height,2)-2*pow(b,2)*pow(L1,2)-2*pow(b,2)*pow(L2,2)-4*b*pow(height,3)+4*b*height*pow(L1,2)+4*b*height*pow(L2,2)+pow(height,4)-2*pow(height,2)*pow(L1,2)-2*pow(height,2)*pow(L2,2)+pow(L1,4)-2*pow(L1,2)*pow(L2,2)+pow(L2,4)))+a*pow(b,2)-2*a*b*height+a*pow(height,2)+a*pow(L1,2)-a*pow(L2,2)	)/(2*(pow(a,2)+pow(b,2) -2*b*height+pow(height,2)));
    
    // py=(pow(a,2)*pow(b,2)-pow(a,2)*pow(height,2)-a*sqrt
    // 	(-1*pow((b-height),2)*
    // 	 (
    // 	  pow(a,4)+2*pow(a,2)*pow(b,2)
    // 	  -4*pow(a,2)*b*height+2*pow(a,2)*pow(height,2)-2*pow(a,2)*pow(L1,2)-2*pow(a,2)*pow(L2,2)+pow(b,4)
    // 	  -4*pow(b,3)*height
    // 	  +6*pow(b,2)*pow(height,2)-2*pow(b,2)*pow(L1,2)-2*pow(b,2)*pow(L2,2)
    // 	  -4*b*pow(height,3)+4*b*height*pow(L1,2)
    // 	  +4*b*height*pow(L2,2)+pow(height,4)-2*pow(height,2)*pow(L1,2)-2*pow(height,2)*pow(L2,2)
    // 	  +pow(L1,4)-2*pow(L1,2)*pow(L2,2)+pow(L2,4)))+pow(b,4)-2*pow(b,3)*height+pow(b,2)*pow(L1,2)
    // 	-pow(b,2)*pow(L2,2)
    // 	+2*b*pow(height,3)-2*b*height*pow(L1,2)+2*b*height*pow(L2,2)-pow(height,4)+pow(height,2)*pow(L1,2)
    // 	-pow(height,2)*pow(L2,2)
    // 	)
    //   /(2*(b-height)*(pow(a,2)+pow(b,2)-2*b*height+pow(height,2)));

    py=(pow(a,2)*pow(b,2)-pow(a,2)*pow(height,2)-a*sqrt(-1*pow((b-height),2)*(pow(a,4)+2*pow(a,2)*pow(b,2)-4*pow(a,2)*b*height+2*pow(a,2)*pow(height,2)-2*pow(a,2)*pow(L1,2)-2*pow(a,2)*pow(L2,2)+pow(b,4)-4*pow(b,3)*height+6*pow(b,2)*pow(height,2)-2*pow(b,2)*pow(L1,2)-2*pow(b,2)*pow(L2,2)-4*b*pow(height,3)+4*b*height*pow(L1,2)+4*b*height*pow(L2,2)+pow(height,4)-2*pow(height,2)*pow(L1,2)-2*pow(height,2)*pow(L2,2)+pow(L1,4)-2*pow(L1,2)*pow(L2,2)+pow(L2,4)))+pow(b,4)-2*pow(b,3)*height+pow(b,2)*pow(L1,2)-pow(b,2)*pow(L2,2)+2*b*pow(height,3)-2*b*height*pow(L1,2)+2*b*height*pow(L2,2)-pow(height,4)+pow(height,2)*pow(L1,2)-pow(height,2)*pow(L2,2))/(2*(b-height)*(pow(a,2)+pow(b,2)-2*b*height+pow(height,2)));

    
    float r1,r2;
    r1=atan((height-py)/px)/3.1415926535/2*360; // in deg
    r2=atan((py-pos[i].b)/(px-pos[i].a))/3.1415926535/2*360; //in deg
    pos[i].px=px;
    pos[i].py=py;
    pos[i].t1=r1; 
    pos[i].t2=r1+r2;
   
  //    printf("x:%6.2f y:%6.2f px:%6.2f py:%6.2f r1:%6.2f r2:%6.2f\n",
  //    	   pos[i].a,pos[i].b,
  //    	   pos[i].px,pos[i].py,
  //    	   pos[i].t1,pos[i].t2);
  }  
}

void Leg::setPhase(int deg){
  phase=deg;
}

int Leg::currentPos(int clock){
  int p1 = ((phase/360.0)*steps);
  int r  = (clock +p1) % steps;

  return r;
}

float Leg::link1( int clock ){ 
  return pos[currentPos(clock)].t1;
}

float Leg::link2( int clock ){ 
  return pos[currentPos(clock)].t2;
}


void Leg::setAbsolute(int s1, int s2, int s3){
  sv[0].absolute(s1);
  sv[1].absolute(s2);
  sv[2].absolute(s3);
}

void Leg::position(float s1, float s2,float s3){
  sv[0].position(s1);
  sv[1].position(s2);
  sv[2].position(s3);
 
}

void Leg::updateClock(int tick){
  float t1,t2;
  if (transient>0){
    int tpos=TRANSIENTS-transient;
    sv[0].value=transientBuf[tpos][0].value;
    sv[1].value=transientBuf[tpos][1].value;
    sv[2].value=transientBuf[tpos][2].value;
    if (tpos >0){
      transient--;
    }
  }else{ // normal updte
    t1=pos[tick % steps].t1;
    t2=pos[tick % steps].t2;
    sv[0].position(0);
    sv[1].position(t1);
    sv[2].position(t2);
  }
}
