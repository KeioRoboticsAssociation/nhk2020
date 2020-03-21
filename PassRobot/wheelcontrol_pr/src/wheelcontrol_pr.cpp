#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"
#include "sensor_msgs/Joy.h"

float wheel_control[2] = {0};

/*void msgCallback(const joystick_pr::control_float &msg) {
    wheel_control[0] = msg[0];
    wheel_control[1] = msg[1];
}*/

void msgCallback(const std_msgs::Joy &msg) {
    wheel_control[0] = msg.axes[0];
    wheel_control[1] = msg.axes[1];
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "wheelcontrol_pr_node");
    ros::NodeHandle n;
    ros::Publisher wheelcontrol_pub_plus = n.advertise<std_msgs::Float32MultiArray>("wheelcontrol_plus", 100);
    ros::Publisher wheelcontrol_pub_minus = n.advertise<std_msgs::Float32MultiArray>("wheelcontrol_minus", 100);

    ros::Subscriber wheelcontrol_sub = n.subscribe("control_float", 100, msgCallback);

    ros::NodeHandle arg_n("~");
    int looprate = 60;
    arg_n.getParam("loop", looprate);

    ros::Rate loop_rate(looprate);

    while(ros::ok()) {
        std_msgs::Float32MultiArray floatarray_plus, floatarray_minus;
        floatarray_plus.data.resize(1);
        floatarray_minus.data.resize(1);
        floatarray_plus[0] = wheel_control[0] + wheel_control[1];
        floatarray_minus[0] = wheel_control[1] - wheel_control[0];
        wheelcontrol_pub_plus.publish(floatarray_plus);
        wheelcontrol_pub_minus.publish(floatarray_minus);

        ros::spinOnce();
        loop_rate.sleep();
    }
}
