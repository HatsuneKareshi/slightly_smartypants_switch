#include <Arduino.h>
#include <DHT.h>

class temp_humid_sensor
{
private:
    uint8_t sense_pin;
    bool freedom_degrees = false;
    DHT *sense_obj_ptr = 0x0;

public:
    temp_humid_sensor(uint8_t pin);
    ~temp_humid_sensor();
    void toggle_fahrenheit();
    float get_temp();
    float get_humidity();
};