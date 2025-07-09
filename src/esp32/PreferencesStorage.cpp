#include "PreferencesStorage.h"

void PreferencesStorage::begin(const String& nsName) {
  _nsName = nsName;
}

bool PreferencesStorage::load(String& outJson) {
  Preferences prefs;
  if (!prefs.begin(_nsName.c_str(), true)) {
    return false;
  }

  String val = prefs.getString(_key, "");
  prefs.end();

  if (val.length() == 0) return false;

  outJson = val;
  return true;
}

bool PreferencesStorage::save(const String& json) {
  Preferences prefs;
  if (!prefs.begin(_nsName.c_str(), false)) {
    return false;
  }

  bool ok = prefs.putString(_key, json);
  prefs.end();
  return ok;
}

bool PreferencesStorage::clear() {
  Preferences prefs;
  if (!prefs.begin(_nsName.c_str(), false)) {
    return false;
  }

  prefs.clear();
  prefs.end();
  return true;
}

bool PreferencesStorage::exists() {
  String tmp;
  return load(tmp);
}
