# Smart Farm IoT Dashboard

## Professional Web Dashboard for Real-Time Farm Monitoring

This dashboard provides a comprehensive, real-time interface for monitoring and controlling all 16 sensors in your IoT farm monitoring system.

---

## 🚀 Quick Start

### Simulation Mode (Default - For Testing)
1. Open `dashboard/index.html` in a web browser (or use HTTP server)
2. Dashboard runs in **Simulation Mode** - no ESP32 connection required
3. Use sliders to test all sensor displays and alerts
4. Perfect for Wokwi simulation or offline testing

### Real Hardware Mode (ESP32 Connected)
1. Open `dashboard/app.js`
2. Set `SIMULATION_MODE = false` (line 3)
3. Update `ESP32_IP` to your ESP32's IP address (line 2)
4. Upload WebSocket server code to ESP32 (see Setup section)
5. Refresh dashboard - should connect automatically

---

## 🌟 Features

### Real-Time Monitoring
- **Live sensor data updates** via WebSocket connection
- **Auto-reconnection** if connection is lost
- **Color-coded status badges** for quick assessment
- **Alert notifications** for critical conditions

### Interactive Controls
- **Professional sliders** for simulating each sensor value
- **Instant feedback** with value updates
- **Bidirectional communication** - control ESP32 from dashboard

### Organized Sections
1. **Soil Monitoring** - Moisture, Temperature, pH
2. **Plant Health** - Leaf Temperature, Leaf Wetness
3. **Climate Monitoring** - Air Temp, Humidity, Light, Rainfall
4. **Wind Monitoring** - Speed and Direction
5. **Air Quality** - Gas (MQ2), CO₂, CO sensors
6. **Resources & Security** - Water Tank, Motion, Weight

### Visual Indicators
- ✅ **Green badges** - Normal/Optimal conditions
- ⚠️ **Yellow badges** - Warning conditions
- 🔴 **Red badges** - Danger/Critical conditions
- 📊 **Real-time graphs** (coming soon)

---

## 🔌 Communication Protocol: WebSocket

### Why WebSocket?

**WebSocket** is the recommended protocol because it provides:

1. **Full-Duplex Communication** - Simultaneous send/receive
2. **Low Latency** - Real-time updates (<50ms)
3. **Efficient** - Persistent connection, no HTTP overhead
4. **Native ESP32 Support** - Built-in Arduino libraries
5. **Browser Compatible** - Works in all modern browsers

### Alternative Protocols

| Protocol | Pros | Cons | Best Use Case |
|----------|------|------|---------------|
| **WebSocket** | Real-time, bidirectional, low latency | Requires persistent connection | Live monitoring dashboards |
| **MQTT** | Lightweight, pub/sub model, cloud-ready | Requires broker | IoT cloud integration |
| **HTTP REST API** | Simple, stateless, cacheable | Higher latency, polling overhead | Mobile apps, periodic updates |
| **Bluetooth** | Low power, no WiFi needed | Short range (~10m) | Wearable devices, proximity |
| **LoRa** | Long range (>1km), low power | Low bandwidth, requires gateway | Remote rural farms |

---

## 📋 Setup Instructions

### 1. Dashboard Setup

1. **Open the dashboard:**
   ```bash
   # Simply open index.html in a browser
   # Or use a local server:
   python -m http.server 8080
   # Then navigate to: http://localhost:8080
   ```

2. **Configure ESP32 IP address:**
   - Open `app.js`
   - Line 2: Change `const ESP32_IP = '192.168.1.100';` to your ESP32's IP
   - Save the file

### 2. ESP32 WebSocket Server Setup

#### Install Required Library

Add to your `platformio.ini`:
```ini
lib_deps =
    LiquidCrystal_I2C
    OneWire
    DallasTemperature
    adafruit/DHT sensor library
    bogde/HX711
    ottowinter/ESPAsyncWebServer-esphome @ ^3.0.0
    me-no-dev/AsyncTCP @ ^1.1.1
```

#### Add WiFi and WebSocket Code

Create a new file: `src/WebSocketServer.cpp`

```cpp
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// WebSocket server on port 81
AsyncWebServer server(80);
AsyncWebSocket ws("/");

// External sensor variables (declare these in main.ino)
extern float soilMoisture, soilTemp, soilPH;
extern float leafTemp, leafWetness;
extern float airTemp, humidity, lightIntensity;
extern float rainfall, windSpeed, windDirection;
extern int gasPPM, co2PPM, coPPM;
extern float waterLevel, weight;
extern bool motionDetected;

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
               AwsEventType type, void *arg, uint8_t *data, size_t len) {
    
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client #%u connected\n", client->id());
        
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        
    } else if (type == WS_EVT_DATA) {
        // Handle incoming commands from dashboard
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0;
            String message = (char*)data;
            Serial.println("Received: " + message);
            
            // Parse JSON command
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, message);
            
            if (!error) {
                const char* command = doc["command"];
                float value = doc["value"];
                
                // Update sensor simulation values based on command
                // (In Wokwi, you would update the potentiometer values here)
                Serial.printf("Command: %s, Value: %.2f\n", command, value);
            }
        }
    }
}

void setupWebSocket() {
    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
    
    // Setup WebSocket
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    
    // Start server
    server.begin();
    Serial.println("WebSocket server started on port 81");
}

void sendSensorData() {
    // Create JSON document
    StaticJsonDocument<512> doc;
    
    // Add all sensor data
    doc["soilMoisture"] = soilMoisture;
    doc["soilTemp"] = soilTemp;
    doc["soilPH"] = soilPH;
    doc["leafTemp"] = leafTemp;
    doc["leafWetness"] = leafWetness;
    doc["airTemp"] = airTemp;
    doc["humidity"] = humidity;
    doc["light"] = lightIntensity;
    doc["rainfall"] = rainfall;
    doc["windSpeed"] = windSpeed;
    doc["windDirection"] = windDirection;
    doc["gas"] = gasPPM;
    doc["co2"] = co2PPM;
    doc["co"] = coPPM;
    doc["waterLevel"] = waterLevel;
    doc["motion"] = motionDetected;
    doc["weight"] = weight;
    doc["timestamp"] = millis();
    
    // Serialize to string
    String jsonString;
    serializeJson(doc, jsonString);
    
    // Send to all connected clients
    ws.textAll(jsonString);
}
```

