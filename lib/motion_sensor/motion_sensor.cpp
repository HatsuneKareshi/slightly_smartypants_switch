#include "motion_sensor.h"

motion_sensor::motion_sensor(uint8_t pin)
{
    pinMode(pin, INPUT);
    this->sense_pin = pin;
}

bool motion_sensor::get_motion()
{
    return digitalRead(this->sense_pin) == HIGH;
}