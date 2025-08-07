#include "mqtt.h"

void callback(char *topic, byte *message, unsigned int length)
{
    Serial.println(String("topic is: ") + String(topic));
    String msg;
    for (int i = 0; i < length; i++)
    {
        msg += (char)message[i];
    }
    Serial.println(String("msg is: ") + String(msg));

    STAT_BUFF = String(msg);
    STAT_TOPC = String(topic);
    NEW_MSG = true;

    //***Code here to process the received package***
}

mqttbroker::mqttbroker(String mqtt_host, int port)
{
    this->wfc_ptr = new WiFiClient;
    this->psc_ptr = new PubSubClient(*wfc_ptr);
    this->hostname = mqtt_host;
    this->port = port;
    this->psc_ptr->setServer(this->hostname.c_str(), this->port);
    this->psc_ptr->setCallback(callback);
    this->psc_ptr->setKeepAlive(90);
}

mqttbroker::~mqttbroker() //  this never gets called but eh whatevers
{
    delete this->psc_ptr;
    delete this->wfc_ptr;
}

void mqttbroker::connect()
{
    Serial.println("Attemping MQTT connection...");
    while (!this->psc_ptr->connected())
    {
        String clientId = "ESP32Client-" + String(random(0xffff), HEX);
        if (this->psc_ptr->connect(clientId.c_str()))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print(this->psc_ptr->state());
            Serial.print(", trying again in 2 seconds");
            delay(2000);
        }
    }
}

bool mqttbroker::is_broker_connected()
{
    return this->psc_ptr->connected();
}

void mqttbroker::subscribe(String topic)
{
    this->psc_ptr->subscribe(topic.c_str());
}

void mqttbroker::publish(String topic, String buffer)
{
    this->psc_ptr->publish(topic.c_str(), buffer.c_str());
}

void mqttbroker::loop()
{
    NEW_MSG = false;
    this->psc_ptr->loop();
}

String mqttbroker::get_message(String topic)
{

    if (topic == STAT_TOPC && NEW_MSG)
        return STAT_BUFF;
    return String("");
}
