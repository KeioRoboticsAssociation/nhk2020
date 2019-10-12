#!/usr/bin/env python
import rospy,os
import SimpleHTTPServer

def kill():
    os.system("kill -KILL" + str(os.getgid()))

os.chdir(os.path.dirname(__file__) + "/../WebGUI")
rospy.init_node("webserver")
rospy.on_shutdown(kill)
SimpleHTTPServer.test()