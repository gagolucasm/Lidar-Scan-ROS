#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <stdlib.h>


void processScanCallback(const sensor_msgs::LaserScan::ConstPtr& msg);
ros::Publisher pub;

int main (int argc,char **argv)
{
    ros::init(argc, argv, "roomba2");
    ROS_INFO_STREAM("Me comere a tu perro");
    //CREACION DE COSAS DE ROS-> POS Y SUB
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/base_scan",1000,processScanCallback);
    pub=nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
    ros::Rate rate (20);


       ros::spin();
}


void processScanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
int n_ranges = msg->ranges.size();
std::vector<float>::const_iterator min_it =
        std::min_element(msg->ranges.begin(),msg->ranges.end());

double nearest = *min_it;

int pos = std::distance(msg->ranges.begin(), min_it);
ROS_INFO_STREAM(": mas cercano"<< nearest << "  pos: " << pos);



geometry_msgs::Twist msg1;

double linear=0, angular=0;


if (nearest < 0.65) {
  if (pos<165)
  {
      msg1.angular.z=-1;
      msg1.linear.x=0;
      ROS_INFO_STREAM("Puerta encontrada");


}
  else if (pos<200)
  {

      msg1.angular.z=0.2;
      msg1.linear.x=1;
      ROS_INFO_STREAM("Pared encontrada");

}
  else
  {

      msg1.angular.z=1;
      msg1.linear.x=0;
      ROS_INFO_STREAM("girando");

}
}
else {
    msg1.angular.z=-0.3;
    msg1.linear.x=1;
ROS_INFO_STREAM("Buscando Pared");
}



pub.publish(msg1);

ROS_INFO_STREAM("Sending velocity command: "
<< "linear= " << msg1.linear.x
<< "angular= " << msg1.angular.z);

