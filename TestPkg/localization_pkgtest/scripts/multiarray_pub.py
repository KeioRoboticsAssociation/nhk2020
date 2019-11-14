#!/usr/bin/env python
import rospy
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import MultiArrayDimension

if __name__ == "__main__":
    rospy.init_node("MultiArrayPublisher")
    rate = rospy.Rate(1)
    while not rospy.is_shutdown():
        array = Float32MultiArray()
        array.layout.dim.append(MultiArrayDimension())
        array.layout.dim[0].label = "array"
        array.layout.dim[0].size = 2
        array.data=[2.0,3.0]
        pub = rospy.Publisher("Float32MultiArray",Float32MultiArray,queue_size=10)
        pub.publish(array)
        rate.sleep()