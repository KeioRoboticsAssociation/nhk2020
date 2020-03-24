#include <mbed.h>
#include "mbedserial.h"
#define SUMPLING_TIME_US 50000
#define PI 3.141592f
#define PASS_MOTOR_PERIOD 20 // (ms)
#define PASS_MOTOR_DUTY 0.3f

Serial pc(USBTX, USBRX, 115200);
Ticker ticker1, ticker2;
Mbedserial Ms(pc);

PwmOut pwm_pass(PA_8);
DigitalOut phase_pass(PA_11);
DigitalOut myled(LED1);
InterruptIn switch1(USER_BUTTON);
InterruptIn enA(PA_9);
DigitalOut enB(PB_4);
// bool phase_pass = 1;

//encoder
int en_count = 0, old_en_count = 0, d_en_count = 0;
//PID
float Kp = 1, Ki = 0, Kd = 0, diff[2], integral;
float control_speed;

void encoder();
void speed_calc();
void PID();
void pass_motor();

int main() {
  myled = 0;
  while(switch1);

  myled = 1;
  wait(1);
  myled = 0;

  ticker1.attach_us(&speed_calc, SUMPLING_TIME_US);
  ticker2.attach_us(&pass_motor, SUMPLING_TIME_US);
  enA.rise(encoder);
  pwm_pass.period_ms(20);
  while(1) {
    control_speed = Ms.getfloat[0];
    wait_ms(20);
  }
}

void pass_motor() {
  float target_speed = PID(control_speed);
  if(target_speed > 0) {
    phase_pass = 1;
    pwm_pass.write(target_speed);
  }
  else {
    phase_pass = 0;
    pwm_pass.write(-target_speed);
  }
}

void encoder() {
  if(enB == 1) en_count += 1; //todo : pulse to degree
  else en_count -= 1;
}

void speed_calc() {
  d_en_count = en_count - old_en_count;
  old_en_count = en_count;
}

float PID(float control_speed) {
  float p, i, d;
  diff[0] = diff[1];
  diff[1] = control_speed - (d_en_count); //d_encount : degree/sec to m/sec
  integral += (diff[0] + diff[1]) / 2;
  p = Kp * diff[1];
  i = Ki * integral * SUMPLING_TIME_US / (float)1000000;
  d = (diff[0] - diff[1]) * (float)1000000 / SUMPLING_TIME_US;
  float res = control_speed + p + i + d;
  if(res > 0.8) res = 0.8;
  else if(res < -0.8) res = -0.8;
  return res;
}