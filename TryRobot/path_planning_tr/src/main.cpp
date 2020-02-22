#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
#include <tf/transform_listener.h>
//#include <nav_msgs/Odometry.h>

#include "path_planning.h"

#define LINE_NUM 26

Path path[LINE_NUM];
/*
+++++ KEY +++++
0: start
1: receive
2~6: try1~5
7~9: kick1~3
m: manual_mode
p: show_path
F1: change_Red<->Blue
F2: create csv Blue -> Red

+++++ path_mode +++++
0: none
1: start -> recieve
2~6: receive -> try1~5
7: try1,2 -> kick1
8: try3~5 -> kick1
9: try1,2 -> kick2
10: try3~5 -> kick2
11: try1 -> kick3
12: try2~5 -> kick3
13: kick1 -> receive
14: kick2 -> receive
15: kick3 -> receive
16~20: start -> try1~5
21~23: start -> kick1~3
24: kick1 -> kick2
25: kick2 -> kick3
26: kick1 -> kick3
*/

int armmode = 0;	// 2:reset
int mode = 0;		// 0:start, 1:receive, 2~6:try1~5, 7~9:kick1~3
int path_mode = 0;	// 0:none, 1:path1, 2:path2, ...LINE_NUM
int getmode = 0;	// 0:else, 1:start, 2:receive, 3:try, 4~6:kick1~3, 7:change_RB
int trymode = 0;	// 0:else 1~5:try1~5
bool forwardflag = true;
bool redflag = true;
float pos[3] = {0};	// [x,y,theta]

void change_RB();
void set_mode(int next);
void csv_converter(string infilename);

void odomCallback(const std_msgs::Float32MultiArray &msg)
{
    pos[0] = msg.pose.pose.position.x;
    pos[1] = msg.pose.pose.position.y;
}

void buttonCallback(const std_msgs::Float32MultiArray &msg)
{
	armmode = msg.data[1];
	getmode = msg.data[3];
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "path_planning_node");

    ros::NodeHandle n;
    ros::Publisher path_pub = n.advertise<std_msgs::Float32MultiArray>("path_control", 10);
    
	ros::Subscriber pathmode_sub = n.subscribe("arm_mbed", 100, buttonCallback);
	ros::Subscriber odom_sub = n.subscribe("odom", 100, odomCallback);

    ros::NodeHandle arg_n("~");
    int looprate = 30;      // Hz
    arg_n.getParam("frequency", looprate);

    change_RB();

	// tf listener
	//////tf::TransformListener listener;

    ros::Rate loop_rate(looprate);
    while (ros::ok())
    {
		// tf_listner
		/*
		tf::StampedTransform transform;
		try{
			listener.lookupTransform("/odom", "/base_link",  ros::Time(0), transform);
		}
		catch (tf::TransformException ex){
			ROS_ERROR("%s", ex.what());
			ros::Duration(1.0).sleep();
		}
		pos[0] = transform.getOrigin().x();
		pos[1] = transform.getOrigin().y();
		*/

		// pure_pursuit
        Matrix control(4, 1);
        if (path_mode != 0)
        {
            control = path[path_mode - 1].pure_pursuit(pos[0], pos[1], forwardflag);
            if (forwardflag)
            {
                if (control[4][1] >= path[path_mode - 1].pnum - 0.03)
                    path_mode = 0;
            }else{
                if (control[4][1] <= 1.03)
                    path_mode = 0;
            }
			std_msgs::Float32MultiArray floatarray;
			floatarray.data.resize(3);
			floatarray.data[0] = control[1][1];
			floatarray.data[1] = control[2][1];
			floatarray.data[2] = control[3][1];
			rogi_pub.publish(floatarray);
		}

		// change path
		switch(getmode){
		case 1:
			set_mode(0);
			break;
		case 2:
			set_mode(1);
			break;
		case 3:
			set_mode(trymode + 1);
			break;
		case 4:case 5:case 6:
			set_mode(trymode + 3);
			trymode++;
			if (trymode == 6)
				trymode = 1;
			break;
		case 7:
			change_RB();
			break;
		}

		ros::spinOnce();
		loop_rate.sleep();
    }
}

