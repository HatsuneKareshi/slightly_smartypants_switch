#include <PubSubClient.h>
#include <WiFi.h>
#include <Arduino.h>

static String STAT_BUFF;
static String STAT_TOPC;
static bool NEW_MSG;

class mqttbroker
{
private:
    WiFiClient *wfc_ptr = 0x0;
    PubSubClient *psc_ptr = 0x0;
    String hostname;
    int port;

public:
    mqttbroker(String mqtt_host, int port);
    ~mqttbroker();
    void connect();
    bool is_broker_connected();
    void subscribe(String topic);
    void publish(String topic, String buffer);
    void loop();
    String get_message(String topic);
};