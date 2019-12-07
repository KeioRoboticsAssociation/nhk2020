#!/usr/bin/env python
#encoding: utf8
import rospy
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import MultiArrayDimension
import math

class Localization():
    
    def __init__(self):
        self.sub_a = rospy.Subscriber("array_a",Float32MultiArray,self.right_forward)
        self.sub_b = rospy.Subscriber("array_b",Float32MultiArray,self.left_forword)
        self.sub_c = rospy.Subscriber("array_c",Float32MultiArray,self.right_back)
        self.sub_d = rospy.Subscriber("array_d",Float32MultiArray,self.left_back)
        self.pub = rospy.Publisher("array_localization",Float32MultiArray,queue_size=10)
	self.l = 500
        self.theta_a = math.pi / 4
        self.r_a = self.l*math.sqrt(2) + self.l*math.sqrt(2)
        self.theta_b = 3 * math.pi / 4
        self.r_b = self.l*math.sqrt(2) + self.l*math.sqrt(2)
        self.theta_c = -math.pi / 4
        self.r_c = self.l*math.sqrt(2) + self.l*math.sqrt(2)
        self.theta_d = - 3 * math.pi / 4
        self.r_d = self.l*math.sqrt(2) + self.l*math.sqrt(2)

    def updateInput(self):
        x_a, y_a = self.right_forward()
        x_b, y_b = self.left_forward()
        x_c, y_c = self.right_back()
        x_d, y_d = self.left_back()
        x_0 = (x_a + x_b + x_c + x_d) / 4
        y_0 = (y_a + y_b + y_c + y_d) / 4
        theta = (math.atan(y_a - y_0 / x_a - x_0) + math.atan(y_b - y_0 / x_b - x_0) 
                + math.atan(y_c - y_0 / x_c - x_0) + math.atan(y_d - y_0 / x_d - x_0)) / 4
	msg = Float32MultiArray()
	msg.layout.dim.append(MultiArrayDimension())
	msg.layout.dim[0].label = "array"
	array.layout.dim[0].size = 2
	msg.data = [math.sqrt(x_0 ** 2 + y_0 ** 2), theta]   
	self.pub.publish(msg)
        return 0

    def right_forward(self,msg):
        delta_r_a = msg.data[0]
        delta_theta_a = msg.data[1]
        x_a = self.r_a - self.l*math.cos(theta)
        y_a = self.r_a - self.l*math.sin(theta)
        self.theta_a = self.theta_a + delta_theta_a
        '''
        r_a = r_a + delta_r_a * ((math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi / 2)),
                                    math.sin(theta + (theta_a - delta_theta_a / 2.0)))
        '''
	if delta_r_a*math.cos(theta) < 0:
		delta_x = - delta_r_a * math.cos(theta)
	else:
		delta_x = delta_r_a * math.sin(theta)
	if delta_r_a*math.sin(theta) < 0:
		delta_y = - delta_r_a * math.sin(theta)
	else:
		delta_y = delta_r_a * math.sin(theta)
        x_a = x_a + (delta_x) * (math.cos(theta + (self.theta_a - delta_theta_a / 2.0) + math.pi/2))
        y_a = y_a + (delta_y) * (math.sin(theta + (self.theta_a - delta_theta_a / 2.0) + math.pi/2))

        ##print("-------------")
        ##print(x)
        ##print(theta)
        return x_a, y_a

    def left_forword(self,msg):
        delta_r_b = msg.data[0]
        delta_theta_b = msg.data[1]
        x_b = self.r_b - self.l*math.cos(theta)
        y_b = self.r_b - self.l*math.sin(theta)
        self.theta_b = self.theta_b + delta_theta_b
	if delta_r_b*math.cos(theta) < 0:
		delta_x = - delta_r_b * math.cos(theta)
	else:
		delta_x = delta_r_b * math.sin(theta)
	if delta_r_b*math.sin(theta) < 0:
		delta_y = - delta_r_b * math.sin(theta)
	else:
		delta_y = delta_r_b * math.sin(theta)
        '''
        r_a = r_a + delta_r_a * ((math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi / 2)),
                                    math.sin(theta + (theta_a - delta_theta_a / 2.0)))
        '''
        x_b = x_b + (delta_x) * (math.cos(theta + (self.theta_b - delta_theta_b / 2.0) + math.pi/2))
        y_b = y_b + (delta_y) * (math.sin(theta + (self.theta_b - delta_theta_b / 2.0) + math.pi/2))

        ##print("-------------")
        ##print(x)
        ##print(theta)
        return x_b, y_b

        

    def right_back(self,msg):
        delta_r_c = msg.data[0]
        delta_theta_c = msg.data[1]
        x_c = self.r_c - self.l*math.cos(theta)
        y_c = self.r_c - self.l*math.sin(theta)
        self.theta_c = self.theta_c + delta_theta_c
	if delta_r_c*math.cos(theta) < 0:
		delta_x = - delta_r_c * math.cos(theta)
	else:
		delta_x = delta_r_c * math.sin(theta)
	if delta_r_c*math.sin(theta) < 0:
		delta_y = - delta_r_c * math.sin(theta)
	else:
		delta_y = delta_r_c * math.sin(theta)
        '''
        r_a = r_a + delta_r_a * ((math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi / 2)),
                                    math.sin(theta + (theta_a - delta_theta_a / 2.0)))
        '''
        x_c = x_c + (delta_x) * (math.cos(theta + (self.theta_c - delta_theta_c / 2.0) + math.pi/2))
        y_c = y_c + (delta_y) * (math.sin(theta + (self.theta_c - delta_theta_c / 2.0) + math.pi/2))

        ##print("-------------")
        ##print(x)
        ##print(theta)
        return x_c, y_c


    def left_back(self,msg):
        delta_r_d = msg.data[0]
        delta_theta_d = msg.data[1]
        x_d = self.r_d - self.l*math.cos(theta)
        y_d = self.r_d - self.l*math.sin(theta)
        self.theta_d = self.theta_d + delta_theta_d
	if delta_r_d*math.cos(theta) < 0:
		delta_x = - delta_r_d * math.cos(theta)
	else:
		delta_x = delta_r_d * math.sin(theta)
	if delta_r_d*math.sin(theta) < 0:
		delta_y = - delta_r_d * math.sin(theta)
	else:
		delta_y = delta_r_d * math.sin(theta)
        '''
        r_a = r_a + delta_r_a * ((math.cos(theta + (theta_a - delta_theta_a / 2.0) + math.pi / 2)),
                                    math.sin(theta + (theta_a - delta_theta_a / 2.0)))
        '''
        x_d = x_d + (delta_x) * (math.cos(theta + (self.theta_d - delta_theta_d / 2.0) + math.pi/2))
        y_d = y_d + (delta_y) * (math.sin(theta + (self.theta_d - delta_theta_d / 2.0) + math.pi/2))

        ##print("-------------")
        ##print(x)
        ##print(theta)
        return x_d, y_d


    

if __name__ == "__main__":
    rospy.init_node("Localization")
    l = Localization()
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        l.updateInput()
	rate.sleep()
    rospy.spin()
