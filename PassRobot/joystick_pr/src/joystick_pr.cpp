#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"
#include "sensor_msgs/Joy.h"

#define JOY_X 2
#define JOY_Y 3

float joystick_R[2] = {0};
int LED = 0;

void msgCallback(const sensor_msgs::Joy &msg) {
    joystick_R[0] = -msg.axes[JOY_X];
    joystick_R[1] = msg.axes[JOY_Y];
    LED = msg.buttons[0];
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "joystick_pr_node");

    ros::NodeHandle n;
    ros::Publisher control_float_pub = n.advertise<std_msgs::Float32MultiArray>("control_float", 10);
    ros::Publisher control_LED_pub = n.advertise<std_msgs::Int32MultiArray>("control_int", 10);
    ros::Subscriber joy_sub = n.subscribe("joy", 100, msgCallback);

    ros::NodeHandle arg_n("~");
    int looprate = 60;
    arg_n.getParam("loop", looprate);

    ros::Rate loop_rate(looprate);

    while(ros::ok()) {
        std_msgs::Float32MultiArray floatarray;
        std_msgs::Int32MultiArray intarray;
        intarray.data.resize(1);
        intarray.data[0] = LED;
        floatarray.data.resize(2);
        floatarray.data[0] = joystick_R[0];
        floatarray.data[1] = joystick_R[1];
        control_float_pub.publish(floatarray);
        control_LED_pub.publish(intarray);

        ros::spinOnce();
        loop_rate.sleep();
    }
}
