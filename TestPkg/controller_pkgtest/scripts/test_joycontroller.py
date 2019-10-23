#!/usr/bin/env python
#encoding: utf8

import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist

if __name__ == "__main__":
    rospy.init_node("Joyconverter")
    rospy.spin()
