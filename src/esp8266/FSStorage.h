#pragma once
#include <Arduino.h>
#include <LittleFS.h>  // Assumes LittleFS is used. Fallback to SPIFFS can be added if needed.

class FSStorage {
public:
  void begin(const String& nsName);
  bool load(String& outJson);
  bool save(const String& json);
  bool clear();
  bool exists();

private:
  String _filename = "/config.json";
};
