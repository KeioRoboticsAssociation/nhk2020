#ifndef SERVO_H
#define SERVO_H

#include <mbed.h>

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

#endif /* SERVO_H */
