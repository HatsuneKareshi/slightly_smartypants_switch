#include "wf.h"

void wifi_connect(const char *ssid, const char *password) // this thing will have to be slow whether you like it or not
{
    Serial.println(ssid);
    Serial.println(password);
    WiFi.mode(WIFI_STA); // this causes AP turn off
    delay(1000);
    WiFi.disconnect(true, true);
    delay(1000);
    WiFi.begin(ssid, password); // this likely turns off the AP
    delay(1000);
    AP_FLAG = false; // keeps track of AP state

    for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++)
    {
        Serial.println(WiFi.status());
        delay(100);
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
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_STA);
        AP_FLAG = false;
    }
}

void root_action()
{
    wififormserver.send(200, "text/html",
                        "<form method='POST' action='/connect'>"
                        "<label for=' fname'>SSID:</label><br>"
                        "<input type='text' id='ssid' name='ssid' value=''><br>"
                        "<label for='lname'>PASSWORD:</label><br>"
                        "<input type='text' id='pssw' name='pssw' value=''><br><br>"
                        "<input type='submit' value='Submit'>"
                        "</form>");
}

void submit_action()
{
    String SSID = wififormserver.arg("ssid");
    String PSSW = wififormserver.arg("pssw");
    Serial.printf("Received SSID: |%s|, Password: |%s|\n", SSID.c_str(), PSSW.c_str());
    wififormserver.send(200, "text/html",
                        "<p> we have your wifi credentials </p>");
    wifi_connect(SSID.c_str(), PSSW.c_str());
}

void form_host_init()
{
    wififormserver.on("/", HTTP_GET, root_action);
    wififormserver.on("/connect", HTTP_POST, submit_action);
}

void form_on()
{
    if (!FORM_FLAG)
    {
        wififormserver.begin();
        FORM_FLAG = true;
    }
}

void form_off()
{
    if (FORM_FLAG)
    {
        wififormserver.stop();
        FORM_FLAG = false;
    }
}

void form_handleClient()
{
    wififormserver.handleClient();
}