#ifndef __PICKUP_H__
#define __PICKUP_H__

#include <mbed.h>
#include "RDS5160_Servo.h"

class PickUp
{
private:
    RDS5160Servo &LEFT_SERVO, &RIGHT_SERVO;
    
public:
    PickUp(RDS5160Servo &, RDS5160Servo &);

    void Horizontal();
    void Vertical();

    void MoveTo(float degree);
};

#endif