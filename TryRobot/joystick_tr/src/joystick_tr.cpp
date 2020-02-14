#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"

#include "sensor_msgs/Joy.h"

/***************** joystick number ********************/
#define JOY_X 2
#define JOY_Y 3
#define JOY_OMEGA_R 5
#define JOY_OMEGA_L 4
#define TRY_MOTOR_R 7
#define TRY_MOTOR_L 6
#define FLAG_1 11
#define FLAG_2 10
#define FLAG_3 9
#define MODE_1 0
#define MODE_2 1
#define MODE_3 2
/******************************************************/

#define PI 3.141592f

float joystick_R[6] = {0};
float omega = 0.0;
int flag = 0;      // 0:not publish, 1:stop, 2:reset, 3:enable
                   // (1->button->3, 2->while->0, 3->while->0)
int mode = 0;      // 0:others, 1:catch, 2:try, 3:kick
                   // send arm mbed (mbedreply->0)
int try_motor = 0; // 0:none, 1:push, -1:pull

void msgCallback(const sensor_msgs::Joy &msg)
{
    joystick_R[0] = -msg.axes[JOY_X];
    joystick_R[1] = msg.axes[JOY_Y];

    if (msg.buttons[JOY_OMEGA_L] > msg.buttons[JOY_OMEGA_R])    // rotation
        omega = 1.0;
    else if (msg.buttons[JOY_OMEGA_L] < msg.buttons[JOY_OMEGA_R])
        omega = -1.0;
    else
        omega = 0;

    // try_motor
    if (msg.buttons[TRY_MOTOR_L] > msg.buttons[TRY_MOTOR_R])
        try_motor = 1;
    else if (msg.buttons[TRY_MOTOR_L] < msg.buttons[TRY_MOTOR_R])
        try_motor = -1;
    else
        try_motor = 0;

    // flag
    if (msg.buttons[FLAG_1] == 1) // stop
        flag = 1;
    if (flag == 1 && msg.buttons[FLAG_3] == 1) // enable
        flag = 3;
    if (flag == 0)
    {
        if (msg.buttons[FLAG_2] == 1) // reset
            flag = 2;
    }

    // mode
    if (mode == 0) // others
    {
        if (msg.buttons[MODE_1] == 1) // catch
            mode = 1;
        else if (msg.buttons[MODE_2] == 1) // try
            mode = 2;
        else if (msg.buttons[MODE_3] == 1) // kick
            mode = 3;
    }
}

void armCallback(const std_msgs::Int32MultiArray &msg)
{
    // flag_reply
    // 0:not yet, 1:finished
    static bool mode0flag = true;
    if (mode0flag)
    {
        if (msg.data[0] == 1)
        {
            mode = 0;
            mode0flag = false;
        }
    }
    else
    {
        if (msg.data[0] == 0)
            mode0flag = true;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "joystick_tr_node");

    ros::NodeHandle n;
    ros::Publisher control_float_pub = n.advertise<std_msgs::Float32MultiArray>("control_float", 10); // joystick
    ros::Publisher leg_mbed_pub = n.advertise<std_msgs::Int32MultiArray>("leg_mbed", 10);             // leg mbed(flag)
    ros::Publisher arm_mbed_pub = n.advertise<std_msgs::Int32MultiArray>("arm_mbed", 10);             // leg mbed(flag, mode)
    //ros::Publisher flag_int_pub = n.advertise<std_msgs::Int32>("flag_int", 10);

    ros::Subscriber joy_sub = n.subscribe("joy", 100, msgCallback);
    ros::Subscriber arm_sub = n.subscribe("arm_mbed_reply", 100, armCallback);

    ros::NodeHandle arg_n("~");
    int looprate = 30; // Hz
    arg_n.getParam("loop", looprate);

    ros::Rate loop_rate(looprate);

    int flagcount = 0;

    while (ros::ok())
    {
        // joystick wheel
        std_msgs::Float32MultiArray floatarray;
        floatarray.data.resize(3);
        floatarray.data[0] = joystick_R[0];
        floatarray.data[1] = joystick_R[1];
        floatarray.data[2] = omega;
        control_float_pub.publish(floatarray);

        // flag
        if (flag != 0)
        {
            std_msgs::Int32MultiArray intarray_leg;
            intarray_leg.data.resize(1);
            intarray_leg.data[0] = flag;
            leg_mbed_pub.publish(intarray_leg);

            if (flag == 2 || flag == 3)
            {
                if (flagcount < looprate / 2)   // publishing flag
                    flagcount++;
                else
                {
                    flagcount = 0;  // stop publish
                    flag = 0;
                }
            }
        }

        // mode
        std_msgs::Int32MultiArray intarray_arm;
        intarray_arm.data.resize(3);
        intarray_arm.data[0] = flag;
        intarray_arm.data[1] = mode;
        intarray_arm.data[2] = try_motor;
        arm_mbed_pub.publish(intarray_arm);

        ros::spinOnce();
        loop_rate.sleep();
    }
}
