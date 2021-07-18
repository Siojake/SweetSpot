#include <ros/ros.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "std_msgs/String.h"
#include <sys/time.h>
#include <string.h>

int lstnfd,clifd;

typedef enum tagBASE64_TYPE {
    BASE64_TYPE_STANDARD,
    BASE64_TYPE_MIME,
    BASE64_TYPE_URL
} BASE64_TYPE;

static const char BASE64_TABLE[] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
};
static const char BASE64_TABLE_URL[] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"
};
static const int BASE64_TABLE_LENGTH = {
    sizeof(BASE64_TABLE) / sizeof(BASE64_TABLE[0]) - 1
};

typedef struct tagBASE64_SPEC {
    BASE64_TYPE type;
    const char *table;
    char pad;
    int maxLineLength;
    char *lineSep;
    int lineSepLength;
} BASE64_SPEC;

static const BASE64_SPEC BASE64_SPECS[] = {
    { BASE64_TYPE_STANDARD, BASE64_TABLE,     '=', 0,  NULL,   0 },
    { BASE64_TYPE_MIME,     BASE64_TABLE,     '=', 76, "\r\n", 2 },
    { BASE64_TYPE_URL,      BASE64_TABLE_URL, 0,   0,  NULL,   0 }
};

static const size_t BASE64_SPECS_LENGTH = {
    sizeof(BASE64_SPECS) / sizeof(BASE64_SPECS[0])
};

char *base64Encode(const char *data, const size_t size, const BASE64_TYPE type)
{
    BASE64_SPEC spec;
    size_t length;
    char *base64;
    char *cursor;
    int lineLength;
    int i;
    int j;

    if (data == NULL) {
        return NULL;
    }

    spec = BASE64_SPECS[0];
    for (i = 0; i < (int)BASE64_SPECS_LENGTH; i++) {
        if (BASE64_SPECS[i].type == type) {
            spec = BASE64_SPECS[i];
            break;
        }
    }

    length = size * 4 / 3 + 3 + 1;
    if (spec.maxLineLength > 0) {
        length += size / spec.maxLineLength * spec.lineSepLength;
    }
    base64 = (char *)malloc(length);
    if (base64 == NULL) {
        return NULL;
    }

    cursor = base64;
    lineLength = 0;
    for (i = 0, j = size; j > 0; i += 3, j -= 3) {
        if (spec.maxLineLength > 0) {
            if (lineLength >= spec.maxLineLength) {
                char *sep;

                for (sep = spec.lineSep; *sep != 0; sep++) {
                    *(cursor++) = *sep;
                }
                lineLength = 0;
            }
            lineLength += 4;
        }

        if (j == 1) {
            *(cursor++) = spec.table[(data[i + 0] >> 2 & 0x3f)];
            *(cursor++) = spec.table[(data[i + 0] << 4 & 0x30)];
            *(cursor++) = spec.pad;
            *(cursor++) = spec.pad;
        }
        else if (j == 2) {
            *(cursor++) = spec.table[(data[i + 0] >> 2 & 0x3f)];
            *(cursor++) = spec.table[(data[i + 0] << 4 & 0x30) | (data[i + 1] >> 4 & 0x0f)];
            *(cursor++) = spec.table[(data[i + 1] << 2 & 0x3c)];
            *(cursor++) = spec.pad;
        }
        else {
            *(cursor++) = spec.table[(data[i + 0] >> 2 & 0x3f)];
            *(cursor++) = spec.table[(data[i + 0] << 4 & 0x30) | (data[i + 1] >> 4 & 0x0f)];
            *(cursor++) = spec.table[(data[i + 1] << 2 & 0x3c) | (data[i + 2] >> 6 & 0x03)];
            *(cursor++) = spec.table[(data[i + 2] << 0 & 0x3f)];
        }
    }
    *cursor = 0;

    return base64;
}

unsigned int frames=0;

void imageCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
    //    printf("recv\n");
    cv::Mat image = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat

    //    printf("%d \n",msg->data.size());
    if (clifd){
      // cv::imshow("view", image);
      
      char buf[8200];
      memset(&buf,0,8200);
      struct timeval tv;
      gettimeofday(&tv,NULL);
      char * base64;
      base64 =base64Encode((const char *)&(msg->data[0]),msg->data.size(),BASE64_TYPE_STANDARD);
      //      printf("%s\n",base64);

      sprintf(buf,"jpeg:%8d: %8d:%8d",(frames++)&0xffff,(unsigned int)((tv.tv_sec%100000)*1000+tv.tv_usec/1000),strlen(base64));
      //      printf("%s\n",buf);

      
      int r = send( clifd, (void *)buf,strlen(buf),0);
      //r = send( clifd ,(void *)&(msg->data),msg->data.size(),0);
      r=send(clifd, (void*) base64 ,strlen(base64),0);
      //  sprintf(buf,"\n");
      //      r=send(clifd,(void*) buf,1,0);
      // printf("%d:%d\n",(int)msg->data.size(),r);
      if (r>0){
	//i+=r;
      }else{
	
	if (r==0){
	  //socket closed
	}else{
	  perror("send");
	}
	
      }
    
      // int i;
      // char * p = (char *)&(msg->data);
      
      // for (i=0; msg->data.size()-i > 8192 ; ){
      // 	memcpy(buf,&p[i],8192);
      // 	int r=send(clifd,(void*) buf, 8192,0);
      // 	if (r>0){
      // 	  i+=r;
      // 	}else{
	  
      // 	  if (r==0){
      // 	    //socket closed
      // 	  }else{
      // 	    perror("send");
      // 	  }

      // 	}
	  
      // }
      // // last block
      // memset(buf,0,8192);
      // memcpy(buf,&p[i],msg->data.size()-i);
      // int r= send(clifd,(void*)buf ,8192,0);
      // if (r>0){
      // 	i+=r;
      // }else{
      // 	if (r==0){
      // 	  //socket closed
      // 	}else{ 
      // 	  perror("send");
      // 	}// else if (r==0)
      // }// else if (r>0)

      
      
    } // while ros::ok
    
    cv::waitKey(1);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert to image!");
  }
}

