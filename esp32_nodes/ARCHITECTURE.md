# ESP32 Smart Farm - System Architecture

## System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                      Smart Farm IoT System                       │
│                                                                   │
│  ┌──────────────┐         ┌──────────────┐                      │
│  │  Soil Node   │         │Weather Node  │                      │
│  │  (ESP32)     │         │  (ESP32)     │                      │
│  │              │         │              │                      │
│  │ • Moisture   │         │ • Air Temp   │                      │
│  │ • Soil Temp  │         │ • Humidity   │                      │
│  │ • pH         │         │ • Light      │                      │
│  │ • Leaf Temp  │         │ • Rainfall   │                      │
│  │ • Wetness    │         │ • Wind       │                      │
│  │              │         │ • Gas/CO2/CO │                      │
│  └──────┬───────┘         └──────┬───────┘                      │
│         │                        │                               │
│         │    ESP-NOW (2.4GHz)    │                              │
│         │    Local Network       │                              │
│         └────────────┬───────────┘                              │
│                      │                                           │
│         ┌────────────▼───────────┐                              │
│         │   Gateway Node         │                              │
│         │     (ESP32)            │                              │
│         │                        │                              │
│         │ • Data Aggregation     │                              │
│         │ • LCD Display          │                              │
│         │ • Motion Sensor        │                              │
│         │ • Weight Scale         │                              │
│         │ • Water Level          │                              │
│         │ • Firebase Upload      │                              │
│         └────────────┬───────────┘                              │
│                      │                                           │
└──────────────────────┼───────────────────────────────────────────┘
                       │
                       │ WiFi/HTTPS
                       │
            ┌──────────▼──────────┐
            │   Firebase Cloud    │
            │  Realtime Database  │
            │                     │
            │  • Data Storage     │
            │  • Real-time Sync   │
            │  • Authentication   │
            └──────────┬──────────┘
                       │
                       │ WebSocket/REST
                       │
            ┌──────────▼──────────┐
            │   Web Dashboard     │
            │   (HTML/JS/CSS)     │
            │                     │
            │  • Live Monitoring  │
            │  • Data Viz         │
            │  • Alerts           │
            └─────────────────────┘
