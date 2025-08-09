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

void hotspot_on(String ssid, String password)
{
    if (!AP_FLAG)
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ssid.c_str(), password.c_str());
        Serial.println(WiFi.softAPIP());
        AP_FLAG = true;
    }
}

void hotspot_off()
{
    if (AP_FLAG)
    {

        WiFi.mode(WIFI_OFF);
        delay(800);

        WiFi.mode(WIFI_STA);
        AP_FLAG = false;
    }
}