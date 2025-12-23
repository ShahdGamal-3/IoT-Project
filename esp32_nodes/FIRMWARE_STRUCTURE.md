# ESP32 Nodes Firmware Structure

## 📁 Project Architecture

```
esp32_nodes/
├── gateway_node/           # Central Gateway Node
│   ├── src/
│   │   └── gateway_node.cpp
│   ├── include/
│   │   ├── config.h        # Configuration constants
│   │   ├── sensors.h       # Sensor definitions
│   │   ├── firebase.h      # Firebase communication
│   │   └── display.h       # LCD display functions
│   ├── lib/                # Local libraries
│   ├── platformio.ini      # Build configuration
│   └── wokwi.toml         # Wokwi simulation config
│
├── soil_node/             # Soil Monitoring Node
│   ├── src/
│   │   └── soil_node.cpp
│   ├── include/
│   │   ├── config.h       # Node configuration
│   │   ├── sensors.h      # Soil sensor definitions
│   │   └── communication.h # ESP-NOW/WiFi protocols
│   ├── platformio.ini
│   └── wokwi.toml
│
├── weather_node/          # Weather Monitoring Node
│   ├── src/
│   │   └── weather_node.cpp
│   ├── include/
│   │   ├── config.h
│   │   ├── sensors.h
│   │   └── communication.h
│   ├── platformio.ini
│   └── wokwi.toml
│
├── common/                # Shared code across all nodes
│   ├── include/
│   │   ├── common_types.h     # Shared data structures
│   │   ├── protocols.h        # Communication protocols
│   │   └── utils.h            # Utility functions
│   └── src/
│       ├── protocols.cpp
│       └── utils.cpp
│
├── docs/                  # Documentation
│   ├── API.md
│   ├── ARCHITECTURE.md
│   └── DEPLOYMENT.md
│
└── scripts/               # Build and deployment scripts
    ├── build_all.ps1      # Build all nodes
    ├── flash_all.ps1      # Flash all nodes
    └── monitor.ps1        # Monitor serial output
```

---

## 🏗️ Firmware Components

### 1. Gateway Node Architecture

```
Gateway Node (Central Hub)
├── Hardware Layer
│   ├── ESP32 (Main Controller)
│   ├── HX711 (Load Cell - Weight)
│   ├── LCD I2C Display (16x2)
│   ├── PIR Motion Sensor
│   └── Ultrasonic Sensor (Water Level)
│
├── Communication Layer
│   ├── WiFi (Internet Connection)
│   ├── Firebase (Cloud Database)
│   └── ESP-NOW (Receive from nodes)
│
├── Processing Layer
│   ├── Data Aggregation
│   ├── Sensor Reading
│   ├── Alert Generation
│   └── Display Management
│
└── Application Layer
    ├── Firebase Upload
    ├── LCD Display Update
    ├── Motion Detection
    └── Weight Monitoring
```

**Key Responsibilities:**
- Collect data from sensor nodes via ESP-NOW
- Read local sensors (weight, motion, water level)
- Upload all data to Firebase
- Display critical info on LCD
- Generate alerts for critical conditions

---

### 2. Soil Node Architecture

```
Soil Node (Environmental Monitoring)
├── Hardware Layer
│   ├── ESP32 (Controller)
│   ├── Soil Moisture Sensor (Analog)
│   ├── DS18B20 (Soil Temperature)
│   ├── pH Sensor (Analog)
│   ├── Leaf Temperature Sensor
│   └── Leaf Wetness Sensor
│
├── Communication Layer
│   └── ESP-NOW (Send to Gateway)
│
├── Processing Layer
│   ├── Sensor Calibration
│   ├── Data Filtering
│   └── Data Packaging
│
└── Application Layer
    ├── Periodic Reading (every 5s)
    ├── Data Transmission
    └── Sleep Mode (power saving)
```

**Key Responsibilities:**
- Monitor soil conditions (moisture, temp, pH)
- Monitor plant health (leaf temp, wetness)
- Transmit data to gateway via ESP-NOW
- Power management for battery operation

