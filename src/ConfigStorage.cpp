#include "ConfigStorage.h"

#if defined(ESP32)
  #include "esp32/PreferencesStorage.h"
  PreferencesStorage storageImpl;
#elif defined(ESP8266)
  #include "esp8266/FSStorage.h"
  FSStorage storageImpl;
#else
  #error "Unsupported platform"
#endif

ConfigStorageClass ConfigStorage;

void ConfigStorageClass::begin(const String& nsName) {
  _nsName = nsName;
  storageImpl.begin(nsName);
}

bool ConfigStorageClass::load(String& outJson) {
  return storageImpl.load(outJson);
}

bool ConfigStorageClass::save(const String& json) {
  return storageImpl.save(json);
}

bool ConfigStorageClass::clear() {
  return storageImpl.clear();
}

bool ConfigStorageClass::exists() {
  return storageImpl.exists();
}
