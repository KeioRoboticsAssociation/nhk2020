#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist

def callback(msg):
    print("linear.x : ", msg.linear.x)
    print("angular.z : ", msg.angular.z)

if __name__ == "__main__":
    rospy.init_node("cmd_vel_receiver")
    sub = rospy.Subscriber("cmd_vel",Twist,callback)
    rospy.spin()