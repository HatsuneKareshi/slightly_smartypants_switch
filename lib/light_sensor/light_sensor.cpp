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
    return 4095 - analogRead(this->apin); // apparently on the esp32 this goes up to 12 bits
}

int light_sensor::get_light_digital()
{
    return !digitalRead(this->apin);
}