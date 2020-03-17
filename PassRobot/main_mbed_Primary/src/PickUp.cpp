#include "PickUp.h"
#include "RDS5160_Servo.h"
#include "mbed.h"

PickUp::PickUp(RDS5160Servo &Left, RDS5160Servo &Right) : LEFT_SERVO(Left), RIGHT_SERVO(Right)
{

}

void PickUp::Horizontal(void) {
    LEFT_SERVO.Neutral();
    RIGHT_SERVO.Neutral();
}

void PickUp::Vertical(void) {
    LEFT_SERVO.MoveTo(90.0f);
    RIGHT_SERVO.MoveTo(90.0f);
}

void PickUp::MoveTo(float degree) {
    LEFT_SERVO.MoveTo(degree);
    RIGHT_SERVO.MoveTo(degree);
}