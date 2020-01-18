/* NHK学生ロボコン2020用モータードライバ
 * Motor A: ENABLE == PB_5, PWM_P == PA_10, PWM_N == PA11_ALT_0
 * MOtor B: ENABLE == PB_4, PWM_P == PB_0_ALT_0, PWM_N == PB_1_ALT_0
 * デフォルトのPWM周波数は20kHz
 * インダクタンスの小さいコアレスモータなどは周波数を高くする必要がある
 * デューティ比は高くしすぎるとハイサイドFETがスイッチングできないので90％以下に制限している
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <mbed.h>

class Motor
{
private:
    DigitalOut GATE_DRIVER_ENABLE; // Output HIGH to enable gate driver
    PwmOut PWM_P;                  // Positive side PWM input
    PwmOut PWM_N;                  // Negative side PWM input

    uint8_t PWM_PERIOD;
    uint8_t PWM_PULSEWIDTH;

public:
    /* Constructor */
    Motor(PinName EN_GD, PinName PWM_p, PinName PWM_n); // ENABLE, PWM_P, PWM_N

    /* Setter */
    mbed_error_status_t setPWMPeriod(uint8_t value);
    mbed_error_status_t setPWMPulsewidth(uint8_t value);

    /* Getter */
    uint8_t getPWMPeriod(void) { return PWM_PERIOD; };
    uint8_t getPWMPulsewidth(void) { return PWM_PULSEWIDTH; };

    mbed_error_status_t enableGateDriver(void);
    mbed_error_status_t disableGateDriver(void);

    void Brake(void);
    mbed_error_status_t CW(void);  // Clockwise
    mbed_error_status_t CCW(void); // Counterclockwise

    void setSpeed(float_t value);
};

#endif /* MOTOR_H */