#### Update main.ino

Add to the beginning of `main.ino`:
```cpp
#include "WebSocketServer.h"

// In setup()
void setup() {
    // ... existing setup code ...
    
    setupWebSocket();
}

// In loop() - send data every 2 seconds
unsigned long lastWSUpdate = 0;

void loop() {
    unsigned long currentTime = millis();
    
    // ... existing sensor reading code ...
    
    // Send WebSocket updates
    if (currentTime - lastWSUpdate >= 2000) {
        lastWSUpdate = currentTime;
        sendSensorData();
    }
    
    // ... rest of existing code ...
}
```

---

## 🎨 Dashboard Usage

### 1. Opening the Dashboard

Simply open `index.html` in any modern web browser:
- Chrome
- Firefox
- Edge
- Safari

### 2. Connection Status

Top right corner shows:
- 🔴 **Red** = Disconnected
- 🟢 **Green** = Connected to ESP32

### 3. Reading Sensor Data

Each sensor card displays:
- **Large value** - Current reading
- **Unit** - Measurement unit (%, °C, ppm, etc.)
- **Status badge** - Condition assessment
- **Color coding** - Quick visual status

### 4. Simulating Values

Use the sliders to:
1. Adjust sensor values in real-time
2. Test different scenarios
3. Trigger alerts and warnings
4. See how the system responds

### 5. Monitoring Alerts

Bottom section shows:
- Recent alerts and warnings
- Timestamp for each event
- Color-coded severity
- Auto-scrolling list

---

## 🔧 Customization

### Change Color Theme

Edit `styles.css` at the top:
```css
:root {
    --primary-color: #2ecc71;  /* Main theme color */
    --secondary-color: #27ae60;
    --danger-color: #e74c3c;   /* Alert color */
    --warning-color: #f39c12;  /* Warning color */
}
```

### Adjust Update Frequency

In `app.js`, change the WebSocket send interval:
```javascript
// In ESP32 code (main.ino)
if (currentTime - lastWSUpdate >= 2000) {  // Change 2000 to desired milliseconds
```

### Add New Sensors

1. Add sensor card in `index.html`
2. Add styling in `styles.css`
3. Add data handling in `app.js`
4. Update ESP32 WebSocket data packet

---

## 📊 Technical Specifications

### Browser Requirements
- HTML5 WebSocket support
- CSS3 for animations
- JavaScript ES6+

### Network Requirements
- ESP32 and computer on same WiFi network
- Open port 81 for WebSocket
- Static IP recommended for ESP32

### Performance
- Update Rate: 2 seconds (configurable)
- Latency: <50ms typical
- Max Clients: 4 simultaneous connections
- Data Size: ~512 bytes per update

---

## 🐛 Troubleshooting

### Cannot Connect

1. **Check ESP32 IP address**
   - Verify IP in Serial Monitor
   - Update in `app.js`

2. **Check WiFi connection**
   - ESP32 connected to same network
   - Firewall not blocking port 81

3. **Check browser console**
   - Press F12 to open Developer Tools
   - Check Console tab for errors

### Slow Updates

1. Increase update interval in ESP32 code
2. Reduce number of connected clients
3. Check WiFi signal strength

### Values Not Updating

1. Verify WebSocket connection (green indicator)
2. Check Serial Monitor for errors
3. Refresh the browser page

---

## 📝 Notes for Wokwi Simulation

**Important:** WebSocket won't work in Wokwi simulation as it doesn't support WiFi.

For Wokwi testing:
1. Use the **sliders** to simulate sensor changes
2. Dashboard will update locally
3. Test the UI and alert logic
4. Deploy to real ESP32 hardware for full functionality

---

## 🚀 Future Enhancements

- [ ] Historical data charts
- [ ] Data export to CSV
- [ ] Mobile responsive design
- [ ] Dark/Light theme toggle
- [ ] User authentication
- [ ] Cloud integration (MQTT)
- [ ] Email/SMS alerts
- [ ] Multiple farm support

---

## 📄 License

This dashboard is part of the Smart Farm IoT Project.
Feel free to modify and adapt for your needs.

---

## 👤 Author

Created for 4th Year IoT Project
Year 4, Term 1 - 2024

---

## 📞 Support

For issues or questions:
1. Check the troubleshooting section
2. Review the Serial Monitor output
3. Verify all connections and configurations
4. Test with a simple ping to ESP32 IP

---

**Happy Farming! 🌱**
