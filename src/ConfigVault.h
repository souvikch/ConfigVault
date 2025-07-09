#pragma once
#include <Arduino.h>
#include "ConfigStorage.h"

#include "version.h"

struct ConfigVaultMeta {
  String deviceName;
  uint8_t triggerPin;
};

class ConfigVaultClass {
public:
  bool begin(const String& deviceName, uint8_t triggerPin = 255);
  template<typename T> bool loadConfig(T& configStruct);
  template<typename T> bool saveConfig(const T& configStruct);
  uint8_t formatFlash();
  bool configPresent();

private:
  bool configLoaded = false;
  String configJson;
  ConfigVaultMeta meta;

  bool shouldStartWebUI();
  void launchWebUI();

  bool serializeStruct(const void* structPtr, size_t structSize, String& outJson);
  bool deserializeStruct(const String& json, void* structPtr, size_t structSize);
};

template<typename T>
bool ConfigVaultClass::loadConfig(T& configStruct) {
  if (!configLoaded) return false;
  return deserializeStruct(configJson, &configStruct, sizeof(T));
}

template<typename T>
bool ConfigVaultClass::saveConfig(const T& configStruct) {
  String json;
  if (!serializeStruct(&configStruct, sizeof(T), json)) return false;
  return ConfigStorage.save(json);
}

extern ConfigVaultClass ConfigVault;
