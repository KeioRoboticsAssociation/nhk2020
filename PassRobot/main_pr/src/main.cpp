#include <mbed.h>
#include "mbedserial.h"
#define SUMPLING_TIME_US 50000
#define PI 3.141592f
#define PASS_MOTOR_PERIOD 20 // (ms)
#define PASS_MOTOR_DUTY 0.3f

Serial pc(USBTX, USBRX, 115200);
Ticker ticker;
Mbedserial Ms(pc);
PwmOut pwm_pass(PA_8);
DigitalOut myled(LED1);
InterruptIn button(USER_BUTTON);

float floatarray_plus = 0.5;
bool phase_pass = 1;

int main() {

  while(button);

  pwm_pass.period_ms(20);
  while(1) {
    pass_motor(Ms.getfloat);
  }
}

void pass_motor(float floatarray_plus) {
  if(floatarray_plus > 0) {
    phase_pass = 1;
    pwm_pass.write(floatarray_plus / 1.8);
  }
  else {
    phase_pass = 0;
    pwm_pass.write(floatarray_plus / 1.8);
  }
}