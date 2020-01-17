#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"

#include "sensor_msgs/Joy.h"

#define PI 3.141592f

float joystick_R[6] = {0};
int button[12] = {0};
float omega = 0.0;
int flag = 0; // 1:stop, 2:reset, 3:enable
int mode = 0; // 1:kick

void msgCallback(const sensor_msgs::Joy &msg)
{
    joystick_R[0] = -msg.axes[2];
    joystick_R[1] = msg.axes[3];
    for (int i = 0; i < 12; i++)
    {
        button[i] = msg.buttons[i];
    }
    if (button[4] > button[5])
        omega = 1.0;
    else if (button[4] < button[5])
        omega = -1.0;
    else
        omega = 0;

    // flag
    if (button[12] == 1) // stop
        flag = 1;
    else if (flag == 0)
    {
        if (button[11] == 1) // reset
            flag = 2;
        else if (button[10] == 1) // enable
            flag = 3;
    }

    // mode
    if (mode == 0)
    {
        if (button[9] == 1)
            mode = 1;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "joystick_tr_node");

    ros::NodeHandle n;
    ros::Publisher control_float_pub = n.advertise<std_msgs::Float32MultiArray>("control_float", 10);
    ros::Publisher flag_mbed_pub = n.advertise<std_msgs::Int32MultiArray>("flag_mbed", 10);
    ros::Publisher flag_int_pub = n.advertise<std_msgs::Int32>("flag_int", 10);

    ros::Subscriber joy_sub = n.subscribe("joy", 100, msgCallback);

    ros::NodeHandle arg_n("~");
    int looprate = 30; // Hz
    arg_n.getParam("loop", looprate);

    ros::Rate loop_rate(looprate);

    int flagcount = 0;
    int modecount = 0;

    while (ros::ok())
    {
        std_msgs::Float32MultiArray floatarray;
        floatarray.data.resize(3);
        floatarray.data[0] = joystick_R[0];
        floatarray.data[1] = joystick_R[1];
        floatarray.data[2] = omega;
        control_float_pub.publish(floatarray);

        // flag
        if (flag != 0)
        {
            if (flagcount < looprate / 2)
                flagcount++;
            else
            {
                flagcount = 0;
                flag = 0;
            }
            std_msgs::Int32 intflag;
            intflag.data = flag;
            flag_int_pub.publish(intflag);

            std_msgs::Int32MultiArray intarray;
            intarray.data.resize(2);
            intarray.data[0] = flag;
            intarray.data[1] = mode;
            flag_mbed_pub.publish(intarray);
        }
        
        // mode
        else if (mode != 0)
        {
            if (modecount < looprate / 2)
                modecount++;
            else
            {
                modecount = 0;
                mode = 0;
            }
            std_msgs::Int32MultiArray intarray;
            intarray.data.resize(2);
            intarray.data[0] = flag;
            intarray.data[1] = mode;
            flag_mbed_pub.publish(intarray);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
}
