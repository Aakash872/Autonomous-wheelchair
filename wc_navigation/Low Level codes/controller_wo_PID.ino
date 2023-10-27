#include <math.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#define PWM_MIN 20
#define PWMRANGE 255
#define L 0.5
#define R 0.1615
bool _connected = false;
bool rosConnected();
void onTwist(const geometry_msgs::Twist &msg);
float mapPwm(float x, float out_min, float out_max);
#include <Cytron_SmartDriveDuo.h>
#define IN1 4 // Arduino pin 4 is connected to MDDS30 pin IN1.
#define AN1 5 // Arduino pin 5 is connected to MDDS30 pin AN1.
#define AN2 6 // Arduino pin 6 is connected to MDDS30 pin AN2.
#define IN2 7 // Arduino pin 7 is connected to MDDS30 pin IN2.
Cytron_SmartDriveDuo smartDriveDuo30(PWM_INDEPENDENT, IN1, IN2, AN1, AN2);
ros::NodeHandle node;
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &onTwist);
std_msgs::String str_msg;
ros::Publisher chatter("debug", &str_msg);
int lp=0,rp=0;

//Consider maximum velocity=0.1 at which corresponding pwm value is noted. This is used in the low level controller



void setup()
{
  Serial.begin(9600);
  node.initNode();
  node.subscribe(sub);
  node.advertise(chatter);
}

void onTwist(const geometry_msgs::Twist &msg)
{
  if (!_connected)
  {
     Serial.println("Stopped");
    return;
  }
  float l = (1/R)*(msg.linear.x-msg.angular.z*L/2);
  float r = (1/R)*(msg.linear.x+msg.angular.z*L/2);
  float lPwm,rPwm;
  if(l==0&&r==0)
  {
    lPwm=0;
    rPwm=0;
  }
  if( l > 0){
    lPwm = 10 + (10.84599 * l);
    }
  else{
    lPwm = -10 + (10.84599* l);
    }

   if( r > 0){
    rPwm = 10 + (10.84599 * r);
    }
  else{
    rPwm = -10 + (10.84599 * r);
    }
    
  char temp[30];
  String t="lpWM : " + String(lPwm) + " " + "rpWM : " + String(rPwm)+"l: "+String(l)+" r: "+String(r);
  t.toCharArray(temp,40);
  str_msg.data=temp;
  chatter.publish(&str_msg);
  smartDriveDuo30.control(-lPwm, rPwm);
  delay(1);
}

void loop()
{
  if (!rosConnected())
  {
     Serial.println("Stopped");
  }
  node.spinOnce();
}

bool rosConnected()
{
  // If value changes, notify via LED and console.
  bool connected = node.connected();
  if (_connected != connected)
  {
    _connected = connected;
    digitalWrite(LED_BUILTIN, !connected); // false -> on, true -> off
    Serial.println(connected ? "ROS connected" : "ROS disconnected");
  }
  return connected;
}