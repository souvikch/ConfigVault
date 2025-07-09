#pragma once
#include <Arduino.h>
#include <Preferences.h>

class PreferencesStorage {
public:
  void begin(const String& nsName);
  bool load(String& outJson);
  bool save(const String& json);
  bool clear();
  bool exists();

private:
  String _nsName;
  const char* _key = "config";
};
