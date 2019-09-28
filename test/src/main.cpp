#include <mbed.h>
#include <ros.h>

DigitalOut myled(LED1);

ros::NodeHandle nh;

int main(){
    nh.getHardware()->setBaud(115200);
    nh.initNode();

    while(1){
        myled = 1;
        wait_ms(1000);
        myled = 0;
        wait_ms(1000);
    }
}