# 🌾 Farm Monitoring System - Complete Implementation

## 📊 System Overview
**Version:** 2.0  
**Platform:** ESP32 DevKit V1  
**Development:** PlatformIO + Wokwi Simulator

---

## 🔧 Hardware Components

### **Agricultural Sensors (11 sensors)**
1. ✅ **Soil Moisture** - Capacitive/Resistive Probe (Potentiometer simulation)
2. ✅ **Soil Temperature** - DS18B20 Digital Sensor
3. ✅ **Soil pH** - pH Electrode (Potentiometer simulation)
4. ✅ **Leaf Temperature** - MLX90614 Infrared Sensor
5. ✅ **Leaf Wetness** - Resistive Plate (Potentiometer simulation)
6. ✅ **Air Temperature & Humidity** - DHT22 Sensor
7. ✅ **Light Intensity** - LDR Photoresistor
8. ✅ **Wind Speed** - Anemometer (PWM simulation with potentiometer control)
9. ✅ **Wind Direction** - Wind Vane (Potentiometer simulation)
10. ✅ **Rainfall** - Tipping Bucket Rain Gauge (Button simulation)
11. ✅ **Water Tank Level** - HC-SR04 Ultrasonic Sensor

### **Safety & Monitoring Sensors (4 sensors)**
12. ✅ **Gas Detection** - MQ2 Gas Sensor (Smoke, LPG, Methane)
13. ✅ **Motion Detection** - PIR Motion Sensor
14. ✅ **Weight Measurement** - HX711 Load Cell (Feed/Grain inventory)
15. ✅ **Alert System** - Active Buzzer (Audio warnings)

### **Display & Interface**
- 20x4 LCD Display with I2C interface
- Serial Monitor output (115200 baud)

---

## 📌 Pin Configuration

| Sensor | Pin(s) | Type |
|--------|--------|------|
| Soil Moisture | 34 | ADC1 |
| Soil Temperature | 15 | OneWire |
| Soil pH | 35 | ADC1 |
| Leaf Temperature | 21,22 | I2C |
| Leaf Wetness | 32 | ADC1 |
| DHT22 | 25 | Digital |
| LDR | 33 | ADC1 |
| Wind Speed (Input) | 27 | Digital (Interrupt) |
| Wind Speed (Simulator) | 26 | Digital (PWM) |
| Wind Speed (Control) | 36 | ADC1 |
| Wind Direction | 39 | ADC1 |
| Rain Gauge | 14 | Digital (Interrupt) |
| Water Tank (Trig) | 12 | Digital |
| Water Tank (Echo) | 13 | Digital |
| Gas Sensor | 34 | ADC1 (shared) |
| PIR Motion | 19 | Digital |
| HX711 (Data) | 5 | Digital |
| HX711 (Clock) | 18 | Digital |
| Buzzer | 23 | Digital |
| 20x4 LCD (SDA) | 21 | I2C |
| 20x4 LCD (SCL) | 22 | I2C |

---

## 📚 Libraries Used

```ini
LiquidCrystal_I2C       # 20x4 LCD Display
OneWire                 # DS18B20 Communication
DallasTemperature       # DS18B20 Driver
Adafruit MLX90614       # IR Temperature Sensor
DHT sensor library      # DHT22 Driver
HX711                   # Load Cell Amplifier
```

---

## 🖥️ 20x4 LCD Display Screens (14 Rotating Screens)

1. **Screen 1:** Soil Moisture + Soil Temperature
2. **Screen 2:** Soil pH + Leaf Temperature
3. **Screen 3:** Air Temperature + Humidity
4. **Screen 4:** Light Intensity
5. **Screen 5:** Wind Speed
6. **Screen 6:** Wind Direction
7. **Screen 7:** Rainfall
8. **Screen 8:** Water Tank Level
9. **Screen 9:** Gas Sensor
10. **Screen 10:** Motion Sensor
11. **Screen 11:** Weight Sensor
12. **Screen 12:** Leaf Wetness

**Display Rotation:** Every 4 seconds

---

## 🚨 Alert System

### **Alert Conditions:**
- 💧 **Low Water Tank** (< 25%)
- 🌱 **Low Soil Moisture** (< 20%)
- 💨 **Dangerous Gas Level** (> 3000 ppm)
- 👤 **Motion Detected**
- ⚖️ **Overweight** (> max capacity)

### **Alert Patterns:**
- Low Water: 2 beeps (200ms each)
- High/Low Temperature: 3 beeps (150ms each)
- Low Soil Moisture: 2 beeps (250ms each)
- Gas Detected: 5 rapid beeps (100ms each)
- Motion: 1 short beep
- Overweight: 4 beeps (200ms each)

