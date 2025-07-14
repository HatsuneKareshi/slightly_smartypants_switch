#include "light_sensor.h"

// 1 means light detected
// higher numbers mean brigher

light_sensor::light_sensor(uint8_t d, uint8_t a)
{
    this->apin = a;
    this->dpin = d;
    pinMode(a, INPUT);
    pinMode(d, INPUT);
}

int light_sensor::get_light_analog()
{
    return 1023 - analogRead(this->apin);
}

int light_sensor::get_light_digital()
{
    return !digitalRead(this->apin);
}