```

## Communication Protocols

### ESP-NOW (Node ↔ Gateway)
- **Protocol:** ESP-NOW
- **Frequency:** 2.4 GHz
- **Range:** ~200m line of sight
- **Latency:** <10ms
- **Advantages:**
  - Low power consumption
  - No WiFi router needed
  - Fast peer-to-peer
  - Encrypted communication

### WiFi/HTTPS (Gateway ↔ Cloud)
- **Protocol:** HTTPS over WiFi
- **Frequency:** 2.4 GHz
- **API:** Firebase REST API
- **Advantages:**
  - Cloud storage
  - Remote access
  - Data persistence
  - Authentication

### WebSocket (Cloud ↔ Dashboard)
- **Protocol:** WebSocket / Long Polling
- **Connection:** Firebase SDK
- **Advantages:**
  - Real-time updates
  - Bidirectional
  - Auto-reconnect
  - Event-driven

## Hardware Architecture

### Gateway Node Components
```
ESP32 Dev Board
├── HX711 Load Cell Amplifier → Weight Sensor
├── I2C LCD 16x2 → Display
├── HC-SR04 Ultrasonic → Water Level
├── PIR Sensor → Motion Detection
└── WiFi/ESP-NOW Radio → Communication
```

### Soil Node Components
```
ESP32 Dev Board
├── Analog Soil Moisture Sensor
├── DS18B20 → Soil Temperature
├── Analog pH Sensor
├── Analog Leaf Temperature
├── Analog Leaf Wetness
└── ESP-NOW Radio → Communication
```

### Weather Node Components
```
ESP32 Dev Board
├── DHT22 → Air Temperature & Humidity
├── LDR → Light Intensity
├── Digital Rainfall Sensor
├── Anemometer → Wind Speed
├── Wind Vane → Wind Direction
├── MQ2 → Gas Detection
├── MQ135 → CO2 Detection
├── MQ7 → CO Detection
└── ESP-NOW Radio → Communication
```

## Software Architecture

### Layered Architecture
```
┌─────────────────────────────────────┐
│      Application Layer              │
│  - Business Logic                   │
│  - Alert Generation                 │
│  - Data Formatting                  │
├─────────────────────────────────────┤
│      Communication Layer            │
│  - ESP-NOW Protocol                 │
│  - Firebase API                     │
│  - Data Serialization               │
├─────────────────────────────────────┤
│      Hardware Abstraction Layer     │
│  - Sensor Drivers                   │
│  - GPIO Management                  │
│  - ADC Reading                      │
├─────────────────────────────────────┤
│      ESP32 Hardware                 │
│  - WiFi Radio                       │
│  - ADC/GPIO                         │
│  - Timers/Interrupts                │
└─────────────────────────────────────┘
```

## Data Flow

### 1. Sensor Reading (Every 5 seconds)
```
Sensor → ADC/GPIO → Calibration → Filtering → Data Structure
```

### 2. Node to Gateway (ESP-NOW)
```
Node: Package Data → ESP-NOW Send → Gateway: Receive → Store
```

### 3. Gateway to Cloud (Firebase)
```
Gateway: Aggregate → JSON Format → HTTPS POST → Firebase: Store
```

### 4. Cloud to Dashboard (Real-time)
```
Firebase: Update → WebSocket Push → Dashboard: Display
```

## Power Management

### Sensor Nodes (Battery Operation)
```cpp
// Sleep mode for power saving
void enterSleepMode() {
    esp_sleep_enable_timer_wakeup(5 * 60 * 1000000); // 5 min
    esp_light_sleep_start();
}
```

### Gateway Node (Continuous Operation)
```cpp
// WiFi power saving
WiFi.setSleep(true);
esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
```

## Security

### ESP-NOW Encryption
- Built-in AES-128 encryption
- Peer authentication required
- MAC address filtering

### Firebase Security
- API key authentication
- HTTPS encrypted transport
- Database security rules
- User authentication (optional)

## Error Handling

### Node Level
1. Sensor read failures → Retry with backoff
2. Communication failures → Queue and retry
3. Memory errors → Restart with error log

### Gateway Level
1. WiFi disconnection → Auto-reconnect
2. Firebase errors → Local caching
3. Node timeout → Alert generation

### Dashboard Level
1. Connection loss → Reconnection attempt
2. Data timeout → Warning display
3. Invalid data → Validation error

## Monitoring & Debugging

### Serial Monitor Output
```
[INFO] Node ID: 1 (Soil Node)
[INFO] WiFi: Connected to Wokwi-GUEST
[INFO] Firebase: Connected
[DATA] Moisture: 45.5%, Temp: 22.3°C, pH: 6.8
[SEND] Data sent successfully
```

### LCD Display (Gateway)
```
Line 1: Temp:25C Hum:60%
Line 2: Motion:Y  Wtr:75%
```

### Dashboard Alerts
- Low water level
- High temperature
- Motion detected
- Poor air quality
- Sensor disconnection

## Scalability

### Adding New Nodes
1. Clone node firmware
2. Assign unique NODE_ID
3. Register MAC address
4. Deploy and test

### Adding New Sensors
1. Define pin in config.h
2. Add read function
3. Update data structure
4. Modify display/upload code

## Performance Metrics

### Typical Values
- **Sensor read time:** ~100ms
- **ESP-NOW latency:** <10ms
- **Firebase upload:** ~500ms
- **Dashboard update:** Real-time
- **Power consumption (node):** ~80mA active, ~5mA sleep
- **Memory usage:** RAM <50%, Flash <60%

## Future Enhancements

1. **OTA Updates:** Firmware updates over WiFi
2. **SD Card Logging:** Local data backup
3. **MQTT Integration:** Alternative cloud protocol
4. **Mesh Network:** Extended range with mesh
5. **Mobile App:** iOS/Android application
6. **ML Predictions:** Weather forecasting, plant health
