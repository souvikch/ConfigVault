#include "ConfigVault.h"
#include "ConfigStorage.h"
#include "WebUIHandler.h"

#include <ArduinoJson.h>

ConfigVaultClass ConfigVault;

bool ConfigVaultClass::begin(const String& deviceName, uint8_t triggerPin) {
  meta.deviceName = deviceName;
  meta.triggerPin = triggerPin;

  ConfigStorage.begin(deviceName);

  if (shouldStartWebUI()) {
    launchWebUI();
    return false;
  }

  if (!ConfigStorage.load(configJson)) {
    launchWebUI();  // fallback if config not found
    return false;
  }

  configLoaded = true;
  return true;
}

bool ConfigVaultClass::configPresent() {
  return configLoaded;
}

uint8_t ConfigVaultClass::formatFlash() {
  return ConfigStorage.clear() ? 0 : 1;
}

bool ConfigVaultClass::shouldStartWebUI() {
  if (meta.triggerPin != 255) {
    pinMode(meta.triggerPin, INPUT);
    if (digitalRead(meta.triggerPin) == HIGH) {
      return true;
    }
  }

  return !ConfigStorage.exists();
}

void ConfigVaultClass::launchWebUI() {
  WebUIHandler::start(meta.deviceName, [](const String& newJson) {
    ConfigStorage.save(newJson);
    ESP.restart();
  });
}

// Serialization/Deserialization via JSON

bool ConfigVaultClass::serializeStruct(const void* structPtr, size_t structSize, String& outJson) {
  DynamicJsonDocument doc(1024);
  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(structPtr);

  for (size_t i = 0; i < structSize; ++i) {
    doc["b" + String(i)] = bytes[i];
  }
  doc["_version"] = CONFIGVAULT_VERSION;

  serializeJson(doc, outJson);
  return true;
}

bool ConfigVaultClass::deserializeStruct(const String& json, void* structPtr, size_t structSize) {
  DynamicJsonDocument doc(1024);
  DeserializationError err = deserializeJson(doc, json);
  if (err) return false;

  uint8_t* bytes = reinterpret_cast<uint8_t*>(structPtr);

  for (size_t i = 0; i < structSize; ++i) {
    String key = "b" + String(i);
    if (!doc.containsKey(key)) return false;
    bytes[i] = doc[key];
  }

  return true;
}

/*template<typename T>
bool ConfigVaultClass::loadConfig(T& configStruct) {
  if (!configLoaded) return false;
  return deserializeStruct(configJson, &configStruct, sizeof(T));
}

template<typename T>
bool ConfigVaultClass::saveConfig(const T& configStruct) {
  String json;
  if (!serializeStruct(&configStruct, sizeof(T), json)) return false;
  return ConfigStorage.save(json);
}*/

// Explicit template instantiation
/*template bool ConfigVaultClass::loadConfig<>(MyConfig& configStruct);
template bool ConfigVaultClass::saveConfig<>(const MyConfig& configStruct);*/
