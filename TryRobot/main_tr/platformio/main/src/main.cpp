#include <mbed.h>
#include <BNO055.h>
#include "mbedserial.h"

/*********************** Param *************************/
#define PI 3.141592f
#define TRY_MOTOR_PERIOD 20 // (ms)
#define TRY_MOTOR_DUTY 0.3f
#define KICK_WAIT_TIME_MS 0 // (ms)

float bno_offset = 0.0f; // (rad)
float bno_old = 0.0f;    // (rad)
float bno_angle = 0.0f;  // old - offset (rad)

/*******************************************************/

Serial pc(USBTX, USBRX, 115200);
BNO055 bno(PB_7, PB_8); // SDA,ACL
Ticker ticker;
Mbedserial Ms(pc);
PwmOut pwm_try(PA_8);
DigitalOut phase_try(PC_11);
DigitalOut kick1(PC_5);
DigitalOut kick1_2(PB_14);
DigitalOut kick2(PB_12);
DigitalOut kick2_2(PC_4);
DigitalOut myled(LED1);
InterruptIn button(USER_BUTTON);

/******************* Function *************************/
void Push();
void Pull();
void bno_init();
void set_offset();
void get_angle();
void try_motor(int phasedata_, int flagdata_);
void kickandhold();
void serial_interrupt();
void waittime_ms(int t);

int replyflag = 1;         // publish (0:mode-task, 1:finished)
bool modezeroflag = false; // true: mode-task has already done
/******************************************************/

// subscribe_int: [flag, mode, try_motor]
// publish_float: [bno]
// publish_int: [replyflag]

int main()
{
  button.fall(Push);
  button.rise(Pull);
  bno_init();
  pwm_try.period_ms(TRY_MOTOR_PERIOD);
  Ms.int_attach(serial_interrupt);

  while (1)
  {
    if (Ms.getint[1] == 0) // mode = others
    {
      modezeroflag = true;
    }
    else if (modezeroflag)
    {
      // mode-task
      modezeroflag = false;
      replyflag = 0;
      waittime_ms(500);
      switch (Ms.getint[1]) // mode
      {
      case 1:
      case 2:
        break;
      case 3: // kick
        kickandhold();
        break;
      default:
        break;
      }
      replyflag = 1;
    }
    waittime_ms(30);
  }
}

void Push()
{
  set_offset();
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
  get_angle();
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
  angle_raw = -bno.euler.yaw * PI / 180.0f;
  //angle_raw += 0.01;////////////////////
  if (angle_raw + PI * (float)(n_pi - 1) > bno_old)
    n_pi -= 2;
  else if (angle_raw + PI * (float)(n_pi + 1) < bno_old)
    n_pi += 2;
  bno_old = angle_raw + PI * (float)n_pi;
  bno_angle = bno_old - bno_offset;
}

void try_motor(int phasedata_, int flagdata_)
{
  float duty_ = 0;
  if (flagdata_ == 1 || phasedata_ == 0) // stop or try_motor=0
  {
    phase_try = 0;
    pwm_try.pulsewidth_ms(0);
  }
  else if (phasedata_ == 1)
  {
    phase_try = 1;
    pwm_try.pulsewidth_ms(TRY_MOTOR_DUTY * (float)TRY_MOTOR_PERIOD);
  }
  else if (phasedata_ == -1)
  {
    phase_try = 0;
    pwm_try.pulsewidth_ms(TRY_MOTOR_DUTY * (float)TRY_MOTOR_PERIOD);
  }
}

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

void serial_interrupt()
{
  //get_angle();
  // send replyflag and bno_angle
  static bool writeflag = true;
  if (writeflag)
    Ms.float_write(&bno_angle, 1);
  else
    Ms.int_write(&replyflag, 1);
  writeflag = !writeflag;
}

void waittime_ms(int t)
{
  int t_count = 0;
  while (t_count < t)
  {
    try_motor(Ms.getint[2], Ms.getint[0]);  // try_motor
    if (Ms.getint[0] != 1) //stop
    {
      t_count += 10;
      if (Ms.getint[0] == 2) //reset
        set_offset();
    }
    get_angle();
    wait_ms(10);
  }
}