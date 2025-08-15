#include <mqtt.h>
#include <wf.h>
#include <ArduinoJson.h>
#include <temp_humid_sensor.h>
#include <rtc.h>
#include <light_sensor.h>
#include <motion_sensor.h>
#include <cmath>
#include <relay.h>

String host = "broker.hivemq.com";
int port = 1883;

// Initialize sensors
mqttbroker brokeass(host, port);
temp_humid_sensor DHTSensor(4);
light_sensor lightSensor(32, 33);
motion_sensor motionSensor(5);
relay rl(14);

// Init time
unsigned long lastPublish = 0;

void setup()
{
  Serial.begin(115200);
  form_host_init();
}

bool IS_ON_MANUAL;
bool MANUAL_RELAY_STATE;
int UP_TEMP;
int UP_HMDT;
int UP_LGHT;
int LR_TEMP;
int LR_HMDT;
int LR_LGHT;
bool MOTION_TRIGGER;

bool RELAY_CHANGE_TRACKER = false;

void loop()
{
  if (!is_connected()) // if not connected, turn on the hotspot, host the site, handle the inputs and connect
  {
    // hotspot_on();
    // form_on();
    // form_handleClient();
    init_wifi_fetch();
  }
  else // if already connectd, turn off the site and hotspot, do the needful
  {
    // form_off();
    // hotspot_off();

    if (!brokeass.is_broker_connected())
    {
      brokeass.connect();
      brokeass.subscribe("smartypantsswitch/to_esp");
    }
    brokeass.loop();

    // incoming command is handled here

    Serial.println(); // handle incoming payload here
    StaticJsonDocument<500> command;
    String msg = brokeass.get_message("smartypantsswitch/to_esp").c_str();
    Serial.println(msg);
    if (msg != "")
    {
      deserializeJson(command, msg.c_str());
      IS_ON_MANUAL = command["manual"];
      MANUAL_RELAY_STATE = command["relay"];
      UP_HMDT = command["upper_humidity"];
      UP_LGHT = command["upper_light"];
      UP_TEMP = command["upper_temp"];
      LR_HMDT = command["lower_humidity"];
      LR_LGHT = command["lower_light"];
      LR_TEMP = command["lower_temp"];
      MOTION_TRIGGER = command["motion_trigger"];
    }

    if (IS_ON_MANUAL)
    {
      Serial.println("on manual, relay sig " + String(MANUAL_RELAY_STATE));
      if (MANUAL_RELAY_STATE)
      {
        rl.turn_on();
      }
      else
      {
        rl.turn_off();
      }
      // lastPublish = 10000;
    }
    else
    {
      // bool cond_bool = LR_HMDT <= DHTSensor.get_humidity() && DHTSensor.get_humidity() <= UP_HMDT &&
      //                  LR_LGHT <= lightSensor.get_light_analog() && lightSensor.get_light_analog() <= UP_LGHT &&
      //                  LR_TEMP <= DHTSensor.get_temp() && DHTSensor.get_temp() <= UP_TEMP;
      //  MOTION_TRIGGER == motionSensor.get_motion();
      bool hmdt, lght, tmp, motn;
      hmdt = LR_HMDT <= DHTSensor.get_humidity() && DHTSensor.get_humidity() <= UP_HMDT;
      lght = LR_LGHT <= lightSensor.get_light_analog() && lightSensor.get_light_analog() <= UP_LGHT;
      tmp = LR_TEMP <= DHTSensor.get_temp() && DHTSensor.get_temp() <= UP_TEMP;
      motn = MOTION_TRIGGER == motionSensor.get_motion();

      Serial.println(String("humidity check: ") + String(hmdt) + String(" sensing: ") + String(DHTSensor.get_humidity()));
      Serial.println(String("light check: ") + String(lght) + String(" sensing: ") + String(lightSensor.get_light_analog()));
      Serial.println(String("temperature check: ") + String(tmp) + String(" sensing: ") + String(DHTSensor.get_temp()));
      Serial.println(String("motion check: ") + String(motn) + String(" sensing: ") + String(motionSensor.get_motion()));

      bool cond_bool = hmdt && lght && tmp && motn;

      Serial.println("on auto, relay sig " + String(cond_bool));
      if (cond_bool)
      {
        rl.turn_on();
      }
      else
      {
        rl.turn_off();
      }
      // lastPublish = 10000;
    }

    // Create a JSON document
    if (millis() - lastPublish >= 10000 || rl.get_changed()) // if more than 10s or relay just changed
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
      doc["relay"] = rl.get_state_NO();
      String jsonString;
      serializeJson(doc, jsonString);

      Serial.println(String("msg to server is: |") + jsonString + String("|"));

      brokeass.publish("smartypantsswitch/to_web", jsonString.c_str());
      lastPublish = millis();
    }
  }
  delay(1000);
}