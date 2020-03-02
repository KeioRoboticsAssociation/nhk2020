// main.cpp

#include "common.h"

Serial pc(USBTX, USBRX, 115200);
BOARDC_BNO055 bno(PB_7, PB_8); // SDA,SCL
Mbedserial Ms(pc);
PwmOut pwm_try1(PA_8);
PwmOut pwm_try2(PA_0);
RDS5160Servo servo1(pwm_try1, 1500, true);  // PWM:500us-2500us, 270 degree
RDS5160Servo servo2(pwm_try2, 1500, false);
DigitalOut kick1(PC_5);
DigitalOut kick1_2(PB_14);
DigitalOut kick2(PB_12);
DigitalOut kick2_2(PC_4);
DigitalOut myled(LED1);
InterruptIn button(USER_BUTTON);

/******************* Function *************************/
void Push();
void Pull();
void serial_interrupt();

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
  Ms.int_attach(serial_interrupt);
  try_motor(TRY_MOTOR_ANGLE1, 0);

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
      case 1: // try
        try_motor(TRY_MOTOR_ANGLE2, 500);
        waittime_ms(1000);
        try_motor(TRY_MOTOR_ANGLE1, 500);
        break;
      case 2: // kick
        kickandhold();
        break;
      case 3:
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

void serial_interrupt()
{
  // send replyflag and bno_angle
  static bool writeflag = true;
  if (writeflag)
    Ms.float_write(&bno_angle, 1);
  else
    Ms.int_write(&replyflag, 1);
  writeflag = !writeflag;
  bno.recover(RECOVER_TIMEOUT);
  //bno.recover();
}

void waittime_ms(int t)
{
  int t_count = 0;
  while (t_count < t)
  {
    if (Ms.getint[0] != 1) //stop
    {
      t_count += 20;
      if (Ms.getint[0] == 2) //reset
        set_offset();
    }
    get_angle();
    wait_us(20000);
  }
}