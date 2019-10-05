#!/usr/bin/env python
#encoding: utf8

import rospy
from std_msgs.msg import Float32MultiArray
from geometry_msgs.msg import Twist

class MultiArrayConverter():
    def __init__(self):
        self.pub_cmdvel = rospy.Publisher("cmd_vel",Twist,queue_size=10)
        self.sub_cmdvel = rospy.Subscriber("array",Float32MultiArray,self.to_cmdvel)
        ## euler角をSubscribeするインスタンスの生成

    def to_cmdvel(self,msg):
        print("=================================")
        print("Float32MultiArray [%f , %f] " % (msg.data[0],msg.data[1]))
        m = Twist()
        m.linear.x = msg.data[0]
        m.angular.z = msg.data[1]
        print("---------------------------------")
        print("linear.x : %f" % m.linear.x)
        print("angular.z : %f" % m.angular.z)
        self.pub_cmdvel.publish(m)
        return 0

    ## オイラー角を配信するcallback関数
    def to_euler(self):
        return 0


if __name__ == "__main__":
    rospy.init_node("multiarrayConverter")
    conv = MultiArrayConverter()
    rospy.spin()