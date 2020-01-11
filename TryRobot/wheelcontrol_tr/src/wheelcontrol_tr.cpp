#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"

#include "sensor_msgs/Joy.h"

extern float theta_1, theta_2, theta_3, theta_4;
extern float target_speed_1, target_speed_2, target_speed_3, target_speed_4;
void wheel_control(float, float, float, float);

float theta = 0, joy_x = 0, joy_y = 0, omega = 0;

void msgCallback(const std_msgs::Float32MultiArray &msg)
{
    joy_x = msg.data[0];
    joy_y = msg.data[1];
    omega = +msg.data[2];
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "wheelcontrol_tr_node");

    ros::NodeHandle n;
    ros::Publisher pub_RF = n.advertise<std_msgs::Float32MultiArray>("control_RF", 10);
    ros::Publisher pub_LF = n.advertise<std_msgs::Float32MultiArray>("control_LF", 10);
    ros::Publisher pub_LB = n.advertise<std_msgs::Float32MultiArray>("control_LB", 10);
    ros::Publisher pub_RB = n.advertise<std_msgs::Float32MultiArray>("control_RB", 10);
    //ros::Publisher multiint_pub = n.advertise<std_msgs::Int32MultiArray>("Serial_sub_int", 10);

    ros::Subscriber joy_sub = n.subscribe("control_float", 100, msgCallback);

    ros::NodeHandle arg_n("~");
    int looprate = 30; // Hz
    arg_n.getParam("frequency", looprate);

    ros::Rate loop_rate(looprate);
    while (ros::ok())
    {
        wheel_control(theta, joy_x, joy_y, omega);


        std_msgs::Float32MultiArray floatarray;
        floatarray.data.resize(2);
        floatarray.data[0] = target_speed_1;
        floatarray.data[1] = theta_1;
        pub_RF.publish(floatarray);

        floatarray.data[0] = target_speed_2;
        floatarray.data[1] = theta_2;
        pub_LF.publish(floatarray);

        floatarray.data[0] = target_speed_3;
        floatarray.data[1] = theta_3;
        pub_LB.publish(floatarray);

        floatarray.data[0] = target_speed_4;
        floatarray.data[1] = theta_4;
        pub_RB.publish(floatarray);

        ros::spinOnce();
        loop_rate.sleep();
    }
}
