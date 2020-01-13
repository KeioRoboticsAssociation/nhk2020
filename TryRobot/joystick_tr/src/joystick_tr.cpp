#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"

#include "sensor_msgs/Joy.h"

#define PI 3.141592f

float joystick_R[6] = {0};
int button[12] = {0};
float omega = 0.0;

void msgCallback(const sensor_msgs::Joy &msg)
{
    joystick_R[0] = -msg.axes[2];
    joystick_R[1] = msg.axes[3];
    for (int i = 0; i < 12; i++)
    {
        button[i] = msg.buttons[i];
    }
    if (button[4] == 1 && button[5] == 0)
        omega = 1.0;
    else if (button[4] == 0 && button[5] == 1)
        omega = -1.0;
    else
        omega = 0;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "joystick_tr_node");

    ros::NodeHandle n;
    ros::Publisher multifloat_pub = n.advertise<std_msgs::Float32MultiArray>("control_float", 10);
    ros::Publisher multiint_pub = n.advertise<std_msgs::Int32MultiArray>("control_int", 10);

    ros::Subscriber joy_sub = n.subscribe("joy", 100, msgCallback);

    ros::NodeHandle arg_n("~");
    int looprate = 30; // Hz
    arg_n.getParam("loop", looprate);

    ros::Rate loop_rate(looprate);
    while (ros::ok())
    {
        std_msgs::Float32MultiArray floatarray;
        floatarray.data.resize(3);
        floatarray.data[0] = joystick_R[0] / (float)(looprate);
        floatarray.data[1] = joystick_R[1] / (float)(looprate);
        floatarray.data[2] = omega / (float)(looprate);
        multifloat_pub.publish(floatarray);

        std_msgs::Int32MultiArray intarray;
        intarray.data.resize(12);
        for (int i = 0; i < 12; i++)
        {
            intarray.data[i] = button[i];
        }
        multiint_pub.publish(intarray);

        ros::spinOnce();
        loop_rate.sleep();
    }
}
