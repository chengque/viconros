#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <geometry_msgs/PoseStamped.h> 
#include <sstream>
#include <string.h>
#include "CFetchViconData.h"
int main(int argc, char **argv) {
	ros::init(argc, argv, "viconros");
	ros::NodeHandle n("~");
	std::string ip;
	std::string model;
	std::string segment;
	n.getParam("host",ip);
	n.getParam("model",model);
	n.getParam("segment",segment);
	ROS_INFO("HOST:%s",ip.c_str());
	ROS_INFO("MODEL:%s; SEGMENT:%s",model.c_str(),segment.c_str());
	ros::Publisher vicon_pub = n.advertise<geometry_msgs::PoseStamped> ("/mocap/pose", 10);
	ros::Rate loop_rate(100);
	int count = 0;
	CFetchViconData * vicon=new CFetchViconData();
	const char * host=ip.c_str();
	ObjStatus objs;
	
	if(!(vicon->IsConnected))
    { 
            ROS_INFO("Connecting to %s",host);
            bool res=vicon->Connect(host);
            if(res==false)
            {
                ROS_INFO("Failed to connect!\r\n");
                    return 0;
            }
            else
            {
                ROS_INFO("Successfully connected!\r\n");
            }

    }
	while (ros::ok()) {
		geometry_msgs::PoseStamped msg;
		objs=vicon->GetStatus(model.c_str(),segment.c_str());
		msg.header.stamp.sec=(int)objs.tm;
		msg.header.stamp.nsec=(objs.tm-msg.header.stamp.sec)*10000*10000;
		msg.pose.position.x =objs.pos[0];
		msg.pose.position.y =objs.pos[1];
		msg.pose.position.z =objs.pos[2];
		msg.pose.orientation.x =objs.ort[0];
		msg.pose.orientation.y =objs.ort[1];
		msg.pose.orientation.z =objs.ort[2];
		msg.pose.orientation.w =objs.ort[3];
		//ROS_INFO("position:%f-%f-%f; velocity: %f-%f-%f", msg.position.x,msg.position.y,msg.position.z,msg.velocity.x,msg.velocity.y,msg.velocity.z);
		vicon_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
		//++count;
	}
	return 0;
}