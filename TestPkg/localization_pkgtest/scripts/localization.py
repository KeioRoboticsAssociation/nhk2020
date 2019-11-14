#!/usr/bin/env python
#encoding: utf8
import rospy
from std_msgs.msg import Float32MultiArray

def calcLocalization(msg):
    x = msg.data[0]
    theta = msg.data[1]
    print("-------------")
    print(x)
    print(theta)

if __name__ == "__main__":
    rospy.init_node("Localization")
    sub = rospy.Subscriber("Float32MultiArray",Float32MultiArray,calcLocalization)
    rospy.spin()