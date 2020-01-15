#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"

#include "sensor_msgs/Joy.h"

extern float theta_1, theta_2, theta_3, theta_4;
extern float target_speed_1, target_speed_2, target_speed_3, target_speed_4;
void wheel_control(float, float, float, float);

float bno_theta = 0, joy_x = 0, joy_y = 0, omega = 0;
float joy_x_t = 0, joy_y_t = 0, omega_t = 0;
float joy_xy = 0, old_joy_xy = 0, old_omega = 0;

void msgCallback(const std_msgs::Float32MultiArray &msg)
{
    joy_x_t = msg.data[0];
    joy_y_t = msg.data[1];
    omega_t = msg.data[2];
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
    int looprate = 30;           // Hz
    float accel[2] = {1.0, 1.0}; // [vel, omega]
    arg_n.getParam("frequency", looprate);
    arg_n.getParam("accel_xy", accel[0]);
    arg_n.getParam("accel_theta", accel[1]);

    accel[0] /= (float)looprate;
    accel[1] /= (float)looprate;

    ros::Rate loop_rate(looprate);
    while (ros::ok())
    {
        joy_x = joy_x_t / (float)looprate;
        joy_y = joy_y_t / (float)looprate;
        omega = omega_t / (float)looprate;
        if (omega > old_omega)
        {
            if (omega > old_omega + accel[1])
                omega = old_omega + accel[1];
        }
        else
        {
            if (omega < old_omega - accel[1])
                omega = old_omega - accel[1];
        }
        old_omega = omega;

        joy_xy = sqrt(joy_x * joy_x + joy_y * joy_y);
        if (joy_xy < 1e-8)
        {
            joy_x = 0;
            joy_y = 0;
        }
        else
        {
            float joy_xy_temp = joy_xy;
            if (joy_xy > old_joy_xy)
            {
                if (joy_xy > old_joy_xy + accel[0])
                    joy_xy = old_joy_xy + accel[0];
            }
            else
            {
                if (joy_xy < old_joy_xy - accel[0])
                    joy_xy = old_joy_xy - accel[0];
            }
            joy_x *= joy_xy / joy_xy_temp;
            joy_y *= joy_xy / joy_xy_temp;
        }
        old_joy_xy = joy_xy;

        wheel_control(bno_theta, joy_x, joy_y, omega);

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
