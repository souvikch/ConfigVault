#include <Arduino.h>
#include "ConfigVault.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Erasing config storage...");
  uint8_t result = CVault.formatFlash();

  if (result == 1) {
    Serial.println("Flash formatted successfully.");
  } else {
    Serial.println("Flash format failed.");
  }
}

void loop() {
  // Nothing to do here
}
