#include <WiFi.h>
#include <WebServer.h>

static bool AP_FLAG = false;

void wifi_connect(const char *ssid, const char *password);
bool is_connected();
void wifiscan();
void hotspot_on(String ssid = "smartypants_wifi_form", String password = "");
void hotspot_off();