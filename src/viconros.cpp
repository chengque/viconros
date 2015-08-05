#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <sstream>
#include "CFetchViconData.h"
int main(int argc, char **argv) {
	ros::init(argc, argv, "viconros");
	ros::NodeHandle n;
	ros::Publisher vicon_pub = n.advertise<std_msgs::Float32>("vicon", 1000);
	ros::Rate loop_rate(1);
	int count = 0;
	CFetchViconData * vicon=new CFetchViconData();
	const char * host="192.168.2.102:801";
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
		std_msgs::Float32 msg;
		objs=vicon->GetStatus(0);
		msg.data =objs.pos[0];
		ROS_INFO("%f", msg.data);
		vicon_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
		++count;
	}
	return 0;
}