#include <mqtt.h>
#include <wf.h>
#include <ArduinoJson.h>
#include <temp_humid_sensor.h>
#include <rtc.h>
#include <light_sensor.h>
#include <motion_sensor.h>
#include <cmath>

// String ssid = "j4_big_brotha";
// String password = "wokfuckboi";
String host = "broker.hivemq.com";
int port = 1883;

// Initialize sensors
mqttbroker brokeass(host, port);
temp_humid_sensor DHTSensor(4);
light_sensor lightSensor(32, 33);
motion_sensor motionSensor(5);

// Init time
unsigned long lastPublish = 0;

void setup()
{
  Serial.begin(115200);
  form_host_init();
}

void loop()
{
  if (!is_connected()) // if not connected, turn on the hotspot, host the site, handle the inputs and connect
  {
    hotspot_on();
    form_on();
    form_handleClient();
  }
  else // if already connectd, turn off the site and hotspot, do the needful
  {
    form_off();
    hotspot_off();

    if (!brokeass.is_broker_connected())
    {
      brokeass.connect();
      brokeass.subscribe("/broken/ass");
    }
    brokeass.loop();
    Serial.println(brokeass.get_message("/broken/ass"));
    // Create a JSON document
    if (millis() - lastPublish >= 10000)
    {
      StaticJsonDocument<200> doc;
      float temp = DHTSensor.get_temp();
      float humidity = DHTSensor.get_humidity();
      if (!isnan(temp))
        doc["temperature"] = temp;
      else
        doc["temperature"] = 0;

      if (!isnan(humidity))
        doc["humidity"] = humidity;
      else
        doc["humidity"] = 0;

      doc["light"] = lightSensor.get_light_analog();
      doc["motion"] = motionSensor.get_motion();

      String jsonString;
      serializeJson(doc, jsonString);

      brokeass.publish("MSSV/temperature", jsonString.c_str());
      lastPublish = millis();
    }
  }
  delay(1000);
}