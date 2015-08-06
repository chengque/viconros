#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "viconros/viconmocap.h"
#include <sstream>
#include "CFetchViconData.h"
int main(int argc, char **argv) {
	ros::init(argc, argv, "viconros");
	ros::NodeHandle n;
	ros::Publisher vicon_pub = n.advertise<viconros::viconmocap>("vicon", 1000);
	ros::Rate loop_rate(50);
	int count = 0;
	CFetchViconData * vicon=new CFetchViconData();

	memcpy(vicon->model[0],"quadmodel",sizeof("quadmodel"));
	memcpy(vicon->segment[0],"whole",sizeof("whole"));
	ROS_INFO("Model:%s;Segment:%s",vicon->model[0],vicon->segment[0]);
	const char * host="192.168.2.100:801";
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
		viconros::viconmocap msg;
		objs=vicon->GetStatus(0);
		msg.position.x =objs.pos[0];
		msg.position.y =objs.pos[1];
		msg.position.z =objs.pos[2];
		msg.velocity.x =objs.vel[0];
		msg.velocity.y =objs.vel[1];
		msg.velocity.z =objs.vel[2];
		ROS_INFO("position:%f-%f-%f; velocity: %f-%f-%f", msg.position.x,msg.position.y,msg.position.z,msg.velocity.x,msg.velocity.y,msg.velocity.z);
		vicon_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
		++count;
	}
	return 0;
}