#include <Arduino.h>

class light_sensor
{
private:
    uint8_t dpin;
    uint8_t apin;

public:
    light_sensor(uint8_t d, uint8_t a);
    int get_light_digital();
    int get_light_analog();
};