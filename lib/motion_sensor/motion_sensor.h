#include <Arduino.h>

class motion_sensor
{
private:
    uint8_t sense_pin;

public:
    motion_sensor(uint8_t pin);
    bool get_motion();
};