#include "RDS5160_Servo.h"
#include "mbed.h"
#include <math.h>

RDS5160Servo::RDS5160Servo(PwmOut &PWM_Pin, uint32_t neutral, bool direction)
    : PWM(PWM_Pin), NEUTRAL(neutral), DIRECTION(direction) {
    PWM.period_us(2500);
}

void RDS5160Servo::Neutral(void) {
    PWM.pulsewidth_us(NEUTRAL);
}

void RDS5160Servo::MoveTo(float degree) { //やりかたよく分からない
    int32_t pulsewidth = (int32_t)((degree * 7.41f) * (DIRECTION ? 1 : -1) + NEUTRAL);
    PWM.pulsewidth_us(abs(pulsewidth));
}
