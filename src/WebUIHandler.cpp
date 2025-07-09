// WebUIHandler.cpp (blocking version with AP mode and handleClient loop)
#include "WebUIHandler.h"

#if defined(ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
  WebServer server(80);
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server(80);
#endif

#include <ArduinoJson.h>
#include "ConfigStorage.h"

namespace WebUIHandler {

void start(const String& deviceName, std::function<void(const String&)> onSubmit) {
  WiFi.mode(WIFI_AP);

  IPAddress apIP(192, 168, 10, 1);
  IPAddress netmask(255, 255, 255, 0);
  WiFi.softAPConfig(apIP, apIP, netmask);
  WiFi.softAP(deviceName.c_str());

  IPAddress ip = WiFi.softAPIP();
  Serial.print("[WebUI] Connect to WiFi AP: ");
  Serial.println(deviceName);
  Serial.print("[WebUI] Visit: http://");
  Serial.println(ip);

  server.on("/", HTTP_GET, [deviceName]() {
    Serial.println("[WebUI] Received GET /");
    String html = "<!DOCTYPE html><html><head><title>" + deviceName +
                  " Configuration</title></head><body>"
                  "<h1>Configure " + deviceName + "</h1>"
                  "<form method='POST' action='/save'>"
                  "SSID: <input name='b0'><br/>"
                  "PASS: <input name='b1'><br/>"
                  "Device ID: <input name='b2'><br/>"
                  "Threshold: <input name='b3'><br/>"
                  "<input type='submit' value='Save'>"
                  "</form></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/save", HTTP_POST, [onSubmit]() {
    DynamicJsonDocument doc(512);
    for (uint8_t i = 0; i < server.args(); ++i) {
      doc[server.argName(i)] = server.arg(i);
    }
    String json;
    serializeJson(doc, json);
    onSubmit(json);
    server.send(200, "text/html", "<h2>Saved. Rebooting...</h2>");
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}

} // namespace WebUIHandler
