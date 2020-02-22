#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

#define PI 3.141592F
#define BODY_WIDTH 440.0F // [mm]

int flag = 0;
int mode = 0;
float bno_theta = 0;
float position[2] = {0, 0}; // [mm]
float initialpos[2] = {-6150, 500};
float wheelpos[4][2] = {0};
float looprate_bno = 30.0f;    //[Hz]//////////////////

void set_body_position(float x_, float y_, float theta_);

void bnoCallback(const std_msgs::Float32MultiArray &msg)
{
    bno_theta = msg.data[0];
}

void flagCallback(const std_msgs::Int32MultiArray &msg)
{
    flag = msg.data[0];
    mode = msg.data[1];
}

void RF_Callback(const std_msgs::Float32MultiArray &msg)
{
    static ros::Time last_time = ros::Time::now();
    ros::Time current_time = ros::Time::now();
    float delta = msg.data[0] * 1000.0f * (current_time - last_time).toSec();   // [mm]
    //float delta = msg.data[0] * 1000.0f / looprate_bno; // [mm/step]
    wheelpos[0][0] -= delta * sin(bno_theta + msg.data[1]);
    wheelpos[0][1] += delta * cos(bno_theta + msg.data[1]);
    last_time = current_time;
}

void LF_Callback(const std_msgs::Float32MultiArray &msg)
{
    static ros::Time last_time = ros::Time::now();
    ros::Time current_time = ros::Time::now();
    float delta = msg.data[0] * 1000.0f * (current_time - last_time).toSec();   // [mm]
    wheelpos[1][0] -= delta * sin(bno_theta + msg.data[1]);
    wheelpos[1][1] += delta * cos(bno_theta + msg.data[1]);
    last_time = current_time;
}

void LB_Callback(const std_msgs::Float32MultiArray &msg)
{
    static ros::Time last_time = ros::Time::now();
    ros::Time current_time = ros::Time::now();
    float delta = msg.data[0] * 1000.0f * (current_time - last_time).toSec();   // [mm]
    wheelpos[2][0] -= delta * sin(bno_theta + msg.data[1]);
    wheelpos[2][1] += delta * cos(bno_theta + msg.data[1]);
    last_time = current_time;
}

void RB_Callback(const std_msgs::Float32MultiArray &msg)
{
    static ros::Time last_time = ros::Time::now();
    ros::Time current_time = ros::Time::now();
    float delta = msg.data[0] * 1000.0f * (current_time - last_time).toSec();   // [mm]
    wheelpos[3][0] -= delta * sin(bno_theta + msg.data[1]);
    wheelpos[3][1] += delta * cos(bno_theta + msg.data[1]);
    last_time = current_time;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "localization_4wds_tr_node");

    ros::NodeHandle n;

    ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
    tf::TransformBroadcaster odom_broadcaster;
    // ros::Publisher pub_RF = n.advertise<std_msgs::Float32MultiArray>("control_RF", 10);
    ros::Subscriber bno_sub = n.subscribe("bno_float", 100, bnoCallback);
    ros::Subscriber flag_sub = n.subscribe("arm_mbed", 100, flagCallback);

    ros::Subscriber sub_RF = n.subscribe("data_RF", 100, RF_Callback);
    ros::Subscriber sub_LF = n.subscribe("data_LF", 100, LF_Callback);
    ros::Subscriber sub_LB = n.subscribe("data_LB", 100, LB_Callback);
    ros::Subscriber sub_RB = n.subscribe("data_RB", 100, RB_Callback);

    ros::NodeHandle arg_n("~");
    int looprate = 30;      // Hz
    arg_n.getParam("frequency", looprate);
    arg_n.getParam("frequency_bno", looprate_bno);/////////

    float vx, vy, vtheta;   // [m/s, rad/s]
    float oldpos[3] = {initialpos[0], initialpos[1], 0}; // [mm]

    ros::Rate loop_rate(looprate);
    while (ros::ok())
    {
        // time
        ros::Time current_time = ros::Time::now();

        if (flag == 2)
        {
            set_body_position(initialpos[0], initialpos[1], bno_theta);
        }

        set_body_position((wheelpos[0][0] + wheelpos[1][0] + wheelpos[2][0] + wheelpos[3][0]) / 4,
                          (wheelpos[0][1] + wheelpos[1][1] + wheelpos[2][1] + wheelpos[3][1]) / 4,
                          bno_theta);

        // calc yaw->quaternion
        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(bno_theta);

        // tf trans odom->base_link
        geometry_msgs::TransformStamped odom_trans;
        odom_trans.header.stamp = current_time;
        odom_trans.header.frame_id = "odom";
        odom_trans.child_frame_id = "base_link";
        // set position
        odom_trans.transform.translation.x = position[0];
        odom_trans.transform.translation.y = position[1];
        odom_trans.transform.translation.z = 0.0;
        odom_trans.transform.rotation = odom_quat;
        // send transform
        odom_broadcaster.sendTransform(odom_trans);

        // nav_msgs
        nav_msgs::Odometry odom;
        odom.header.stamp = current_time;
        odom.header.frame_id = "odom";
        // set position
        odom.pose.pose.position.x = position[0];
        odom.pose.pose.position.y = position[1];
        odom.pose.pose.position.z = 0.0;
        odom.pose.pose.orientation = odom_quat;
        // calc velocity
        vx = (position[0] - oldpos[0]) * (float)looprate / 1000.0f;
        vy = (position[1] - oldpos[1]) * (float)looprate / 1000.0f;
        vtheta = (bno_theta - oldpos[2]) * (float)looprate;
        oldpos[0] = position[0];
        oldpos[1] = position[1];
        oldpos[2] = bno_theta;
        // set velocity
        odom.child_frame_id = "base_link";
        odom.twist.twist.linear.x = vx;
        odom.twist.twist.linear.y = vy;
        odom.twist.twist.angular.z = vtheta;
        // publish odom
        odom_pub.publish(odom);

        ros::spinOnce();
        loop_rate.sleep();
    }
}

void set_body_position(float x_, float y_, float theta_){
    position[0] = x_;
    position[1] = y_;
    for (int i = 0; i < 4; i++)
    {
        wheelpos[i][0] = x_ + BODY_WIDTH / sqrt(2) * cos(theta_ + PI * (2 * i + 1) / 4);
        wheelpos[i][1] = y_ + BODY_WIDTH / sqrt(2) * sin(theta_ + PI * (2 * i + 1) / 4);
    }
}