#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/String.h"

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int open_serial(const char *device_name){
    int fd1=open(device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    fcntl(fd1, F_SETFL,0);
    //load configuration
    struct termios conf_tio;
    tcgetattr(fd1,&conf_tio);
    //set baudrate
    speed_t BAUDRATE = B115200;
    cfsetispeed(&conf_tio, BAUDRATE);
    cfsetospeed(&conf_tio, BAUDRATE);
    //non canonical, non echo back
    conf_tio.c_lflag &= ~(ECHO | ICANON);
    //non blocking
    conf_tio.c_cc[VMIN]=0;
    conf_tio.c_cc[VTIME]=0;
    //store configuration
    tcsetattr(fd1,TCSANOW,&conf_tio);
    return fd1;
}

int fd1;
char endmsg = '\n';

void float_callback(const std_msgs::Float32MultiArray& serial_msg){
    int datasize = serial_msg.data.size();
    char *floattochar
    floattochar = new char[datasize*4+6];
    floattochar[0] = 'f';
    memcpy(&floattochar[1],datasize,4);
    for(int i=0;i<datasize;i++){
        memcpy(&floattochar[i*4+5],&serial_msg.data[i],4);
    }
    floattochar[datasize*4+5] = endmsg;

    int rec=write(fd1,floattochar,datasize*4+6);
    if(rec<0){
        ROS_ERROR_ONCE("Serial Fail: cound not write");
    }
    delete [] floattochar;
}

void int_callback(const std_msgs::Int32MultiArray& serial_msg){
    int datasize = serial_msg.data.size();
    char *inttochar
    inttochar = new char[datasize*4+6];
    inttochar[0] = 'i';
    memcpy(&inttochar[1],datasize,4);
    for(int i=0;i<datasize;i++){
        memcpy(&inttochar[i*4+5],&serial_msg.data[i],4);
    }
    inttochar[datasize*4+5] = endmsg;

    int rec=write(fd1,inttochar,datasize*4+6);
    if(rec<0){
        ROS_ERROR_ONCE("Serial Fail: cound not write");
    }
    delete [] inttochar;
}

void string_callback(const std_msgs::String& serial_msg){
    int datasize = serial_msg.data.size();
    char *chartochar
    chartochar = new char[datasize+6];
    chartochar[0] = 'c';
    memcpy(&chartochar[1],datasize,4);
        memcpy(&chartochar[5],&serial_msg.data[i],datasize);
    chartochar[datasize+6] = endmsg;

    int rec=write(fd1,chartochar,datasize+6);
    if(rec<0){
        ROS_ERROR_ONCE("Serial Fail: cound not write");
    }
    delete [] chartochar;
    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "serial_test_node");
    ros::NodeHandle n;

    //Publisher
    ros::Publisher serial_pub_f = n.advertise<std_msgs::Float32MultiArray.h>("Serial_pub_float", 1000);
    ros::Publisher serial_pub_i = n.advertise<std_msgs::Int32MultiArray.h>("Serial_pub_int", 1000);
    ros::Publisher serial_pub_c = n.advertise<std_msgs::String>("Serial_pub_string", 1000);

    //Subscriber
    ros::Subscriber serial_sub_f = n.subscribe("Serial_sub_float", 10, float_callback);
    ros::Subscriber serial_sub_i = n.subscribe("Serial_sub_int", 10, int_callback);  
    ros::Subscriber serial_sub_c = n.subscribe("Serial_sub_string", 10, string_callback);

    char device_name[]="/dev/ttyACM0";
    fd1=open_serial(device_name);
    if(fd1<0){
        ROS_ERROR("Serial Fail: cound not open %s", device_name);
        printf("Serial Fail\n");
        ros::shutdown();
    }

    char *buf_pub;
    buf_pub = new char[256];
    int recv_data_size = 0;
    int arraysize = 0;
    ros::Rate loop_rate(200); 
    while (ros::ok()){
        char buf[256]={0};
        int recv_data=read(fd1, buf, sizeof(buf));

        if(recv_data>0){
            strcat(buf_pub,buf);
            recv_data_size += recv_data;
            if(buf_pub[recv_data_size-1] == endmsg){
                switch(buf_pub[0]){
                    case 'f':
                    memcpy(&arraysize,&buf_pub[1],4);
                    if(recv_data_size==arraysize*4+6){
                        std_msgs::Float32MultiArray pub_float;
                        pub_float.data.resize(arraysize);
                        for(int i=0;i<arraysize;i++){
                            memcpy(&pub_float.data[i],&buf_pub[i*4+5],4);
                        }
                        serial_pub_f.publish(pub_float);
                    }
                    break;
                    case 'i':
                    memcpy(&arraysize,&buf_pub[1],4);
                    if(recv_data_size==arraysize*4+6){
                        std_msgs::Int32MultiArray pub_int;
                        pub_int.data.resize(arraysize);
                        for(int i=0;i<arraysize;i++){
                            memcpy(&pub_int.data[i],&buf_pub[i*4+5],4);
                        }
                        serial_pub_i.publish(pub_int);
                    }
                    break;
                    case 'c':
                    memcpy(&arraysize,&buf_pub[1],4);
                    if(recv_data_size==arraysize+6){
                        std_msgs::String pub_string;
                        memcpy(&pub_string.data[0],&buf_pub[5],arraysize);
                        serial_pub_c.publish(pub_string);
                    }
                }

                delete[] buf_pub;
                recv_data_size = 0;
                buf_pub = new char[256];
            }
        }
        
        ros::spinOnce();
        loop_rate.sleep();
    } 
    return 0;
}