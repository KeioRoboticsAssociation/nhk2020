#include <mbed.h>
#include "mbedserial.h"

Serial pc(USBTX, USBRX, 115200);
Mbedserial Ms(pc);
DigitalOut myled(LED1);
InterruptIn switch1(USER_BUTTON);

void led_light(){
  myled = !myled;
}

int main(){
  myled = 0;
  while(switch1);

  myled = 1;
  wait(1);
  myled = 0;
  wait(1);

  while(1){

    //int_attach(led_light);

    if(0 <= Ms.getfloat[1] && Ms.getint[0] == 1){
      myled = 1;
    }
    else if(Ms.getfloat[1] < 0 && Ms.getint[0] == 1){
      myled = 0;
    }

    // if(2 <= Ms.floatarraysize){
    //   myled = 1;
    // }
    // else{
    //   myled = 0;
    // }

    wait_ms(20);
  }
}