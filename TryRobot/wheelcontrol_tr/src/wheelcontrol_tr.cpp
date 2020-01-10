#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"

#include "sensor_msgs/Joy.h"

void msgCallback(const sensor_msgs::Joy &msg)
{
    ;
}

std_msgs::Float32MultiArray pub_multi_float(float *, int);
std_msgs::Int32MultiArray pub_multi_int(int *, int);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "wheelcontrol_tr_node");

    ros::NodeHandle n;
    ros::Publisher multifloat_pub = n.advertise<std_msgs::Float32MultiArray>("Serial_sub_float", 10);
    ros::Publisher multiint_pub = n.advertise<std_msgs::Int32MultiArray>("Serial_sub_int", 10);

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

        std_msgs::Int32MultiArray intarray;
        intarray.data.resize(4);
        for (int i = 0; i < 4; i++)
        {
            intarray.data[i] = i;
        }
        multiint_pub.publish(intarray);

        ros::spinOnce();
        loop_rate.sleep();
    }
}
