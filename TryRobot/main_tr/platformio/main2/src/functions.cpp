// COMMON.cpp

#include "common.h"

float bno_euler_scale = 1;
float bno_offset = 0.0f; // (rad)
float bno_old = 0.0f;    // (rad)
float bno_angle = 0.0f;  // old - offset (rad)
float servo_angle = 0.0;

/************************function****************************/

RDS5160Servo::RDS5160Servo(PwmOut &PWM_Pin, uint32_t neutral, bool direction)
    : PWM(PWM_Pin), NEUTRAL(neutral), DIRECTION(direction) {
    PWM.period_us(2500);
}

void RDS5160Servo::Neutral(void) {
    PWM.pulsewidth_us(NEUTRAL);
}

void RDS5160Servo::MoveTo(float degree) {
    int32_t pulsewidth = (int32_t)((degree * 7.41f) * (DIRECTION ? 1 : -1) + NEUTRAL);
    PWM.pulsewidth_us(abs(pulsewidth));
}

// bno
void bno_init()
{
  bno.initialize();
  bno_euler_scale = bno.getEulerScale();
  get_angle();
  set_offset();
}

void set_offset()
{
  bno_offset = bno_old;
}

void get_angle()
{
  static float angle_raw = 0.0f;
  static int n_pi = 0;
  short dataBox[3] = {0};
  // get data
  bno.getEulerDataAll(dataBox[2], dataBox[1], dataBox[0]);
  angle_raw = -(float)dataBox[2] * bno_euler_scale * PI / 180.0f;
  if (angle_raw + PI * (float)(n_pi - 1) > bno_old)
    n_pi -= 2;
  else if (angle_raw + PI * (float)(n_pi + 1) < bno_old)
    n_pi += 2;
  float bno_old_temp = angle_raw + PI * (float)n_pi;
  if(abs(bno_old_temp-bno_old) < PI/9 || bno_old_temp == 0.0f)
    bno_old = bno_old_temp;
  bno_angle = bno_old - bno_offset;
}

// kick
void kickandhold()
{
  // kick
  kick1 = 1;
  waittime_ms(KICK_WAIT_TIME_MS); // delay
  kick2 = 1;
  waittime_ms(3000); // wait
  // return
  kick1 = 0;
  kick2 = 0;
  kick1_2 = 1;
  kick2_2 = 1;
  waittime_ms(5000); // wait
  // switch off
  kick1_2 = 0;
  kick2_2 = 0;
}

// try_motor
void try_motor(float degree, int time_ms)
{
    for (int i = 0; i < time_ms; i += 50)
    {
        servo1.MoveTo(servo_angle + (degree - servo_angle) * (float)i / (float)time_ms);
        servo2.MoveTo(servo_angle + (degree - servo_angle) * (float)i / (float)time_ms);
        waittime_ms(50);
    }
    servo1.MoveTo(degree);
    servo2.MoveTo(degree);
    servo_angle = degree;
}