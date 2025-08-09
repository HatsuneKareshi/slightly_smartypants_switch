#include <mqtt.h>
#include <wf.h>
#include <ArduinoJson.h>
#include <temp_humid_sensor.h>
#include <rtc.h>
#include <light_sensor.h>
#include <motion_sensor.h>

String ssid = "j4_big_brotha";
String password = "wokfuckboi";
String host = "broker.hivemq.com";
int port = 1883;

// Initialize sensors
mqttbroker brokeass(host, port);
temp_humid_sensor DHTSensor;
light_sensor lightSensor;
motion_sensor motionSensor;

// Init time
unsigned long lastPublish = 0;

void setup()
{
  Serial.begin(115200);
  wifiscan();
  wifi_connect(ssid.c_str(), password.c_str());
}

void loop()
{
  if (!is_connected())
    wifi_connect(ssid.c_str(), password.c_str());
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
    doc["temperature"] = DHTSensor.get_temp();
    doc["humidity"] = DHTSensor.get_humidity();
    doc["light"] = lightSensor.get_light_analog();
    doc["motion"] = motionSensor.get_motion();

    String jsonString;
    serializeJson(doc, jsonString);

    brokeass.publish("MSSV/temperature", jsonString.c_str());
    lastPublish = millis();
  }
  delay(1000);
}