int main(int argc, char **argv)
{

  // make ready ros
  ros::init(argc, argv, "commander_relay");
  ros::NodeHandle nh;
  //cv::namedWindow("view");
  cv::startWindowThread();
  ros::Subscriber sub = nh.subscribe("/raspicam_node/image/compressed", 1, imageCallback);
  ros::Publisher pub = nh.advertise <std_msgs::String> ("cmd",1000);
#define PORT 11320
#define MAXCONN 2
  // make ready socket
  socklen_t len = sizeof(struct sockaddr_in);
  struct sockaddr_in from_addr,addr;
  
  lstnfd=socket(AF_INET,SOCK_STREAM,0);

  addr.sin_family = AF_INET;
  addr.sin_port=ntohs(PORT);
  addr.sin_addr.s_addr=INADDR_ANY;

  if (bind(lstnfd,(struct sockaddr *) & addr,sizeof(addr))<0){
    perror("socket bind");
  }
  printf("wait for client.connect at %d :%d\n",PORT,lstnfd);  
  fd_set RFDS;
  
  listen(lstnfd,5);
  
  FD_ZERO(&RFDS);
  
  FD_SET(lstnfd,&RFDS);
  
  while (ros::ok()){
    fd_set rfds;

    memcpy(&rfds,&RFDS,sizeof(fd_set));
    struct timeval tv;
    tv.tv_sec=0;
    tv.tv_usec=1;
    
    int r= select(FD_SETSIZE,&rfds,NULL,NULL,&tv);
    if (r !=0) printf("select break:%d\n",r);
    if (r <0){
      perror("select");
      printf("%x",rfds);
      exit(1);
    }
    if (r>0){
      if (FD_ISSET(lstnfd,&rfds)){
	printf("accept\n");
	if ( clifd == 0 ){
	  struct sockaddr_in toward;
	  socklen_t tl ;
	  int tmpfd=accept(lstnfd,(struct sockaddr*)&toward,&tl );
	  if (tmpfd >0){
	    clifd = tmpfd;
	    //	  printf("accepted connection from %s, port=%d\n",
	    //		 inet_ntoa(toward.sin_addr), ntohs(toward.sin_port),clifd);
	    printf("clifd:%d\n",clifd);
	    FD_SET(clifd,&RFDS);
	  }
	}
      }
      if (FD_ISSET(clifd,&rfds)){
	printf("clifd\n");
	char buf[1024];
	int r = recv(clifd,&buf,1024,0);
	if (r==0){// socket closed
	  FD_CLR(clifd,&RFDS);
	  close(clifd);
	  clifd=0;
	}
	//printf("recv:%s\n",buf);
	#define LAST 11
	
        char *cmds[LAST]={"stp","sit","std","fwd",
			  "sto","bck","lft","ctr",
			  "rgt","lay","end"};

	
	std_msgs::String msg;
	
	for (int i=0;i< LAST ;i++){
	  if (strncmp(buf,cmds[i],3) == 0){
	      msg.data=cmds[i];
	      pub.publish(msg);
	  }
	}

	  
	// if (strncmp(buf,"end",3)==0){
	//   FD_CLR(clifd,&RFDS);
	//   close(clifd);
	//   clifd=0;
	// }
	// if (strncmp(buf,"stp",3)==0){
	//   // command stop
	//   printf("stop\n");
	// }
	// if (strncmp(buf,"sit",3)==0){
	//   // command sit
	//   printf("sit\n");
	// }
	// if (strncmp(buf,"std",3)==0){
	//   // command stand
	//   printf("stand\n");
	// }
	// if (strncmp(buf,"lay",3)==0){
	//   // command laydown
	//   printf("laydown\n");
	// }
	// if (strncmp(buf,"fwd",3)==0){
	//   // command forward
	//   printf("forword\n");
	// }
	// if (strncmp(buf,"sto",3)==0){
	//   // command stop
	//   printf("stop\n");
	// }
	// if (strncmp(buf,"bck",3)==0){
	//   // command backword
	//   printf("backword\n");
	// }
	// if (strncmp(buf,"lft",3)==0){
	//   // command left
	//   printf("left\n");
	// }
	// if (strncmp(buf,"ctr",3)==0){
	//   // command cener
	//   printf("center\n");
	// }
	// if (strncmp(buf,"rgt",3)==0){
	//   // command righr
	//   printf("right\n");
	// }
      }
    }
    
    ros::spinOnce();
  }
  cv::destroyWindow("view");
}