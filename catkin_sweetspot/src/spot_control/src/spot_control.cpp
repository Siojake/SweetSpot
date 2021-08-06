
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <string>
#include <string.h>

#include "spotmicro.h"


// variables definition
struct servo{
  short no;
  short value;
};

servo servos[16];

std::vector< std::string > cmds =
  { "stp","sit","std","fwd","sto",
    "bck","lft","ctr","rgt","liey","end"};

ros::Publisher  sc; // spot control
ros::Subscriber ss; // spot status
ros::Subscriber cm; // command
ros::Publisher  lc; // spot lcd string
ros::Subscriber bv; // battery volt
Body *spot;

int command_number(std::string cmd)
{
  int ret=-1;
  for (int i=0;i< cmds.size(); i++){
    if ( cmds[i] == cmd ){
      ret=i;
    }
  }

  return ret;
}

// cmd message callback 
void cmdCallback(const std_msgs::String::ConstPtr & msg)
{
  int cmdno=command_number(msg->data);
  ROS_INFO("cmd Recv:[%d:%s]",cmdno,msg->data.c_str());
  printf("cmd Recv:[%d:%s]",cmdno,msg->data.c_str());
  spot->command(cmdno);
  i2cpwm_board::ServoArray *pSrv;
  pSrv=spot->getAbsolute();
  sc.publish(*pSrv);
}

// cmd message callback 
void SpotStatCallback(const std_msgs::String::ConstPtr & msg)
{

  ROS_INFO("battery Recv:[%f]",stof(msg->data));
  //body.setBattery(msg->data);
  //build lcd message
  //body.buildLcdMessage(String * l1, String * l2);
  
}




int main(int argc,char ** argv)
{
  int step;
  ros::init(argc,argv,"spot_control_node");
  ros::NodeHandle nh1,nh2;
  //ros::Publisher
  sc= nh1.advertise < i2cpwm_board::ServoArray >("/spot/servo_absolute", 1000);
  lc= nh1.advertise < std_msgs::String >("/spot/lcd",100);
  ss= nh1.subscribe("/spot/cmd",100,cmdCallback);
  bv= nh1.subscribe("/spot/SpotStatus",100,SpotStatCallback);
  ros::Rate loop_ratte(24);
  memset(&servos, 0, sizeof(servos));
    
  spot = new Body();
  //  spot->init(24);
  step=24;
  spot->init(step);
  while( ros::ok() ) {
    //i2cpwm_board::ServoAbsolute absrv);
    // absrv=body->updateClock();
    //sc.publish(absrv);
    //if (lcdupdate.size()>0){
    //lcd
    ros::spinOnce();
  }
  //body->command();
}
