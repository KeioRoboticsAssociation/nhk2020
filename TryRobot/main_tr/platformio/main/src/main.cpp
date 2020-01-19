#include <mbed.h>
#include <BNO055.h>
#include "mbedserial.h"

/*********************** Param *************************/
#define SUMPLING_TIME_US 10000
#define PI 3.141592f
#define TRY_MOTOR_PERIOD 20 // (ms)
#define TRY_MOTOR_DUTY 0.2f
#define KICK_WAIT_TIME_MS 100

float bno_offset = 0.0f; // (rad)
float bno_old = 0.0f;    // (rad)
float bno_angle = 0.0f;  // old - offset (rad)

float timecount = 0;
/*******************************************************/

Serial pc(USBTX, USBRX, 115200);
BNO055 bno(PB_7, PB_8); // SDA,ACL
Ticker ticker;
Mbedserial Ms(pc);
PwmOut pwm_try(PA_8);
DigitalOut phase_try(PC_11);
DigitalOut kick1(PC_5);
DigitalOut kick2(PB_12);
DigitalOut myled(LED1);
InterruptIn button(USER_BUTTON);

/******************* Function *************************/
void Push();
void Pull();
void bno_init();
void set_offset();
void get_angle();
void try_motor(float);
void kickandhold(int);
void timer_interrupt();
void waittime_ms(float);

int replyflag = 1;
bool modezeroflag = false;
/******************************************************/

// subscribe_int: flag, mode, try_motor
// publish_float: bno
// publish_int: reply

int main()
{
  button.fall(Push);
  button.rise(Pull);
  bno_init();
  pwm_try.period_ms(TRY_MOTOR_PERIOD);
  ticker.attach_us(&timer_interrupt, SUMPLING_TIME_US);

  while (1)
  {
    switch (Ms.getint[0]) // flag
    {
    case 1: // stop
      try_motor(0);
      break;
    case 2: // reset
      set_offset();
      break;
    default:
      switch (Ms.getint[1]) // mode
      {
      case 0: //others
        modezeroflag = true;
        break;
      case 1:
      case 2:
        if (modezeroflag)
          break;
        modezeroflag = false;
        replyflag = 0;
        waittime_ms(500);
        replyflag = 1;
        break;
      case 3: // kick
        if (modezeroflag)
          break;
        modezeroflag = false;
        replyflag = 0;
        waittime_ms(500);
        kickandhold(0);
        replyflag = 1;
        break;
      }
      // try_motor
      if (Ms.getint[2] == 1)
        try_motor(TRY_MOTOR_DUTY);
      else if (Ms.getint[2] == -1)
        try_motor(-TRY_MOTOR_DUTY);
      else
        try_motor(0);
    }
    waittime_ms(100);
  }
}

void Push()
{
  myled = 1;
}

void Pull()
{
  myled = 0;
}

void bno_init()
{
  if (!bno.check())
  {
    while (1)
    {
      myled = !myled;
      wait(0.1);
    }
  }
  myled = 0;

  bno.setmode(OPERATION_MODE_IMUPLUS);
  set_offset();
}

void set_offset()
{
  bno_offset = bno_old;
}

void get_angle()
{
  static float angle_raw = 0.0;
  static int n_pi = 0;
  bno.get_angles();
  angle_raw = bno.euler.yaw * PI / 180.0f;
  if (angle_raw + PI * (float)(n_pi + 1) > bno_old)
    n_pi -= 2;
  else if (angle_raw + PI * ((float)n_pi - 1) < bno_old)
    n_pi += 2;
  bno_old = angle_raw + PI * (float)n_pi;
  bno_angle = bno_old - bno_offset;
}

void try_motor(float duty_)
{
  if (duty_ < 0)
  {
    phase_try = 0;
    pwm_try.pulsewidth_ms(-duty_ * (float)TRY_MOTOR_PERIOD);
  }
  else
  {
    phase_try = 1;
    pwm_try.pulsewidth_ms(duty_ * (float)TRY_MOTOR_PERIOD);
  }
}

void kickandhold(int stop_)
{
  kick1 = 1;
  waittime_ms(KICK_WAIT_TIME_MS);

  kick2 = 1;

  waittime_ms(1000);
  kick1 = 0;
  kick2 = 0;
}
void timer_interrupt()
{
  static bool writeflag = true;
  if (writeflag)
    Ms.float_write(&bno_angle, 1);
  else
    Ms.int_write(&replyflag, 1);
  writeflag = !writeflag;

  timecount += (float)SUMPLING_TIME_US / 1000.0f;
}

void waittime_ms(float t)
{
  timecount = 0;
  while (timecount < t)
  {
    get_angle();
    wait_ms(10);
  }
  timecount = 0;
}