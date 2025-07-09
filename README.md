# ConfigVault

**Version**: 1.0.0  
**Author**: Souvik  

---

## Overview

ConfigVault is a lightweight, easy-to-use configuration library for ESP32 and ESP8266. It automatically detects the platform, stores configuration data in non‑volatile storage (Preferences on ESP32, LittleFS on ESP8266), and—if no valid configuration is found or a designated GPIO goes HIGH at boot—launches a minimal web UI to collect settings.

---

## Features

- **Simple API**: One-line initialization; templated load/save functions.
- **Platform‑agnostic**: Hides all `#ifdef` logic; supports ESP32 & ESP8266.
- **Auto WebUI**: Falls back to a built-in AsyncWebServer form when no config is present.
- **GPIO trigger**: Holding a user‑specified pin HIGH on boot forces WebUI mode.
- **Flash formatting**: `formatFlash()` to erase stored config.
- **Versioning**: Built‑in version macro (`CONFIGVAULT_VERSION`) embedded in saved JSON.
- **Minimal footprint**: Uses ArduinoJson for (de)serialization; AsyncWebServer for non‑blocking UI.

---

## Installation

1. Download or clone the `ConfigVault` folder into your Arduino `libraries/` directory.
2. Ensure you have the following dependencies installed via Library Manager:
   - **ArduinoJson**  
   - **ESPAsyncWebServer**  
   - **AsyncTCP** (ESP32) or **ESPAsyncTCP** (ESP8266)  
   - **LittleFS** (for ESP8266)  
3. Restart the Arduino IDE.

---

## Library Structure

```
ConfigVault/
├── src/
│   ├── ConfigVault.h
│   ├── ConfigVault.cpp
│   ├── ConfigStorage.h
│   ├── ConfigStorage.cpp
│   ├── WebUIHandler.h
│   ├── WebUIHandler.cpp
│   ├── version.h
│   ├── esp32/
│   │   ├── PreferencesStorage.h
│   │   └── PreferencesStorage.cpp
│   └── esp8266/
│       ├── FSStorage.h
│       └── FSStorage.cpp
├── examples/
│   └── ConfigVaultBasic/
│       └── ConfigVaultBasic.ino
├── library.properties
└── README.md
```

---

## Usage

### 1. Define Your Config Struct

```cpp
struct MyConfig {
  char ssid[32];
  char pass[64];
  uint16_t deviceId;
  float threshold;
};
```

### 2. Basic Sketch

```cpp
#include <ConfigVault.h>

MyConfig cfg;

void setup() {
  Serial.begin(115200);
  // `triggerPin` optional; set to a valid GPIO to force WebUI on HIGH
  if (!ConfigVault.begin("MyDevice", 0)) {
    // WebUI launched—enter configuration via browser at http://<device_IP>
    return;
  }

  if (ConfigVault.configPresent()) {
    if (ConfigVault.loadConfig(cfg)) {
      Serial.println("Config loaded:");
      Serial.println(cfg.ssid);
    }
  }
}

void loop() {
  // Application code here…
  // Save updated config if needed:
  // ConfigVault.saveConfig(cfg);
}
```

---

## API Reference

- `bool begin(const String& deviceName, uint8_t triggerPin = 255);`  
  Initializes storage and checks for existing config. If no config or triggerPin HIGH, launches WebUI and returns `false`. Otherwise returns `true`.

- `template<typename T> bool loadConfig(T& configStruct);`  
  Deserializes stored JSON into your struct. Returns `false` if no config was loaded or on error.

- `template<typename T> bool saveConfig(const T& configStruct);`  
  Serializes your struct to JSON and saves to storage. Returns `true` on success.

- `uint8_t formatFlash();`  
  Erases stored config. Returns `0` on success, `1` on failure.

- `bool configPresent();`  
  Returns `true` if a valid config has been loaded since `begin()`.

- `#define CONFIGVAULT_VERSION "1.0.0"`  
  Library version; also embedded in saved JSON under `"_version"`.

---

## WebUI

- Automatically served on port 80 when:
  - No valid config on startup.
  - `triggerPin` is held HIGH during boot.
- Access via browser at `http://<device_IP>/`
- Form fields generated dynamically based on byte‑indexed keys (“b0”, “b1”, …).
- On submission, config is saved and device restarts.

---

## Example

See `examples/ConfigVaultBasic/ConfigVaultBasic.ino` for a complete working sketch.

---

## License

MIT License. See [LICENSE](LICENSE) for details.
