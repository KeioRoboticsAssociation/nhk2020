#!/usr/bin/env python
import rospy
from std_msgs.msg import Float32MultiArray
from geometry_msgs.msg import Twist

def callback(msg):
    print(msg.data)
    print(msg.data[0])
    print(msg.data[1])
    m = Twist()
    m.linear.x = msg.data[0]
    m.angular.z = msg.data[1]
    pub.publish(m)

if __name__ == "__main__":
    rospy.init_node("multiarrayConverter")
    sub = rospy.Subscriber("array",Float32MultiArray,callback)
    pub = rospy.Publisher("cmd_vel",Twist,queue_size=10)
    rospy.spin()