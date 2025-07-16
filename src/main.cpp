#include <Arduino.h>
#include <rtc.h>
#include <light_sensor.h>
#include <temp_humid_sensor.h>
#include <relay.h>
#include <motion_sensor.h>

relay r(13);
motion_sensor mt(14);
void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if (mt.get_motion())
    r.turn_on();
  else
    r.turn_off();
}