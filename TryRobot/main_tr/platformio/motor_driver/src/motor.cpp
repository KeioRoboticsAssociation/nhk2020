#include "motor.h"

Motor::Motor(PinName EN_GD, PinName PWM_p, PinName PWM_n) : GATE_DRIVER_ENABLE(EN_GD), PWM_P(PWM_p), PWM_N(PWM_n)
{
    GATE_DRIVER_ENABLE.write(DISABLE);
    PWM_PERIOD = 50U; // Default frequency is 20kHz...
    PWM_PULSEWIDTH = 0U;
    PWM_P.period_us(PWM_PERIOD);
    PWM_N.period_us(PWM_PERIOD);
    PWM_P.pulsewidth_us(0U);
    PWM_N.pulsewidth_us(0U);
}

mbed_error_status_t Motor::setPWMPeriod(uint8_t value)
{
    if (value <= 100U && value >= 10U) // 10kHz to 100kHz
    {
        PWM_PERIOD = value;
        PWM_P.period_us(PWM_PERIOD);
        PWM_N.period_us(PWM_PERIOD);
        return MBED_SUCCESS;
    }
    else
    {
        PWM_PERIOD = 50U; // 20kHz
        PWM_P.period_us(PWM_PERIOD);
        PWM_N.period_us(PWM_PERIOD);
        MBED_ERROR(MBED_ERROR_INVALID_ARGUMENT, "PWM period must be specified within 10 to 100 us");
        return MBED_ERROR_INVALID_ARGUMENT;
    }
}

mbed_error_status_t Motor::setPWMPulsewidth(uint8_t value)
{
    if (value <= PWM_PERIOD * 0.9F && value > 0U) // Limit Duty <= 90%
    {
        PWM_PULSEWIDTH = value;
        return MBED_SUCCESS;
    }
    else
    {
        PWM_PULSEWIDTH = PWM_PERIOD * 0.2F; // Duty = 20%
        MBED_ERROR(MBED_ERROR_INVALID_ARGUMENT, "PWM duty must be under 90%");
        return MBED_ERROR_INVALID_ARGUMENT;
    }
}

mbed_error_status_t Motor::enableGateDriver(void)
{
    if (GATE_DRIVER_ENABLE.read() == RESET)
    {
        GATE_DRIVER_ENABLE.write(ENABLE);
        return MBED_SUCCESS;
    }
    else
    {
        MBED_ERROR(MBED_ERROR_ALREADY_INITIALIZED, "Gate Driver is already enabled");
        return MBED_ERROR_ALREADY_INITIALIZED;
    }
}

mbed_error_status_t Motor::disableGateDriver(void)
{
    if (GATE_DRIVER_ENABLE.read() == SET)
    {
        GATE_DRIVER_ENABLE.write(DISABLE);
        return MBED_SUCCESS;
    }
    else
    {
        MBED_ERROR(MBED_ERROR_ALREADY_INITIALIZED, "Gate Driver is already disabled");
        return MBED_ERROR_ALREADY_INITIALIZED;
    }
}

void Motor::Brake(void)
{
    if (GATE_DRIVER_ENABLE.read() == SET)
    {
        PWM_P.pulsewidth_us(0U);
        PWM_N.pulsewidth_us(0U);
    }
}

mbed_error_status_t Motor::CW(void)
{
    if (GATE_DRIVER_ENABLE.read() == SET)
    {
        PWM_P.pulsewidth_us(PWM_PULSEWIDTH);
        PWM_N.pulsewidth_us(0U);
        return MBED_SUCCESS;
    }
    else
    {
        MBED_ERROR(MBED_ERROR_OPERATION_PROHIBITED, "Enable Gate Driver first");
        return MBED_ERROR_OPERATION_PROHIBITED;
    }
}

mbed_error_status_t Motor::CCW(void)
{
    if (GATE_DRIVER_ENABLE.read() == SET)
    {
        PWM_P.pulsewidth_us(0U);
        PWM_N.pulsewidth_us(PWM_PULSEWIDTH);
        return MBED_SUCCESS;
    }
    else
    {
        MBED_ERROR(MBED_ERROR_OPERATION_PROHIBITED, "Enable Gate Driver first");
        return MBED_ERROR_OPERATION_PROHIBITED;
    }
}

void Motor::setSpeed(float_t value) // float -1 to 1
{
    if (value >= 0.02F && value < 0.9F)
    {
        setPWMPulsewidth(PWM_PERIOD * value);
        CW();
    }
    else if (value > -0.9F && value <= -0.02F)
    {
        setPWMPulsewidth(PWM_PERIOD * -1.0F * value);
        CCW();
    }
    else
    {
        Brake();
    }
}
