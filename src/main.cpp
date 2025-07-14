#include <Arduino.h>
#include <rtc.h>
#include <light_sensor.h>
#include <temp_humid_sensor.h>
#include <relay.h>

// rtc cl(13, 14, 15);
relay r(13);
void setup()
{
  Serial.begin(115200);
  // cl.set_running(true);
}

int dl = 3000;

void loop()
{
  // RtcDateTime now = cl.get_time();
  // String response = "\t" + String(now.Hour()) + "\t" + String(now.Minute()) + "\t" + String(now.Second()) + "\n";
  // Serial.print(response);

  r.toggle();
  String a = "closed? " + String((r.get_state_NO()) ? "closed" : "open");
  Serial.println(a);
  delay(500);
}