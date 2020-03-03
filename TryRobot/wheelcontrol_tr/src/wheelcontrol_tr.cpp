#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"

/********************* parameter ************************/
float stock_max = 10.0f;
float Kpid[3] = {0.1, 0.001, 0.008};
/********************************************************/

extern float theta_1, theta_2, theta_3, theta_4;
extern float target_speed_1, target_speed_2, target_speed_3, target_speed_4;
void wheel_control(float, float, float, float);

float bno_theta = 0;
float loop_vx = 0, loop_vy = 0, loop_omega = 0;
float loop_vx_old = 0, loop_vy_old = 0, loop_omega_old = 0;
float joy_vx = 0, joy_vy = 0, joy_omega = 0;
float path_vx = 0, path_vy = 0, path_omega = 0;

int flag = 0;
int mode = 0;

void calc_accel(float &now, float &old, float delta);
float theta_PID(float now, float ref, float sumpling_freq);

void joyCallback(const std_msgs::Float32MultiArray &msg)
{
    joy_vx = msg.data[0] * 1.0f;
    joy_vy = msg.data[1] * 1.0f;
    joy_omega = msg.data[2] * 1.5f;
}

void bnoCallback(const std_msgs::Float32MultiArray &msg)
{
    bno_theta = msg.data[0];
}

void armCallback(const std_msgs::Int32MultiArray &msg)
{
    flag = msg.data[0];
    mode = msg.data[1];
}

void pathCallback(const std_msgs::Float32MultiArray &msg)
{
    path_vx = msg.data[0];
    path_vy = msg.data[1];
    path_omega = msg.data[2];/////////////////
    //path_omega *= path_omega;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "wheelcontrol_tr_node");

    ros::NodeHandle n;
    ros::Publisher pub_RF = n.advertise<std_msgs::Float32MultiArray>("control_RF", 10);
    ros::Publisher pub_LF = n.advertise<std_msgs::Float32MultiArray>("control_LF", 10);
    ros::Publisher pub_LB = n.advertise<std_msgs::Float32MultiArray>("control_LB", 10);
    ros::Publisher pub_RB = n.advertise<std_msgs::Float32MultiArray>("control_RB", 10);

    ros::Subscriber joy_sub = n.subscribe("control_float", 100, joyCallback);
    ros::Subscriber bno_sub = n.subscribe("bno_float", 100, bnoCallback);
    ros::Subscriber flag_sub = n.subscribe("arm_mbed", 100, armCallback);
    ros::Subscriber path_sub = n.subscribe("path_control", 100, pathCallback);

    ros::NodeHandle arg_n("~");
    int looprate = 30;           // Hz
    float accel[2] = {0.2, 0.8}; // [vel, omega]
    arg_n.getParam("frequency", looprate);
    arg_n.getParam("accel_xy", accel[0]);
    arg_n.getParam("accel_theta", accel[1]);

    accel[0] /= (float)looprate;
    accel[1] /= (float)looprate;

    ros::Rate loop_rate(looprate);
    while (ros::ok())
    {
        loop_vx = (joy_vx + path_vx) / (float)looprate;
        loop_vy = (joy_vy + path_vy) / (float)looprate;
        loop_omega = (joy_omega + path_omega) / (float)looprate;
        if (flag == 1) //stop
        {
            loop_vx = 0;
            loop_vy = 0;
            loop_omega = 0;
        }

        // calc accel
        calc_accel(loop_vx, loop_vx_old, accel[0]);
        calc_accel(loop_vy, loop_vy_old, accel[0]);
        calc_accel(loop_omega, loop_omega_old, accel[1]);

        // calc wheel speed
        wheel_control(bno_theta, loop_vx, loop_vy, loop_omega);

        // publish wheel speed
        std_msgs::Float32MultiArray floatarray;
        floatarray.data.resize(2);
        floatarray.data[0] = target_speed_1 * (float)looprate;
        floatarray.data[1] = theta_1;
        pub_RF.publish(floatarray);

        floatarray.data[0] = target_speed_2 * (float)looprate;
        floatarray.data[1] = theta_2;
        pub_LF.publish(floatarray);

        floatarray.data[0] = target_speed_3 * (float)looprate;
        floatarray.data[1] = theta_3;
        pub_LB.publish(floatarray);

        floatarray.data[0] = target_speed_4 * (float)looprate;
        floatarray.data[1] = theta_4;
        pub_RB.publish(floatarray);

        ros::spinOnce();
        loop_rate.sleep();
    }
}

void calc_accel(float &now, float &old, float delta)
{
    if (now > old + delta)
        now = old + delta;
    else if (now < old - delta)
        now = old - delta;
    //if (abs(now) < 0.01)
    //    now = 0;
    old = now;
}

float theta_PID(float now, float ref, float sumpling_freq)
{
    static float stock_ = 0;
    static float olddiff = 0;
    float diff = ref - now;
    if(abs(diff) < 3.141592f/120.0f)
        diff = 0;
    stock_ += (diff + olddiff) / (sumpling_freq * 2);
    if (stock_ > stock_max)
        stock_ = stock_max;
    else if (stock_ < -stock_max)
        stock_ = -stock_max;
    if (flag == 1) //stop
        stock_ = 0;

    float ans = Kpid[0] * diff + Kpid[1] * stock_ + Kpid[2] * (diff - olddiff) * sumpling_freq;
    olddiff = diff;
    return ans;
}