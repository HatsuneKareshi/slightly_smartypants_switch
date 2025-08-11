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
    String frm_html = "<!DOCTYPE html> <html> <style>     body {         background-color: grey;     }      h1 {         color: black;     }      p {         color: white;     }      div {         width: auto;         margin: auto;     } </style>  <head>     <title>configure wifi for the smartass switch</title> </head>  <body>     <div>         <h1>Configure wifi connection for your smartypants switch</h1>         <form method='POST' action='/connect'>             <label for='ssid'>SSID:</label><br>             <input type='text' id='ssid' name='ssid' value=''><br>             <label for='pssw'>PASSWORD:</label><br>             <input type='text' id='pssw' name='pssw' value=''><br><br>             <input type='submit' value='Submit'>         </form>     </div>     <hr width= 'auto'>     <div>         <h1>Networks the ESP32 can connect to:</h1>";
    String end_html = "    </div></body></html>";
    String ssid_html_list = "<p>";

    int ssid_cnt = WiFi.scanNetworks();
    for (int i = 0; i < ssid_cnt; i++)
    {
        ssid_html_list += WiFi.SSID(i) + "<br>";
    }
    ssid_html_list += "</p>";

    // wififormserver.send(200, "text/html",
    //                     "<form method='POST' action='/connect'>"
    //                     "<label for=' fname'>SSID:</label><br>"
    //                     "<input type='text' id='ssid' name='ssid' value=''><br>"
    //                     "<label for='lname'>PASSWORD:</label><br>"
    //                     "<input type='text' id='pssw' name='pssw' value=''><br><br>"
    //                     "<input type='submit' value='Submit'>"
    //                     "</form>");
    wififormserver.send(200, "text/html", (frm_html + ssid_html_list + end_html).c_str());
}

void submit_action()
{
    String SSID = wififormserver.arg("ssid");
    String PSSW = wififormserver.arg("pssw");
    Serial.printf("Received SSID: |%s|, Password: |%s|\n", SSID.c_str(), PSSW.c_str());
    // wififormserver.send(200, "text/html",
    //                     "<p> connected!!! </p>");
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

void init_wifi_fetch()
{
    hotspot_on();
    form_on();
    while (1)
    {
        hotspot_on();
        form_handleClient();
        delay(500);
        if (is_connected())
        {
            delay(2000);
            form_off();
            hotspot_off();
            break;
        }
    }
}