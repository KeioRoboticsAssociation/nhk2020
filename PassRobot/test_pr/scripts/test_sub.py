#!/usr/bin/env python
import rospy
from std_msgs.msg import String

def msgCallback(msg):
    print("-----------------")
    print("I subscribe %s" % msg.data)

if __name__ == "__main__":
    rospy.init_node("Sub")
    sub = rospy.Subscriber("chatter",String,msgCallback)
    rospy.spin()