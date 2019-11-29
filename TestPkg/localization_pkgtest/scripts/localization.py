#!/usr/bin/env python
#encoding: utf8
import rospy
from std_msgs.msg import Float32MultiArray
import math

def updateInput(msg):
    x = msg.data[0]
    theta = msg.data[1]
    theta = 0
    delta_r_a = 0
    delta_theta_a = 0
    r_a = 0
    l = 500
    x_a = r_a - l*math.cos(theta)
    y_a = r_a - l*math.sin(theta)
    theta_a = theta_a + delta_theta_a
    '''
    r_a = r_a + delta_r_a * ((math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi / 2)),
                                math.sin(theta + (theta_a - delta_theta_a / 2.0)))
    '''
    x_a = x_a + delta_r_a * math.cos(theta) * (math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi))
    y_a = y_a + delta_r_a * math.sin(theta) * (math.sin(theta + (theta_a - delta_theta_a / 2.0) + math.pi))

    print("-------------")
    print(x)
    print(theta)
    calcLocalization(x_a,y_a,0,0,0,0,0,0)

def calcLocalization(x_a,y_a,x_b,y_b,x_c,y_c,x_d,y_d):
    x_0 = (x_a + x_b + x_c + x_d) / 4
    y_0 = (y_a + y_b + y_c + y_d) / 4
    theta = (math.atan(y_a - y_0 / x_a - x_0) + math.atan(y_b - y_0 / x_b - x_0) 
            + math.atan(y_c - y_0 / x_c - x_0) + math.atan(y_d - y_0 / x_d - x_0)) / 4
    return x_0, y_0, theta

if __name__ == "__main__":
    rospy.init_node("Localization")
    sub = rospy.Subscriber("Float32MultiArray",Float32MultiArray,updateInput)
    rospy.spin()