---

### 3. Weather Node Architecture

```
Weather Node (Climate Monitoring)
├── Hardware Layer
│   ├── ESP32 (Controller)
│   ├── DHT22 (Air Temp & Humidity)
│   ├── LDR (Light Sensor)
│   ├── Rainfall Sensor (Digital)
│   ├── Wind Speed Sensor (Anemometer)
│   ├── Wind Direction Sensor
│   ├── MQ2 (Gas Sensor)
│   ├── MQ135 (CO2 Sensor)
│   └── MQ7 (CO Sensor)
│
├── Communication Layer
│   └── ESP-NOW (Send to Gateway)
│
├── Processing Layer
│   ├── Sensor Calibration
│   ├── Air Quality Index Calculation
│   ├── Wind Vector Processing
│   └── Data Packaging
│
└── Application Layer
    ├── Periodic Reading (every 5s)
    ├── Air Quality Monitoring
    ├── Weather Condition Detection
    └── Data Transmission
```

**Key Responsibilities:**
- Monitor weather conditions (temp, humidity, rainfall)
- Monitor air quality (gas, CO2, CO)
- Monitor wind (speed, direction)
- Monitor light intensity
- Transmit data to gateway via ESP-NOW

---

## 📊 Data Flow Architecture

```
┌─────────────────┐         ┌─────────────────┐
│   Soil Node     │         │  Weather Node   │
│  (ESP-NOW TX)   │         │  (ESP-NOW TX)   │
└────────┬────────┘         └────────┬────────┘
         │                           │
         │  ESP-NOW Protocol         │
         │  (2.4GHz, Local)          │
         │                           │
         └────────────┬──────────────┘
                      │
                      ▼
         ┌────────────────────────┐
         │    Gateway Node        │
         │  (ESP-NOW RX + WiFi)   │
         │  • Aggregates Data     │
         │  • Local Sensors       │
         │  • LCD Display         │
         └────────────┬───────────┘
                      │
                      │ WiFi/HTTPS
                      │ (Cloud)
                      ▼
         ┌────────────────────────┐
         │   Firebase Realtime    │
         │      Database          │
         └────────────┬───────────┘
                      │
                      │ WebSocket/REST
                      │ (Real-time)
                      ▼
         ┌────────────────────────┐
         │   Web Dashboard        │
         │  (Browser Interface)   │
         └────────────────────────┘
```

---

## 📦 Data Structures

### Sensor Data Packet Structure

```cpp
// Common packet header
struct PacketHeader {
    uint8_t nodeId;          // Node identifier (1=Soil, 2=Weather)
    uint8_t packetType;      // Type of data packet
    uint32_t timestamp;      // Unix timestamp
    uint16_t sequence;       // Packet sequence number
    uint8_t checksum;        // Data integrity check
};

// Soil Node Data
struct SoilData {
    PacketHeader header;
    float soilMoisture;      // 0-100%
    float soilTemp;          // Celsius
    float soilPH;            // 0-14
    float leafTemp;          // Celsius
    float leafWetness;       // 0-100%
};

// Weather Node Data
struct WeatherData {
    PacketHeader header;
    float airTemp;           // Celsius
    float humidity;          // 0-100%
    uint16_t light;          // Lux
    float rainfall;          // mm
    float windSpeed;         // m/s
    uint16_t windDirection;  // 0-360 degrees
    uint16_t gas;            // PPM
    uint16_t co2;            // PPM
    uint16_t co;             // PPM
};

// Gateway Local Data
struct GatewayData {
    float waterLevel;        // 0-100%
    bool motion;             // true/false
    float weight;            // kg
};
```

---

## 🔧 Configuration Files

### config.h (Per Node)

