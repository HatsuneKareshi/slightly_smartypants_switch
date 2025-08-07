#include "wf.h"

void wifi_connect(const char *ssid, const char *password)
{
    Serial.println(ssid);
    Serial.println(password);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println(WiFi.status());
    }
}

bool is_connected()
{
    return WiFi.status() == WL_CONNECTED;
}

void wifiscan()
{
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
        Serial.println(WiFi.SSID(i));
}
