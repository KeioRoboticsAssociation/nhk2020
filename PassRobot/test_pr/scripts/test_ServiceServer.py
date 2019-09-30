#!/usr/bin/env python
import rospy
from std_srvs.srv import Trigger, TriggerResponse

def serviceServer(srv):
    res = TriggerResponse()
    res.success = True
    res.message = "I receive service client"
    return res

if __name__ == "__main__":
    rospy.init_node("server")
    server = rospy.Service("service",Trigger,serviceServer)
    print("Service Server start!")
    print("--------------------------")
    rospy.spin()