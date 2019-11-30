#!/usr/bin/env python
#encoding: utf8
import rospy
from std_msgs.msg import Float32MultiArray
import math

class Localization():
    
    def __init__(self):
        self.sub_a = rospy.Subscriber("Float32MultiArray",Float32MultiArray,self.updateInput)
        self.sub_b = rospy.Subscriber("Float32MultiArray",Float32MultiArray,self.updateInput)
        self.sub_c = rospy.Subscriber("Float32MultiArray",Float32MultiArray,self.updateInput)
        self.sub_d = rospy.Subscriber("Float32MultiArray",Float32MultiArray,self.updateInput)
        self.pub = rospy.Publisher("Float32MultiArray",Float32MultiArray,queue_size=10)

    def updateInput(msg):
        x_a, y_a = self.right_forward()
        x_b, y_b = self.left_forward()
        x_c, y_c = self.right_back()
        x_d, y_d = self.left_back()
        x_0 = (x_a + x_b + x_c + x_d) / 4
        y_0 = (y_a + y_b + y_c + y_d) / 4
        theta = (math.atan(y_a - y_0 / x_a - x_0) + math.atan(y_b - y_0 / x_b - x_0) 
                + math.atan(y_c - y_0 / x_c - x_0) + math.atan(y_d - y_0 / x_d - x_0)) / 4
        return x_0, y_0, theta

    def right_forward(msg):
        theta_a = 0
        r_a = 0
        delta_r_a = msg.data[0]
        delta_theta_a = msg.data[1]
        l = 500
        x_a = r_a - l*math.cos(theta)
        y_a = r_a - l*math.sin(theta)
        theta_a = theta_a + delta_theta_a
        '''
        r_a = r_a + delta_r_a * ((math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi / 2)),
                                    math.sin(theta + (theta_a - delta_theta_a / 2.0)))
        '''
        x_a = x_a + (delta_r_a * math.cos(theta)) * (math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi))
        y_a = y_a + (delta_r_a * math.sin(theta)) * (math.sin(theta + (theta_a - delta_theta_a / 2.0) + math.pi))

        ##print("-------------")
        ##print(x)
        ##print(theta)
        return x_a, y_a

    def left_forword():
        theta_b = 0
        r_b = 0
        delta_r_b = msg.data[0]
        delta_theta_b = msg.data[1]
        l = 500
        x_b = r_b - l*math.cos(theta)
        y_b = r_b - l*math.sin(theta)
        theta_b = theta_b + delta_theta_b
        '''
        r_a = r_a + delta_r_a * ((math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi / 2)),
                                    math.sin(theta + (theta_a - delta_theta_a / 2.0)))
        '''
        x_b = x_b + (delta_r_b * math.cos(theta)) * (math.cos(theta + (theta_b - delta_theta_b / 2.0) + math.pi))
        y_b = y_b + (delta_r_b * math.sin(theta)) * (math.sin(theta + (theta_b - delta_theta_b / 2.0) + math.pi))

        ##print("-------------")
        ##print(x)
        ##print(theta)
        return x_b, y_b

        

    def right_back():
        theta_c = 0
        r_c = 0
        delta_r_c = msg.data[0]
        delta_theta_c = msg.data[1]
        l = 500
        x_c = r_c - l*math.cos(theta)
        y_c = r_c - l*math.sin(theta)
        theta_c = theta_c + delta_theta_c
        '''
        r_a = r_a + delta_r_a * ((math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi / 2)),
                                    math.sin(theta + (theta_a - delta_theta_a / 2.0)))
        '''
        x_c = x_c + (delta_r_c * math.cos(theta)) * (math.cos(theta + (theta_c - delta_theta_c / 2.0) + math.pi))
        y_c = y_c + (delta_r_c * math.sin(theta)) * (math.sin(theta + (theta_c - delta_theta_c / 2.0) + math.pi))

        ##print("-------------")
        ##print(x)
        ##print(theta)
        return x_c, y_c


    def left_back():
        theta_d = 0
        r_d = 0
        delta_r_d = msg.data[0]
        delta_theta_d = msg.data[1]
        l = 500
        x_d = r_d - l*math.cos(theta)
        y_d = r_d - l*math.sin(theta)
        theta_d = theta_d + delta_theta_d
        '''
        r_a = r_a + delta_r_a * ((math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi / 2)),
                                    math.sin(theta + (theta_a - delta_theta_a / 2.0)))
        '''
        x_d = x_d + (delta_r_d * math.cos(theta)) * (math.cos(theta + (theta_d - delta_theta_d / 2.0) + math.pi))
        y_d = y_d + (delta_r_d * math.sin(theta)) * (math.sin(theta + (theta_d - delta_theta_d / 2.0) + math.pi))

        ##print("-------------")
        ##print(x)
        ##print(theta)
        return x_d, y_d


    

if __name__ == "__main__":
    rospy.init_node("Localization")
    l = Localization()
    rospy.spin()