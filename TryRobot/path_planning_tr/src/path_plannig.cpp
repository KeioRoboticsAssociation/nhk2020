#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"

float odom[3] = {0};

void rogiCallback(const std_msgs::Float32MultiArray &msg)
{
    joy_vx = msg.data[0] * 0.8f;
    joy_vy = msg.data[1] * 0.8f;
    joy_omega = msg.data[2] * 0.2f;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "path_plannig_node");

    ros::NodeHandle n;

    ros::Publisher rogi_pub = n.advertise<std_msgs::Float32MultiArray>("direction", 10);
    //ros::Subscriber rogi_sub = n.subscribe("rogi_sub_topic", 100, rogiCallback);

    ros::NodeHandle arg_n("~");
    int looprate = 30;      // Hz
    arg_n.getParam("frequency", looprate);

    ros::Rate loop_rate(looprate);
    while (ros::ok())
    {
        std_msgs::Float32MultiArray floatarray;
        floatarray.data.resize(3);
        floatarray.data[0] = target_speed_1 * (float)looprate;
        floatarray.data[1] = theta_1;
        rogi_pub.publish(floatarray);

        ros::spinOnce();
        loop_rate.sleep();
    }
}