```cpp
#ifndef CONFIG_H
#define CONFIG_H

// Node Identity
#define NODE_ID 1                    // 1=Soil, 2=Weather, 3=Gateway
#define NODE_NAME "Soil_Node"

// WiFi Configuration (Gateway Only)
#define WIFI_SSID "your_ssid"
#define WIFI_PASSWORD "your_password"

// Firebase Configuration (Gateway Only)
#define FIREBASE_HOST "your_project.firebaseio.com"
#define FIREBASE_AUTH "your_firebase_token"

// ESP-NOW Configuration
#define GATEWAY_MAC {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
#define CHANNEL 1                    // WiFi channel (1-13)
#define MAX_RETRY 3                  // Transmission retry attempts

// Sensor Pins
#define SOIL_MOISTURE_PIN 34
#define SOIL_TEMP_PIN 4
#define SOIL_PH_PIN 35
#define LEAF_TEMP_PIN 32
#define LEAF_WETNESS_PIN 33

// Timing Configuration
#define READING_INTERVAL 5000        // 5 seconds
#define SEND_INTERVAL 5000           // 5 seconds
#define RECONNECT_INTERVAL 30000     // 30 seconds

// Sensor Thresholds
#define MOISTURE_LOW 30.0
#define MOISTURE_HIGH 70.0
#define TEMP_LOW 15.0
#define TEMP_HIGH 35.0
#define PH_LOW 6.0
#define PH_HIGH 7.5

#endif
```

---

## 🚀 Build System

### platformio.ini (Standard Template)

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

# Common libraries for all nodes
lib_deps = 
    ESP32

# Build flags
build_flags = 
    -DCORE_DEBUG_LEVEL=3
    -DBOARD_HAS_PSRAM

# Upload settings
upload_speed = 921600
upload_port = COM3  ; Change per node

# Monitor settings
monitor_port = COM3  ; Change per node
monitor_filters = 
    colorize
    time
    esp32_exception_decoder
```

### Node-Specific Dependencies

**Gateway Node:**
```ini
lib_deps = 
    mobizt/Firebase ESP32 Client@^4.4.17
    marcoschwartz/LiquidCrystal_I2C@^1.1.4
    bogde/HX711@^0.7.5
```

**Soil Node:**
```ini
lib_deps = 
    paulstoffregen/OneWire@^2.3.8
    milesburton/DallasTemperature@^3.9.0
```

**Weather Node:**
```ini
lib_deps = 
    adafruit/DHT sensor library@^1.4.6
    adafruit/Adafruit Unified Sensor@^1.1.14
```

---

## 🔄 State Machine

### Node State Diagram

```
┌─────────────┐
│    INIT     │ Power on, initialize hardware
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   CONFIG    │ Load configuration, setup WiFi/ESP-NOW
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   READY     │ Wait for next reading interval
└──────┬──────┘
       │
       ▼
┌─────────────┐
│    READ     │ Read all sensors
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   PROCESS   │ Filter, calibrate, package data
└──────┬──────┘
       │
       ▼
┌─────────────┐
│    SEND     │ Transmit via ESP-NOW/Firebase
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   SLEEP     │ Low power mode (optional)
└──────┬──────┘
       │
       └──────► (Back to READY)
```

---

## 📝 Coding Standards

### File Organization

```cpp
// 1. Header Guards
#ifndef GATEWAY_NODE_H
#define GATEWAY_NODE_H

// 2. Includes (System first, then project)
#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "sensors.h"

// 3. Defines and Constants
#define LED_PIN 2

// 4. Type Definitions
typedef struct { ... } SensorData;

// 5. Global Variables (minimize usage)
extern float temperature;

// 6. Function Declarations
void initSensors();
void readSensors();
void sendData();

// 7. Implementation
void initSensors() {
    // Implementation here
}

#endif
```

### Naming Conventions

- **Functions:** `camelCase` - `readSensorData()`
- **Variables:** `camelCase` - `sensorValue`
- **Constants:** `UPPER_SNAKE_CASE` - `MAX_RETRY_COUNT`
- **Structs:** `PascalCase` - `SensorData`
- **Macros:** `UPPER_SNAKE_CASE` - `DEBUG_PRINT(x)`

---

## 🧪 Testing Strategy

### Unit Testing
```cpp
// test/test_sensors.cpp
#include <unity.h>
#include "sensors.h"

