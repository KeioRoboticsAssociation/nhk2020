#!/usr/bin/env python
import rospy
from std_msgs.msg import String

if __name__ == "__main__":
    rospy.init_node("test_node")
    pub = rospy.Publisher("chatter",String,queue_size=10)
    rate = rospy.Rate(1)
    
    while not rospy.is_shutdown():
        msg = String()
        msg.data = "Hello PassRobot"
        pub.publish(msg)
        rate.sleep()