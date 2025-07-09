#include "FSStorage.h"

void FSStorage::begin(const String& nsName) {
  if (!LittleFS.begin()) {
    // If LittleFS fails, optionally fallback to SPIFFS
    // SPIFFS.begin();
  }
}

bool FSStorage::load(String& outJson) {
  File f = LittleFS.open(_filename, "r");
  if (!f) return false;

  outJson = f.readString();
  f.close();

  return outJson.length() > 0;
}

bool FSStorage::save(const String& json) {
  File f = LittleFS.open(_filename, "w");
  if (!f) return false;

  size_t written = f.print(json);
  f.close();

  return written == json.length();
}

bool FSStorage::clear() {
  return LittleFS.remove(_filename);
}

bool FSStorage::exists() {
  return LittleFS.exists(_filename);
}
