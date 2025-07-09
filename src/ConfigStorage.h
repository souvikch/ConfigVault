#pragma once
#include <Arduino.h>

class ConfigStorageClass {
public:
  void begin(const String& nsName);
  bool load(String& outJson);
  bool save(const String& json);
  bool clear();
  bool exists();

private:
  String _nsName;
};

extern ConfigStorageClass ConfigStorage;
