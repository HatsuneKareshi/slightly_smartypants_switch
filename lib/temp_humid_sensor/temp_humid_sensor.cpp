#include "temp_humid_sensor.h"

temp_humid_sensor::temp_humid_sensor(uint8_t pin)
{
    pinMode(pin, INPUT);
    this->sense_pin = pin;
    this->sense_obj_ptr = new DHT(pin, DHT11);
    this->sense_obj_ptr->begin();
};

temp_humid_sensor::~temp_humid_sensor()
{
    delete this->sense_obj_ptr;
}

void temp_humid_sensor::toggle_fahrenheit()
{
    this->freedom_degrees = !this->freedom_degrees;
}

float temp_humid_sensor::get_temp()
{
    return this->sense_obj_ptr->readTemperature(this->freedom_degrees);
}

float temp_humid_sensor::get_humidity()
{
    return this->sense_obj_ptr->readHumidity();
}