#include <Arduino.h>
#include "WebUIHandler.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting WebUI...");
  WebUIHandler::start("MyDevice", [](const String& json) {
    Serial.println("Received JSON config:");
    Serial.println(json);
    // simulate saving config
    delay(1000);
    ESP.restart();
  });

  Serial.println("WebUI launched for configuration...");
}

void loop() {
  WebUIHandler::loop();  // must call this for WebServer to handle requests
}
