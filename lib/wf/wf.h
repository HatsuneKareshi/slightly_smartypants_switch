#include <WiFi.h>
#include <WebServer.h>

static bool AP_FLAG = false;
static bool FORM_FLAG = false;
static WebServer wififormserver(80);

static String PSSW;
static String SSID;

void wifi_connect(const char *ssid, const char *password);
bool is_connected();
void wifiscan();
void hotspot_on(String ssid = "smartypants_wifi_form", String password = "");
void hotspot_off();

void form_host_init();    // to init the server
void form_on();           // to turn it on when disconnected from wifi
void form_off();          // to turn it off when connected to wifi
void form_handleClient(); // needs to be called constantly

// the function declared below is the wifi connect setup handler

void init_wifi_fetch();