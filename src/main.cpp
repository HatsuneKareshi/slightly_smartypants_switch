#include <mqtt.h>
#include <wf.h>

String ssid = "j4_big_brotha";
String password = "wokfuckboi";
String host = "broker.hivemq.com";
int port = 1883;

mqttbroker brokeass(host, port);

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
  brokeass.publish("MSSV/temperature", "TIEU");
  delay(1000);
}