---

## 📊 Sensor Data Output

### **Serial Monitor Format:**
```
========== SENSOR READINGS ==========
--- SOIL MOISTURE ---
--- SOIL TEMPERATURE ---
--- SOIL pH ---
--- LEAF TEMPERATURE ---
--- LEAF WETNESS ---
--- AIR TEMPERATURE & HUMIDITY ---
--- LIGHT INTENSITY ---
--- WIND SPEED ---
--- WIND DIRECTION ---
--- RAINFALL ---
--- WATER TANK LEVEL ---
--- GAS SENSOR ---
--- MOTION SENSOR ---
--- WEIGHT SENSOR ---
=====================================
```

**Update Interval:** Every 2 seconds

---

## 🎮 Wokwi Simulation Controls

| Component | Control | Description |
|-----------|---------|-------------|
| Soil Moisture | Potentiometer 1 | Adjust soil moisture (0-100%) |
| Soil pH | Potentiometer 2 | Adjust pH (0-14) |
| Leaf Wetness | Potentiometer 3 | Adjust wetness (0-100%) |
| Wind Speed | Potentiometer 4 | Control pulse frequency |
| Wind Direction | Potentiometer 5 | Set direction (0-360°) |
| Rainfall | Green Button | Simulate rain tip (0.28mm/tip) |
| Water Tank | HC-SR04 Distance | Adjust distance in simulator |
| Gas Level | MQ2 Sensor | Adjust gas concentration |
| Motion | PIR Sensor | Trigger motion in simulator |
| Weight | HX711 Load Cell | Adjust weight value |

---

## 🌟 Key Features

### **Real-time Monitoring:**
- Continuous sensor data acquisition
- Automatic 20x4 LCD screen rotation
- Serial monitor logging

### **Safety Features:**
- Multi-condition alert system
- Audio warnings via buzzer
- Gas detection with danger threshold
- Motion detection for security

### **Agricultural Insights:**
- Comprehensive soil analysis (moisture, temp, pH)
- Plant health monitoring (leaf temp & wetness)
- Weather conditions (wind, rain, light)
- Water resource management (tank level)
- Inventory tracking (weight measurement)

### **Smart Alerts:**
- Automatic threshold checking
- Priority-based alert system
- Mute/unmute functionality
- Visual + audio notifications

---

## 🚀 Getting Started

### **1. Hardware Setup:**
- Connect all sensors according to pin configuration
- Ensure proper power supply (5V for sensors, 3.3V for ESP32)
- Verify I2C connections for 20x4 LCD

### **2. Software Setup:**
```bash
# Install PlatformIO
# Clone/download project
# Open in VS Code with PlatformIO extension
pio lib install  # Install dependencies
pio run          # Build project
```

### **3. Wokwi Simulation:**
- Open diagram.json in Wokwi
- Click "Start Simulation"
- Interact with potentiometers and buttons
- Monitor serial output and 20x4 LCD display

---

## 📈 Future Enhancements

### **Potential Additions:**
- [ ] WiFi/Bluetooth connectivity
- [ ] Data logging to SD card
- [ ] Mobile app integration
- [ ] Cloud data upload (ThingSpeak, Firebase)
- [ ] Automated irrigation control
- [ ] Solar power integration
- [ ] Battery backup system
- [ ] GPS location tracking
- [ ] Camera module for visual monitoring
- [ ] Machine learning for predictive analytics

---

## 🛠️ Troubleshooting

### **Common Issues:**

**DHT22 Reading Failed:**
- Check wiring (VCC, GND, Data pin)
- Ensure proper power supply
- Add delay between readings

**HC-SR04 No Reading:**
- Verify Trig/Echo pin connections
- Check power supply (5V)
- Ensure clear line of sight

**HX711 Not Ready:**
- Check DT and SCK connections
- Calibrate load cell
- Perform tare operation

**I2C Device Not Found:**
- Verify I2C address (20x4 LCD: 0x27)
- Check SDA/SCL connections
- Use I2C scanner to detect devices

---

## 📝 License
MIT License - Free for educational and commercial use

## 👨‍💻 Developer
Farm Monitoring System v2.0  
Built with ❤️ for Smart Agriculture

---

## 📞 Support
For issues and questions, refer to:
- PlatformIO Documentation
- Wokwi Simulator Help
- ESP32 Arduino Core Documentation
- Individual sensor datasheets