void change_RB() {
	for (int i = 0; i < LINE_NUM; i++) {
		stringstream ss;
		if (redflag) {
			ss << "data_csv/path_point_blue" << i + 1 << ".csv";
		}
		else {
			ss << "data_csv/path_point_red" << i + 1 << ".csv";
		}
		path[i].set_point_csv(ss.str());
	}
	redflag = !redflag;
}

void set_mode(int next) {
	//if (path_mode != 0)
	//	return;
	bool forward = true;
	switch (mode) {
	case 0:	// start
		if (next == 1)
			path_mode = 1;
		else if (2 <= next && next <= 9)
			path_mode = next + 14;
		else
			return;
		break;
	case 1:	// receive
		if (next == 0) {
			forward = false;
			path_mode = 1;
		}
		else if (2 <= next && next <= 6)
			path_mode = next;
		else
			return;
		break;
	case 2:	// try1
		if (next == 0) {
			forward = false;
			path_mode = 16;
		}
		else if (next == 1) {
			forward = false;
			path_mode = 2;
		}
		else if (next == 7)
			path_mode = 7;
		else if (next == 8)
			path_mode = 9;
		else if (next == 9)
			path_mode = 11;
		else
			return;
		break;
	case 3:	// try2
		if (next == 0) {
			forward = false;
			path_mode = 17;
		}
		else if (next == 1) {
			forward = false;
			path_mode = 3;
		}
		else if (next == 7)
			path_mode = 7;
		else if (next == 8)
			path_mode = 10;
		else if (next == 9)
			path_mode = 12;
		else
			return;
		break;
	case 4:	// try3
		if (next == 0) {
			forward = false;
			path_mode = 18;
		}
		else if (next == 1) {
			forward = false;
			path_mode = 4;
		}
		else if (next == 7)
			path_mode = 8;
		else if (next == 8)
			path_mode = 10;
		else if (next == 9)
			path_mode = 12;
		else
			return;
		break;
	case 5:	// try4
		if (next == 0) {
			forward = false;
			path_mode = 19;
		}
		else if (next == 1) {
			forward = false;
			path_mode = 5;
		}
		else if (next == 7)
			path_mode = 8;
		else if (next == 8)
			path_mode = 10;
		else if (next == 9)
			path_mode = 12;
		else
			return;
		break;
	case 6:	// try5
		if (next == 0) {
			forward = false;
			path_mode = 20;
		}
		else if (next == 1) {
			forward = false;
			path_mode = 6;
		}
		else if (next == 7)
			path_mode = 8;
		else if (next == 8)
			path_mode = 10;
		else if (next == 9)
			path_mode = 12;
		else
			return;
		break;
	case 7:	// kick1
		if (next == 0) {
			forward = false;
			path_mode = 21;
		}
		else if (next == 1)
			path_mode = 13;
		else if (next == 8)
			path_mode = 24;
		else if (next == 9)
			path_mode = 26;
		else
			return;
		break;
	case 8:	// kick2
		if (next == 0) {
			forward = false;
			path_mode = 22;
		}
		else if (next == 1)
			path_mode = 14;
		else if (next == 7) {
			forward = false;
			path_mode = 24;
		}
		else if (next == 9)
			path_mode = 25;
		else
			return;
		break;
	case 9:	// kick3
		if (next == 0) {
			forward = false;
			path_mode = 23;
		}
		else if (next == 1)
			path_mode = 15;
		else if (next == 7) {
			forward = false;
			path_mode = 26;
		}
		else if (next == 8) {
			forward = false;
			path_mode = 25;
		}
		else
			return;
		break;
	}
	mode = next;
	if (forward) {
		forwardflag = true;
		path[path_mode - 1].pure_pursuit(pos[0], pos[1], forwardflag, 1);
	}
	else {
		forwardflag = false;
		path[path_mode - 1].pure_pursuit(pos[0], pos[1], forwardflag, path[path_mode - 1].pnum);
	}
}