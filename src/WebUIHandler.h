#pragma once
#include <Arduino.h>

namespace WebUIHandler {
  void start(const String& deviceName, std::function<void(const String&)> onSubmit);
  void loop();  // Now declared correctly
}
