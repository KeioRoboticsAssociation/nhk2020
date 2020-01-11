#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"

#include "sensor_msgs/Joy.h"

extern float vx, vy, w, d, theta_1, theta_2, theta_3, theta_4, theta;
extern float vx_1, vy_1, vx_2, vy_2, vx_3, vy_3, vx_4, vy_4;
extern float v_1, v_2, v_3, v_4;
void wheel_control(float, float, float, float);

float theta = 0, joy_x = 0, joy_y = 0, omega = 0;

void msgCallback(const sensor_msgs::Joy &msg)
{
    
    ;
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

    ros::NodeHandle arg_n("~");
    int looprate = 30; // Hz
    arg_n.getParam("frequency", looprate);

    ros::Rate loop_rate(looprate);
    while (ros::ok())
    {
        std_msgs::Float32MultiArray floatarray;
        floatarray.data.resize(2);
        floatarray.data[0] = 100;
        floatarray.data[1] = 100;
        multifloat_pub.publish(floatarray);

        wheel_control(theta, joy_x, joy_y, omega);

        ros::spinOnce();
        loop_rate.sleep();
    }
}
