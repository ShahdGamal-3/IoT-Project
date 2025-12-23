# 🌾 IoT Smart Farm Monitoring System

<div align="center">

![IoT](https://img.shields.io/badge/IoT-Enabled-blue?style=for-the-badge)
![ESP32](https://img.shields.io/badge/ESP32-Microcontroller-red?style=for-the-badge)
![Firebase](https://img.shields.io/badge/Firebase-Cloud-orange?style=for-the-badge)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Professional-green?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Production%20Ready-success?style=for-the-badge)

**A Comprehensive IoT Solution for Precision Agriculture and Farm Safety Monitoring**

[Features](#-features) • [Quick Start](#-quick-start) • [Architecture](#-system-architecture) • [Documentation](#-documentation) • [Hardware](#-hardware-components)

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [System Architecture](#-system-architecture)
- [Quick Start](#-quick-start)
- [Hardware Components](#-hardware-components)
- [Software Stack](#-software-stack)
- [Installation Guide](#-installation-guide)
- [Configuration](#-configuration)
- [Usage](#-usage)
- [Dashboard](#-dashboard)
- [Communication Protocols](#-communication-protocols)
- [Project Structure](#-project-structure)
- [Documentation](#-documentation)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)
- [License](#-license)

---

## 🌟 Overview

The **IoT Smart Farm Monitoring System** is a professional-grade, distributed IoT solution designed for modern precision agriculture. This system monitors 17+ critical parameters including soil health, plant conditions, environmental factors, air quality, and security metrics in real-time.

### Key Highlights

- **🎯 Precision Agriculture**: Data-driven farming decisions based on real-time sensor data
- **💧 Water Conservation**: Smart irrigation triggered by actual soil moisture levels
- **⚠️ Early Warning System**: Detect crop diseases, pest risks, and hazardous conditions early
- **👷 Worker Safety**: Monitor CO, CO₂, and hazardous gases for OSHA compliance
- **📊 Cloud Analytics**: Historical data analysis via Firebase for better planning
- **🌍 Remote Access**: Monitor and control your farm from anywhere in the world
- **💰 Cost Optimization**: Reduce water, fertilizer, and pesticide waste
- **🔒 Security**: Motion detection and alert system for intrusion prevention

### Why This System?

Traditional farming relies on scheduled irrigation and manual inspection, leading to:
- 30-50% water waste from over-irrigation
- Delayed disease detection resulting in crop loss
- Inefficient use of fertilizers and pesticides
- Worker safety risks in enclosed spaces

**Our Solution**: Real-time monitoring with automated alerts enables:
- ✅ Optimal resource utilization
- ✅ Early disease/pest detection
- ✅ Improved crop yields (15-30% increase reported)
- ✅ Enhanced worker safety
- ✅ Reduced operational costs

---

## ✨ Features

### 🔬 Comprehensive Sensor Coverage (17+ Sensors)

#### 🌱 Soil Health Monitoring
- **Soil Moisture** (Capacitive) - 0-100% for precision irrigation control
- **Soil pH** (Analog Probe) - pH 4.0-9.0 for nutrient availability assessment
- **Soil Temperature** (DS18B20) - ±0.5°C accuracy for root zone monitoring

#### 🍃 Plant Health & Disease Prediction
- **Leaf Wetness** (Resistive) - Fungal disease risk prediction
- **Leaf Temperature** (Analog) - Plant stress and transpiration monitoring
- **Light Intensity** (LDR) - Photosynthetic active radiation measurement

#### 🌤️ Environmental Monitoring
- **Air Temperature & Humidity** (DHT22) - ±0.5°C, ±2% RH accuracy
- **Wind Speed** (Anemometer) - 0-30 m/s for spray drift management
- **Wind Direction** (Wind Vane) - 0-360° for vector analysis
- **Rainfall** (Tipping Bucket) - Precipitation tracking for irrigation scheduling

#### 🏭 Air Quality & Safety
- **Hazardous Gas Detection** (MQ-2) - LPG, smoke, combustible gases (0-1000 ppm)
- **CO₂ Monitoring** (MQ-135) - Indoor air quality (400-5000 ppm)
- **CO Detection** (MQ-7) - Carbon monoxide safety (0-200 ppm, OSHA compliant)

#### 💧 Resources & Security
- **Water Tank Level** (HC-SR04) - Ultrasonic measurement (2-400 cm range)
- **Weight Scale** (HX711 + Load Cell) - Harvest/inventory tracking (±0.05% accuracy)
- **Motion Detection** (PIR) - Security and wildlife monitoring (3-7m range, 120° coverage)

### 🌐 Advanced Communication Architecture

- **ESP-NOW Protocol**: Ultra-low latency (<10ms) peer-to-peer communication between nodes
- **WiFi/HTTPS**: Secure cloud connectivity to Google Firebase
- **WebSocket**: Real-time bidirectional communication with web dashboard
- **Range**: 200-400m line-of-sight for ESP-NOW, global access via cloud

### 🎯 Intelligent Alert System

- **Multi-Level Alerts**: Critical, Warning, and Information classifications
- **Visual Indicators**: Color-coded LEDs (Red, Yellow, Green, Blue)
- **Audible Alarms**: Active buzzer for critical conditions
- **Cloud Notifications**: Real-time alerts pushed to dashboard
- **Configurable Thresholds**: Customize alert levels per crop/season

### 📱 Professional Web Dashboard

- **Real-Time Visualization**: Live sensor data updates via Firebase
- **Interactive Controls**: Simulation mode with sliders for testing
- **Historical Analytics**: Trend analysis and data logging
- **Responsive Design**: Optimized for desktop, tablet, and mobile
- **Offline Support**: Simulation mode works without hardware

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                    IOT SMART FARM MONITORING SYSTEM                  │
└─────────────────────────────────────────────────────────────────────┘

        ┌──────────────────────┐              ┌──────────────────────┐
        │   Soil Node (ESP32)  │              │ Weather Node (ESP32) │
        │  ───────────────────  │              │  ───────────────────  │
        │  📊 Soil Moisture     │              │  🍃 Leaf Wetness      │
        │  🌡️  Soil Temperature │              │  🌡️  Leaf Temperature │
        │  ⚗️  Soil pH          │              │  🌡️  Air Temp/Humidity│
        │                      │              │  ☀️  Light Intensity   │
        └──────────┬───────────┘              │  💨 Wind Speed/Dir    │
                   │                          │  🌧️  Rainfall          │
                   │                          └──────────┬────────────┘
                   │                                     │
                   │  ESP-NOW (2.4GHz)                   │  ESP-NOW
                   │  Range: 200m                        │  Range: 200m
                   │  Latency: <10ms                     │  Latency: <10ms
                   │                                     │
                   └──────────────┬──────────────────────┘
                                  │
                                  ▼
                   ┌──────────────────────────┐
                   │  Gateway Node (ESP32)    │
                   │  ──────────────────────   │
                   │  🌊 Water Tank Level     │
                   │  ⚠️  Gas Detection (MQ-2) │
                   │  🏭 CO₂ Monitor (MQ-135) │
                   │  ☠️  CO Monitor (MQ-7)    │
                   │  🚶 Motion Sensor (PIR)  │
                   │  ⚖️  Weight Scale (HX711) │
                   │  💾 Data Aggregator      │
                   │  📺 LCD Display (20x4)   │
                   │  🚨 Alert System         │
                   └──────────┬───────────────┘
                              │
                              │ WiFi/HTTPS
                              │ Firebase REST API
                              │ TLS 1.2/1.3
                              │
                   ┌──────────▼──────────────┐
                   │   Firebase Cloud        │
                   │   Realtime Database     │
                   │  ─────────────────────   │
                   │  ☁️  Data Storage        │
                   │  🔐 Authentication       │
                   │  📡 Real-time Sync       │
                   │  📊 Historical Analytics │
                   └──────────┬──────────────┘
                              │
                              │ WebSocket/REST
                              │ Live Updates
                              │
                   ┌──────────▼──────────────┐
                   │   Web Dashboard         │
                   │   (HTML/CSS/JavaScript) │
                   │  ─────────────────────   │
                   │  📊 Data Visualization   │
                   │  🔔 Alert Management     │
                   │  📈 Historical Trends    │
                   │  🎮 Interactive Controls │
                   └─────────────────────────┘
```

### Data Flow

```
Sensor Nodes → ESP-NOW (5s interval) → Gateway Node → Firebase (30s interval) → Web Dashboard (real-time)
```

---

## 🚀 Quick Start

### Prerequisites

1. **Hardware Requirements**:
   - 3x ESP32 Development Boards
   - All sensors listed in [Hardware Components](#-hardware-components)
   - USB cables and power supplies
   - Breadboards or custom PCBs

2. **Software Requirements**:
   - [Python 3.7+](https://www.python.org/downloads/)
   - [PlatformIO](https://platformio.org/)
   - [Git](https://git-scm.com/)
   - ESP32 USB drivers
   - Modern web browser (Chrome, Firefox, Edge, Safari)

3. **Cloud Requirements**:
   - Google Firebase account (free tier sufficient)
   - Internet connection for cloud features

### Installation in 5 Minutes

```powershell
# 1. Clone the repository
git clone https://github.com/ShahdGamal-3/IoT-Project.git
cd IoT-Project

# 2. Install PlatformIO
pip install platformio

# 3. Configure Firebase
# - Copy your Firebase credentials to gateway_node/include/config.h
# - Update FIREBASE_HOST and FIREBASE_AUTH

# 4. Build firmware for all nodes
cd esp32_nodes
.\build_all.ps1

# 5. Upload to ESP32 boards (update COM ports first)
.\upload_all.ps1

# 6. Open dashboard
cd ..\dashboard
# Open index.html in your web browser
```

### First Boot Checklist

1. ✅ Gateway node connects to WiFi (check LCD display)
2. ✅ Gateway node connects to Firebase (check serial monitor)
3. ✅ Soil and Weather nodes send data via ESP-NOW
4. ✅ Dashboard shows "Connected" status
5. ✅ Sensor values update in real-time

---

## 🔧 Hardware Components

### Node 1: Soil Monitoring Node

| Component | Model/Type | Pin | Range/Accuracy | Power |
|-----------|------------|-----|----------------|-------|
| Microcontroller | ESP32 DevKit | - | 240MHz Dual-Core | 5V/200mA |
| Soil Moisture | Capacitive Sensor | GPIO 34 | 0-100%, ±2% | 3.3V |
| Soil pH | Analog pH Meter | GPIO 35 | pH 4-9, ±0.1 | 5V |
| Soil Temperature | DS18B20 Digital | GPIO 15 | -55°C to +125°C, ±0.5°C | 3.3V |

**Total Power Consumption**: ~200mA @ 5V (1W)

### Node 2: Weather Monitoring Node

| Component | Model/Type | Pin | Range/Accuracy | Power |
|-----------|------------|-----|----------------|-------|
| Microcontroller | ESP32 DevKit | - | 240MHz Dual-Core | 5V/250mA |
| Air Temp/Humidity | DHT22 | GPIO 25 | -40°C to +80°C, 0-100% RH | 3.3V |
| Leaf Wetness | Resistive Sensor | GPIO 32 | 0-100% | 3.3V |
| Leaf Temperature | Analog Sensor | GPIO 35 | -10°C to +50°C | 3.3V |
| Light Sensor | LDR (Photoresistor) | GPIO 33 | 0-1000 lux | 3.3V |
| Wind Speed | Anemometer | GPIO 36 | 0-30 m/s | 3.3V |
| Wind Direction | Wind Vane | GPIO 39 | 0-360° | 3.3V |
| Rain Gauge | Tipping Bucket | GPIO 39 | 0-100 mm/hour | 3.3V |

**Total Power Consumption**: ~250mA @ 5V (1.25W)

### Node 3: Gateway & Safety Node

| Component | Model/Type | Pin | Range/Accuracy | Power |
|-----------|------------|-----|----------------|-------|
| Microcontroller | ESP32 DevKit | - | 240MHz Dual-Core | 5V/300mA |
| LCD Display | 20x4 I2C | I2C (21,22) | Blue backlight, 0x27 | 5V/80mA |
| Water Level | HC-SR04 Ultrasonic | 12(Trig), 13(Echo) | 2-400cm, ±3mm | 5V |
| Gas Sensor | MQ-2 | GPIO 35 | 0-1000 ppm | 5V/150mA |
| CO₂ Sensor | MQ-135 | GPIO 32 | 400-5000 ppm | 5V/150mA |
| CO Sensor | MQ-7 | GPIO 33 | 0-200 ppm | 5V/150mA |
| Motion Sensor | PIR HC-SR501 | GPIO 19 | 3-7m, 120° | 5V/50mA |
| Load Cell + ADC | HX711 + 50kg Cell | 25(DT), 18(SCK) | 0-50kg, 24-bit | 5V |
| Buzzer | Active Buzzer | GPIO 23 | 1kHz tone | 5V/30mA |
| Status LEDs | 5mm LEDs | 16,17,26,27 | Soil, Gas, Motion, OK | 3.3V |

**Total Power Consumption**: ~500mA @ 5V (2.5W) with all MQ sensors active

**System Total**: ~5W (all 3 nodes)

---

## 💻 Software Stack

### Embedded Firmware

- **IDE**: [PlatformIO](https://platformio.org/) (Professional IoT Development)
- **Framework**: Arduino Framework for ESP32
- **Language**: C/C++ (ISO C++11)
- **SDK**: Espressif ESP-IDF v4.4+
- **Toolchain**: Xtensa GCC Compiler
- **Flash Size**: 4MB (ESP32 standard)
- **OTA Support**: Over-The-Air updates ready

### Key Libraries

```ini
# Gateway Node
Firebase ESP32 Client @ ^4.4.14    # Cloud communication
HX711 @ ^0.7.5                     # Load cell interface
LiquidCrystal_I2C @ ^1.1.4         # LCD display

# Soil Node
OneWire @ ^2.3.7                   # DS18B20 protocol
DallasTemperature @ ^3.11.0        # Temperature sensor library

# Weather Node
DHT sensor library @ ^1.4.4        # DHT22 interface
Adafruit Unified Sensor @ ^1.1.14  # Sensor abstraction
```

### Cloud Platform

- **Provider**: Google Firebase (Real-time Database)
- **Authentication**: Firebase Auth with API keys
- **Database**: NoSQL JSON structure
- **API**: REST API with HTTPS (TLS 1.2/1.3)
- **SDK**: Firebase JavaScript SDK v9.22.0
- **Security**: Firebase Security Rules configured

### Web Dashboard

- **Frontend**: HTML5, CSS3, JavaScript ES6+
- **Layout**: CSS Grid & Flexbox (Responsive)
- **Icons**: Font Awesome 6.4.0
- **Real-time**: WebSocket for live updates
- **Compatibility**: Modern browsers (Chrome 90+, Firefox 88+, Safari 14+, Edge 90+)
- **Hosting**: Static files (Firebase Hosting optional)

---

## 📥 Installation Guide

### Step 1: Clone Repository

```powershell
git clone https://github.com/ShahdGamal-3/IoT-Project.git
cd IoT-Project
```

### Step 2: Install Development Tools

```powershell
# Install Python (if not already installed)
# Download from: https://www.python.org/downloads/

# Install PlatformIO Core
pip install platformio

# Verify installation
pio --version
```

### Step 3: Install ESP32 USB Drivers

- **Windows**: [CP210x Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
- **macOS**: Usually pre-installed
- **Linux**: Usually pre-installed (add user to dialout group)

### Step 4: Configure Firebase

1. Create a Firebase project at [console.firebase.google.com](https://console.firebase.google.com)
2. Enable Realtime Database
3. Copy your credentials
4. Edit `esp32_nodes/gateway_node/include/config.h`:

```cpp
// WiFi Configuration
#define WIFI_SSID "Your_WiFi_Name"
#define WIFI_PASSWORD "Your_WiFi_Password"

// Firebase Configuration
#define FIREBASE_HOST "your-project.firebaseio.com"
#define FIREBASE_AUTH "your-database-secret-or-token"
```

5. Update `dashboard/app.js` with Firebase config:

```javascript
const firebaseConfig = {
  apiKey: "your-api-key",
  authDomain: "your-project.firebaseapp.com",
  databaseURL: "https://your-project.firebaseio.com",
  projectId: "your-project-id",
  storageBucket: "your-project.appspot.com",
  messagingSenderId: "123456789",
  appId: "your-app-id"
};
```

### Step 5: Build Firmware

```powershell
# Navigate to ESP32 nodes directory
cd esp32_nodes

# Build all nodes at once
.\build_all.ps1

# OR build individually
cd gateway_node
pio run

cd ../soil_node
pio run

cd ../weather_node
pio run
```

### Step 6: Upload to ESP32 Boards

```powershell
# Find your COM ports
mode

# Update COM ports in upload script, then:
.\upload_all.ps1

# OR upload individually
cd gateway_node
pio run --target upload --upload-port COM3

cd ../soil_node
pio run --target upload --upload-port COM4

cd ../weather_node
pio run --target upload --upload-port COM5
```

### Step 7: Monitor Serial Output

```powershell
# Monitor gateway node
.\monitor.ps1 -Node gateway

# Monitor soil node
.\monitor.ps1 -Node soil

# Monitor weather node
.\monitor.ps1 -Node weather
```

### Step 8: Access Dashboard

```powershell
# Option 1: Direct file access
cd dashboard
# Open index.html in your browser

# Option 2: Local HTTP server (recommended)
python -m http.server 8080
# Navigate to: http://localhost:8080
```

---

## ⚙️ Configuration

### WiFi Configuration

Edit `esp32_nodes/gateway_node/include/config.h`:

```cpp
#define WIFI_SSID "Your_Network_Name"
#define WIFI_PASSWORD "Your_Network_Password"
```

### Sensor Calibration

#### Soil Moisture Calibration
```cpp
// In soil_node.cpp
#define MOISTURE_DRY_VALUE 3200    // ADC value in air
#define MOISTURE_WET_VALUE 1200    // ADC value in water
```

#### pH Sensor Calibration
```cpp
// Use pH 4.0, 7.0, and 10.0 buffer solutions
#define PH_4_VOLTAGE 2.6
#define PH_7_VOLTAGE 2.0
#define PH_10_VOLTAGE 1.5
```

#### Weight Scale Calibration
```cpp
// In gateway_node.cpp
scale.set_scale(420.0983);  // Calibration factor
scale.tare();               // Reset to zero
```

### Alert Thresholds

Edit `gateway_node/src/gateway_node.cpp`:

```cpp
// Soil conditions
#define MOISTURE_LOW_THRESHOLD 30.0     // Irrigation needed
#define MOISTURE_HIGH_THRESHOLD 80.0    // Over-watered

// Air quality (OSHA standards)
#define CO_DANGER_THRESHOLD 50.0        // ppm
#define CO2_WARNING_THRESHOLD 1000.0    // ppm
#define GAS_DANGER_THRESHOLD 500.0      // ppm

// Plant health
#define LEAF_WETNESS_WARNING 60.0       // Disease risk
```

### Communication Settings

```cpp
// ESP-NOW Channel (must match on all nodes)
#define ESPNOW_CHANNEL 1

// Firebase update interval
#define FIREBASE_UPDATE_INTERVAL 30000  // 30 seconds

// Sensor read interval
#define SENSOR_READ_INTERVAL 5000       // 5 seconds
```

---

## 📖 Usage

### Normal Operation

1. **Power On**: Connect all ESP32 nodes to power
2. **Wait for Boot**: Gateway LCD shows "Initializing..." then WiFi status
3. **Verify Connection**: 
   - LCD shows "WiFi Connected"
   - LCD shows "Firebase OK"
   - Serial monitor shows "ESP-NOW Init Success"
4. **Monitor Dashboard**: Open web dashboard to see live data
5. **Check Alerts**: Status LEDs indicate system health

### LED Status Indicators

| LED Color | Location | Meaning |
|-----------|----------|---------|
| 🔵 Blue | GPIO 27 | System OK - All sensors normal |
| 🟢 Green | GPIO 16 | Soil conditions optimal |
| 🟡 Yellow | GPIO 17 | Warning - Attention needed |
| 🔴 Red | GPIO 26 | Critical alert - Immediate action required |

### LCD Display Format

```
Line 1: System Status
Line 2: Soil: M:65% T:22C
Line 3: Air: T:25C H:60%
Line 4: Gas:OK Water:75%
```

### Serial Monitor Commands

Connect to gateway node at 115200 baud:

```
[INFO] System initialized
[ESPNOW] Soil data received: M=65.2%, T=22.1C, pH=6.8
[ESPNOW] Weather data received: T=25.3C, H=58.2%
[FIREBASE] Upload successful
[ALERT] Low soil moisture detected!
```

### Simulation Mode (No Hardware)

1. Open `dashboard/index.html` in browser
2. Use sliders to simulate sensor values
3. Test alert thresholds and UI behavior
4. Perfect for demonstrations and development

---

## 📊 Dashboard

### Features

- **Real-Time Updates**: Live sensor data via Firebase/WebSocket
- **Interactive Controls**: Sliders for simulation and testing
- **Color-Coded Status**: Instant visual assessment
- **Alert Management**: Critical condition notifications
- **Historical View**: Trend analysis (coming soon)
- **Mobile Responsive**: Works on all screen sizes

### Dashboard Sections

1. **🌱 Soil Monitoring**: Moisture, Temperature, pH
2. **🍃 Plant Health**: Leaf temperature, wetness
3. **🌤️ Climate**: Air temp, humidity, light, rainfall
4. **💨 Wind**: Speed and direction with compass
5. **🏭 Air Quality**: Gas, CO₂, CO levels
6. **💧 Resources**: Water tank level, weight
7. **🔒 Security**: Motion detection status

### Accessing the Dashboard

```powershell
# Method 1: Direct file (local filesystem)
cd dashboard
# Double-click index.html or drag to browser

# Method 2: HTTP Server (recommended for development)
python -m http.server 8080
# Open: http://localhost:8080

# Method 3: Firebase Hosting (production)
firebase deploy --only hosting
# Access: https://your-project.web.app
```

### Dashboard Configuration

Edit `dashboard/app.js`:

```javascript
// Simulation mode (testing without ESP32)
const SIMULATION_MODE = true;  // Set to false for real hardware

// ESP32 IP address (for WebSocket mode)
const ESP32_IP = '192.168.1.100';

// Update intervals
const UPDATE_INTERVAL = 2000;  // milliseconds
```

---

## 📡 Communication Protocols

### ESP-NOW (Sensor Nodes ↔ Gateway)

**Advantages**:
- ✅ No WiFi router required
- ✅ Ultra-low latency (<10ms)
- ✅ Long range (200m line-of-sight)
- ✅ Low power consumption
- ✅ Direct peer-to-peer communication

**Configuration**:
```cpp
// Both nodes must be on same channel
esp_now_peer_info_t peerInfo;
memcpy(peerInfo.peer_addr, gatewayAddress, 6);
peerInfo.channel = 1;  // Must match
peerInfo.encrypt = false;  // Optional encryption
```

**Data Structures**:
```cpp
typedef struct {
  char nodeId[20];
  float soilMoisture;
  float soilPH;
  float soilTemp;
  unsigned long timestamp;
} soil_data;
```

### WiFi/HTTPS (Gateway → Firebase)

**Advantages**:
- ✅ Secure transmission (TLS 1.2/1.3)
- ✅ Global accessibility
- ✅ Cloud storage and analytics
- ✅ Multi-device access

**Implementation**:
```cpp
Firebase.setFloat(firebaseData, "/sensors/soil/moisture", moistureValue);
Firebase.setFloat(firebaseData, "/sensors/soil/temperature", tempValue);
```

### WebSocket (Dashboard ↔ ESP32) - Optional

For direct local communication without cloud:

```javascript
const ws = new WebSocket('ws://192.168.1.100:81');
ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  updateDashboard(data);
};
```

---

## 📁 Project Structure

```
IoT-Project/
├── 📄 README.md                    # This file - Main documentation
├── 📄 platformio.ini               # Legacy PlatformIO config
├── 📄 wokwi.toml                   # Wokwi simulator config
├── 📄 diagram.json                 # Wokwi circuit diagram
├── 📄 libraries.txt                # Required libraries list
│
├── 📁 esp32_nodes/                 # ESP32 firmware (3 nodes)
│   ├── 📄 README.md                # Node setup guide
│   ├── 📄 ARCHITECTURE.md          # System architecture details
│   ├── 📄 build_all.ps1            # Build all nodes script
│   ├── 📄 upload_all.ps1           # Upload all nodes script
│   ├── 📄 monitor.ps1              # Serial monitor script
│   │
│   ├── 📁 gateway_node/            # Master node + safety sensors
│   │   ├── 📄 platformio.ini
│   │   ├── 📁 src/
│   │   │   └── gateway_node.cpp    # Main gateway firmware
│   │   └── 📁 include/
│   │       ├── config.h            # WiFi + Firebase credentials
│   │       └── data_structures.h   # ESP-NOW data structures
│   │
│   ├── 📁 soil_node/               # Soil monitoring node
│   │   ├── 📄 platformio.ini
│   │   ├── 📁 src/
│   │   │   └── soil_node.cpp       # Soil sensor firmware
│   │   └── 📁 include/
│   │       └── config.h            # Node configuration
│   │
│   └── 📁 weather_node/            # Weather monitoring node
│       ├── 📄 platformio.ini
│       ├── 📁 src/
│       │   └── weather_node.cpp    # Weather sensor firmware
│       └── 📁 include/
│           └── config.h            # Node configuration
│
├── 📁 dashboard/                   # Web dashboard
│   ├── 📄 index.html               # Main dashboard HTML
│   ├── 📄 styles.css               # Dashboard styling
│   ├── 📄 app.js                   # Dashboard logic + Firebase
│   ├── 📄 serial_bridge.py         # Serial-to-WebSocket bridge
│   ├── 📄 find_com_port.py         # Auto-detect ESP32 COM port
│   └── 📄 README_DASHBOARD.md      # Dashboard documentation
│
├── 📁 include/                     # Sensor library headers (legacy)
│   ├── 📄 SoilMoistureSensor.h
│   ├── 📄 DHTSensor.h
│   ├── 📄 CO2Sensor.h
│   └── ... (15+ sensor headers)
│
├── 📁 src/                         # Sensor library implementations (legacy)
│   ├── 📄 SoilMoistureSensor.cpp
│   ├── 📄 DHTSensor.cpp
│   └── ... (15+ sensor implementations)
│
├── 📁 merged_wokwi/                # Wokwi simulation version
│   ├── 📄 smart_farm_merged.ino    # All-in-one simulation code
│   ├── 📄 diagram.json             # Wokwi circuit
│   └── 📄 QUICK_START.md           # Simulation guide
│
└── 📁 wokwi-upload/                # Wokwi ready-to-upload version
    ├── 📄 sketch.ino
    ├── 📄 diagram.json
    └── 📄 LIBRARIES_INFO.md
```

---

## 📚 Documentation

Comprehensive documentation is available in the repository:

### Quick Start Guides
- **[QUICK_START_WOKWI.md](QUICK_START_WOKWI.md)** - Wokwi simulator setup
- **[WOKWI_SIMULATION_GUIDE.md](WOKWI_SIMULATION_GUIDE.md)** - Detailed simulation instructions
- **[esp32_nodes/README.md](esp32_nodes/README.md)** - ESP32 firmware guide

### Technical Documentation
- **[README_PROJECT.md](README_PROJECT.md)** - Comprehensive project details
- **[ARCHITECTURE.md](esp32_nodes/ARCHITECTURE.md)** - System architecture
- **[FIRMWARE_STRUCTURE.md](esp32_nodes/FIRMWARE_STRUCTURE.md)** - Code organization
- **[SENSOR_REFERENCE_TABLE.md](SENSOR_REFERENCE_TABLE.md)** - Sensor specifications

### Dashboard & Frontend
- **[dashboard/README_DASHBOARD.md](dashboard/README_DASHBOARD.md)** - Dashboard setup
- **[dashboard/README.md](dashboard/README.md)** - Dashboard features

### Diagrams & Visual Guides
- **[DIAGRAMS.md](esp32_nodes/DIAGRAMS.md)** - System diagrams
- **diagram.json** - Wokwi circuit diagrams

---

## 🔧 Troubleshooting

### ESP32 Won't Upload

```powershell
# Check COM port
mode

# Hold BOOT button during upload
pio run --target upload --upload-port COMX

# Erase flash if corrupted
esptool.py --port COMX erase_flash
```

### WiFi Connection Issues

```cpp
// Increase connection timeout
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
int timeout = 30;  // 30 seconds
while (WiFi.status() != WL_CONNECTED && timeout > 0) {
  delay(1000);
  timeout--;
}
```

### ESP-NOW Communication Failure

```cpp
// Verify MAC addresses match
uint8_t gatewayMAC[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};

// Check channel consistency
WiFi.channel();  // Must be same on all nodes

// Verify callback registration
esp_now_register_recv_cb(OnDataRecv);
```

### Firebase Connection Issues

1. **Verify credentials** in `config.h`
2. **Check database rules** (set to public for testing)
3. **Verify internet connection**
4. **Check Firebase quota** (free tier limits)

```json
// Firebase rules (for testing - NOT for production!)
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

### Dashboard Not Updating

1. **Check browser console** for errors (F12)
2. **Verify Firebase config** in `app.js`
3. **Test simulation mode** first
4. **Clear browser cache**

### Sensor Reading Issues

```cpp
// Enable debug output
#define DEBUG_MODE 1
Serial.println("Sensor value: " + String(sensorValue));

// Check voltage levels
Serial.println("Analog reading: " + String(analogRead(SENSOR_PIN)));

// Verify pin mode
pinMode(SENSOR_PIN, INPUT);
```

### Common Error Messages

| Error | Cause | Solution |
|-------|-------|----------|
| `ESPNOW Init Failed` | WiFi not initialized | Call `WiFi.mode(WIFI_STA)` first |
| `Firebase connection timeout` | Wrong credentials | Check FIREBASE_HOST and FIREBASE_AUTH |
| `Sensor out of range` | Wiring issue | Verify connections and power supply |
| `Upload failed` | COM port busy | Close serial monitor before upload |

---

## 🤝 Contributing

We welcome contributions! Here's how you can help:

### Reporting Issues

1. Check existing issues first
2. Provide detailed description
3. Include error messages and logs
4. Specify your hardware/software versions

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Development Guidelines

- **Code Style**: Follow existing formatting (2-space indentation)
- **Comments**: Document complex logic
- **Testing**: Test on actual hardware before PR
- **Documentation**: Update README for new features

### Areas for Contribution

- 🌟 Additional sensor support
- 📱 Mobile app development
- 📊 Advanced analytics/ML models
- 🌍 Internationalization (i18n)
- 🔒 Enhanced security features
- ⚡ Power optimization
- 📖 Documentation improvements

---

## 📧 Support & Contact

- **GitHub Issues**: [Report bugs or request features](https://github.com/ShahdGamal-3/IoT-Project/issues)
- **Discussions**: [Ask questions and share ideas](https://github.com/ShahdGamal-3/IoT-Project/discussions)
- **Email**: shahdgamal3@example.com (replace with actual contact)

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2025 Shahd Gamal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 🙏 Acknowledgments

- **Espressif Systems** - ESP32 development platform
- **Google Firebase** - Cloud infrastructure
- **PlatformIO** - Professional IoT development environment
- **Wokwi** - Online ESP32 simulator
- **Arduino Community** - Extensive library ecosystem
- **Open Source Contributors** - All the amazing libraries used

---

## 🌟 Star History

If you find this project helpful, please consider giving it a ⭐ on GitHub!

[![Star History Chart](https://api.star-history.com/svg?repos=ShahdGamal-3/IoT-Project&type=Date)](https://star-history.com/#ShahdGamal-3/IoT-Project&Date)

---

## 📊 Project Statistics

- **Lines of Code**: ~5,000+
- **Sensors Supported**: 17+
- **Communication Protocols**: 3 (ESP-NOW, WiFi, WebSocket)
- **Nodes**: 3 (Soil, Weather, Gateway)
- **Real-time Updates**: <30 seconds latency
- **Power Consumption**: ~5W total system
- **Range**: 200-400m (ESP-NOW), Global (Cloud)
- **Development Time**: 4 months
- **Testing**: Wokwi simulation + Real hardware

---

## 🔮 Future Roadmap

### Version 2.0 (Planned Features)

- [ ] **Machine Learning**: Crop disease prediction using TensorFlow Lite
- [ ] **Mobile App**: Native iOS/Android application
- [ ] **Solar Power**: Battery + solar panel integration
- [ ] **LoRaWAN**: Long-range communication for remote areas
- [ ] **Automated Control**: Servo-controlled irrigation valves
- [ ] **Camera Integration**: ESP32-CAM for visual monitoring
- [ ] **Weather Forecast**: Integration with OpenWeatherMap API
- [ ] **SMS Alerts**: Twilio integration for critical notifications
- [ ] **Multi-Farm Support**: Dashboard for multiple farm locations
- [ ] **Data Export**: CSV/Excel export for offline analysis

### Version 3.0 (Future Vision)

- [ ] **AI Recommendations**: Smart farming advice based on data
- [ ] **Drone Integration**: Aerial monitoring and pesticide application
- [ ] **Blockchain**: Supply chain traceability
- [ ] **VR/AR**: Virtual farm monitoring experience
- [ ] **Edge Computing**: On-device ML inference

---

<div align="center">

**Made with ❤️ for Sustainable Agriculture**

[⬆ Back to Top](#-iot-smart-farm-monitoring-system)

</div>