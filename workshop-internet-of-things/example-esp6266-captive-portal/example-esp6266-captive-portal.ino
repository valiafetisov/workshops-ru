/*
   Code based on captive portal code by: M. Ray Burnette 20150831
   https://www.hackster.io/rayburne/esp8266-captive-portal-5798ff
*/

#include <ESP8266WiFi.h>
#include "./DNSServer.h"                  // Patched lib
#include <ESP8266WebServer.h>

const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(10, 10, 10, 1);    // Private network for server
DNSServer         dnsServer;              // Create the DNS object
ESP8266WebServer  webServer(80);          // HTTP server
int               ledPin = 16;
boolean           isOn = true;

String responseHTML = ""
  "<!DOCTYPE html>\n"
  "<html><head>\n"
  "<title>control me</title>\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
  "<style type='text/css'>\n"
  " body {\n"
  "   background-color: red;\n"
  "   height: 100%;\n"
  " }\n"
  " .btn {\n"
  "   background-color: black;\n"
  "   color: white;\n"
  "   cursor: pointer;\n"
  "   padding: 5px 10px;\n"
  "   font-size: 40px;\n"
  "   position: absolute;\n"
  "   top: 50%;\n"
  "   left: 50%;\n"
  "   transform: translate(-50%, -50%);\n"
  "   transition: all 200ms ease;\n"
  "   user-select: none;\n"
  " }\n"
  " .btn:hover {\n"
  "   background-color: white;\n"
  "   color: black;\n"
  " }\n"
  "</style>\n"
  "</head>\n"
  "<body>\n"
  "<div class='btn' onclick='toggleLED()'>click me</div>\n"
  "<script type='text/javascript'>\n"
  " function toggleLED () {\n"
  "   var xmlHttp = new XMLHttpRequest();\n"
  "   xmlHttp.open('GET', '/toggle', false);\n"
  "   xmlHttp.send(null);\n"
  "   // alert(xmlHttp.responseText);\n"
  " }\n"
  "</script>\n"
  "</body></html>";

void setup() {
  pinMode(ledPin, OUTPUT);

  // создаем точку доступа
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Testing Testing 1, 2, 3");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // при открытии адреса /toggle,
  // переменная isOn поменяет свое состояние на противоположное
  webServer.on("/toggle", []() {
    webServer.send(200, "text/html", "ok");
    isOn = !isOn;
  });

  // для всех остальных запросов, возвращаем один и тот же HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();

  // ledPin становится HIGH или LOW
  // в зависимости от isOn
  digitalWrite(ledPin, isOn);
}
