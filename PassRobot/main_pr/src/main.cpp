#include <mbed.h>
#include "mbedserial.h"
#define SUMPLING_TIME_US 50000
#define PI 3.141592f
#define PASS_MOTOR_PERIOD 20 // (ms)
#define PASS_MOTOR_DUTY 0.3f

Serial pc(USBTX, USBRX, 115200);
Ticker ticker;
Mbedserial Ms(pc);

PwmOut pwm_pass(PA_8);  //
DigitalOut phase_pass(PA_11);  //
DigitalOut myled(LED1);
InterruptIn button(USER_BUTTON);

float floatarray_plus = 0.5;
// bool phase_pass = 1;

void pass_motor(float floatarray_plus) {
  if(floatarray_plus > 0) {
    phase_pass = 1;
    pwm_pass.write(floatarray_plus);
  }
  else {
    phase_pass = 0;
    pwm_pass.write(-floatarray_plus);
  }
}

int main() {
  myled = 0;
  while(!button);

  myled = 1;
  wait(1);
  myled = 0;
  wait(1);

  pwm_pass.period_ms(20);
  while(1) {
    pass_motor(Ms.getfloat[0]);
    if(Ms.getint[0] == 1) myled = 1;
    else myled = 0;
    wait_ms(20);
  }
}
