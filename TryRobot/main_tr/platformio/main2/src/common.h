// common.h

#ifndef COMMON_H
#define COMMON_H

#include <mbed.h>
#include "BOARDC_BNO055.h"
#include "mbedserial.h"
//#include <math.h>

/*********************** Param *************************/
#define PI 3.141592f
#define TRY_MOTOR_PERIOD 50 // (ms)
#define TRY_MOTOR_DUTY 0.3f
#define KICK_WAIT_TIME_MS 40 // (ms)
#define RECOVER_TIMEOUT 100  // (ms)
#define TRY_MOTOR_ANGLE1 -90 // (degree)
#define TRY_MOTOR_ANGLE2 70  // (degree)

/******************** Class ***************************/

// servo PWM:500us-2500us, 270 degree
class RDS5160Servo
{
private:
    PwmOut &PWM;       // main関数内で使うピンを指定して初期化
    //const uint32_t PWM_PERIOD;
    const uint32_t NEUTRAL;
    const bool DIRECTION; // trueのときCWと定義 モーターのつなぎ方によってfalseにする

public:
    /* Constructor */
    RDS5160Servo(PwmOut &PWM_Pin, uint32_t neutral, bool direction);

    void Neutral();

    void MoveTo(float degree);
};

/******************* Instance *************************/

extern Serial pc;
extern BOARDC_BNO055 bno; // SDA,SCL
extern RDS5160Servo servo1;
extern RDS5160Servo servo2;
extern DigitalOut kick1;
extern DigitalOut kick1_2;
extern DigitalOut kick2;
extern DigitalOut kick2_2;
extern DigitalOut myled;

extern float bno_angle;  // old - offset (rad)

/******************* Function *************************/

void bno_init();
void set_offset();
void get_angle();
void try_motor(float degree, int time_ms);
void kickandhold();
void waittime_ms(int t);

#endif