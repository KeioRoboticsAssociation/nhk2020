#!/usr/bin/env python
import rospy
from std_srvs.srv import Trigger

if __name__ == "__main__":
    rospy.init_node("client")
    rospy.wait_for_service("service")
    try:
        srv = rospy.ServiceProxy("service",Trigger)
        on = srv()
        print(on.success)
        print(on.message)
    except rospy.ServiceException as e:
        print("Service call failed: %s" % e)
    rospy.spin()