void test_soil_moisture_range() {
    float moisture = readSoilMoisture();
    TEST_ASSERT_TRUE(moisture >= 0.0 && moisture <= 100.0);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_soil_moisture_range);
    UNITY_END();
}
```

### Integration Testing
- Test ESP-NOW communication between nodes
- Test Firebase connectivity and data upload
- Test sensor reading accuracy
- Test alert generation

---

## 🔒 Error Handling

```cpp
// Error codes
enum ErrorCode {
    ERROR_NONE = 0,
    ERROR_WIFI = 1,
    ERROR_FIREBASE = 2,
    ERROR_SENSOR = 3,
    ERROR_ESPNOW = 4,
    ERROR_MEMORY = 5
};

// Error handling function
void handleError(ErrorCode error) {
    Serial.printf("Error: %d\n", error);
    
    switch(error) {
        case ERROR_WIFI:
            // Attempt WiFi reconnection
            reconnectWiFi();
            break;
        case ERROR_SENSOR:
            // Reset sensor
            resetSensors();
            break;
        // ... handle other errors
    }
}
```

---

## 📈 Performance Optimization

### Memory Management
- Use `const` for read-only data (store in flash)
- Minimize global variables
- Use stack allocation when possible
- Free dynamically allocated memory

### Power Management
```cpp
// Enable light sleep for sensor nodes
esp_sleep_enable_timer_wakeup(5000000); // 5 seconds
esp_light_sleep_start();

// Reduce WiFi power consumption
WiFi.setSleep(true);
esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
```

### Communication Efficiency
- Batch sensor readings
- Use binary protocols (ESP-NOW)
- Compress data when possible
- Implement retry logic with exponential backoff

---

## 🛠️ Development Workflow

### 1. Setup Development Environment
```powershell
# Install PlatformIO
pip install platformio

# Clone repository
git clone <repo_url>
cd esp32_nodes
```

### 2. Build Individual Node
```powershell
cd gateway_node
pio run
```

### 3. Upload to ESP32
```powershell
pio run --target upload
```

### 4. Monitor Serial Output
```powershell
pio device monitor
```

### 5. Build All Nodes
```powershell
.\scripts\build_all.ps1
```

---

## 📚 API Reference

### ESP-NOW Functions
```cpp
// Initialize ESP-NOW
esp_err_t initESPNOW();

// Register peer (gateway)
esp_err_t registerPeer(uint8_t* macAddr);

// Send data
esp_err_t sendData(uint8_t* data, size_t len);

// Callback on data received
void onDataReceived(const uint8_t* mac, const uint8_t* data, int len);
```

### Firebase Functions
```cpp
// Initialize Firebase
void initFirebase(const char* host, const char* auth);

// Upload sensor data
bool uploadToFirebase(String path, float value);

// Read from Firebase
float readFromFirebase(String path);
```

---

## 🚦 Status Indicators

### LED Patterns
- **Solid Green:** Normal operation
- **Blinking Green:** Sending data
- **Solid Red:** Error state
- **Blinking Red:** Attempting reconnection
- **Blue:** Setup/Configuration mode

---

## 📖 Version History

### v1.0.0 - Initial Release
- Basic sensor reading
- ESP-NOW communication
- Firebase integration
- LCD display support

### Future Enhancements
- [ ] OTA (Over-The-Air) updates
- [ ] SD card logging
- [ ] Battery monitoring
- [ ] Mesh networking
- [ ] Local web server
- [ ] MQTT support

---

## 👥 Contributing

See CONTRIBUTING.md for guidelines on:
- Code style
- Commit messages
- Pull request process
- Testing requirements

---

## 📄 License

This project is licensed under the MIT License.

---

## 📞 Support

For issues and questions:
- GitHub Issues: <repo_url>/issues
- Email: support@example.com
- Documentation: <docs_url>
