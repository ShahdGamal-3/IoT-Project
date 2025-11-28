# 🌾 Farm Monitoring System - Complete Sensor Reference Table

## 📊 Comprehensive Sensor Configuration

| # | **Parameter** | **Real Hardware Sensor** | **Wokwi Simulation** | **ESP32 Pin(s)** | **Purpose** | **Explanation** |
|---|--------------|------------------------|---------------------|-----------------|------------|-----------------|
| **1** | **Soil Moisture** | Capacitive/Resistive Soil Probe | Potentiometer (pot1) | **34** (ADC1) | Irrigation Management | Measures soil water content (0-100%) to optimize watering schedules and prevent over/under-watering |
| **2** | **Soil Temperature** | DS18B20 Digital Sensor | DS18B20 (temp1) | **15** (OneWire) | Root Zone Monitoring | Tracks soil thermal conditions affecting seed germination, root growth, and microbial activity |
| **3** | **Soil pH** | pH Electrode | Potentiometer (pot2) | **35** (ADC1) | Nutrient Availability | Monitors soil acidity/alkalinity (0-14 pH) to ensure optimal nutrient uptake for crops |
| **4** | **Leaf Temperature** | MLX90614 IR Sensor | MLX90614 (I2C) | **21/22** (I2C) | Plant Stress Detection | Non-contact infrared measurement detects plant water stress and disease before visible symptoms |
| **5** | **Leaf Wetness** | Resistive Leaf Wetness Plate | Potentiometer (pot3) | **32** (ADC1) | Disease Prevention | Detects moisture on leaf surfaces (0-100%) to predict fungal disease risk and spray timing |
| **6** | **Air Temperature** | DHT22 Sensor | DHT22 (dht1) | **25** (Digital) | Climate Monitoring | Measures ambient temperature (°C/°F) for crop stress assessment and ventilation control |
| **7** | **Relative Humidity** | DHT22 Sensor | DHT22 (dht1) | **25** (Digital) | Evapotranspiration | Tracks air moisture (0-100% RH) affecting plant transpiration and irrigation requirements |
| **8** | **Light Intensity** | LDR (Photoresistor) | LDR Sensor (ldr1) | **33** (ADC1) | Photosynthesis Tracking | Measures sunlight levels (lux) to assess growing conditions and supplemental lighting needs |
| **9** | **Wind Speed** | Anemometer (pulse output) | PWM Generator + Pot (pot4) | **27** (Input), **26** (PWM), **36** (Control) | Crop Protection | Monitors wind velocity (m/s) for spray drift prediction and mechanical damage prevention |
| **10** | **Wind Direction** | Wind Vane (resistance-based) | Potentiometer (pot5) | **39** (ADC1) | Microclimate Analysis | Tracks wind cardinal direction (N/NE/E/SE/S/SW/W/NW) for pesticide application and frost protection |
| **11** | **Rainfall** | Tipping Bucket Rain Gauge | Potentiometer (pot-rain) | **14** (ADC) | Water Budget | Measures precipitation (mm/inches) to adjust irrigation and track water availability |
| **12** | **Water Tank Level** | HC-SR04 Ultrasonic Sensor | HC-SR04 (ultrasonic1) | **12** (Trig), **13** (Echo) | Water Resource Management | Monitors storage tank volume (cm/%) using distance measurement for refill scheduling |
| **13** | **Gas Detection (MQ2)** | MQ2 Gas Sensor | Gas Sensor (gas1) | **4** (AOUT) | Safety & Combustion | Detects smoke, LPG, methane, propane (ppm) for fire hazards and biogas monitoring |
| **14** | **CO₂ Concentration** | MQ135 Sensor | Potentiometer (pot6) | **0** (ADC2) | Air Quality & Photosynthesis | Tracks carbon dioxide (400-5000 ppm) for greenhouse ventilation and plant growth optimization |
| **15** | **CO Concentration** | MQ7 Sensor | Potentiometer (pot7) | **2** (ADC2) | Worker Safety | Monitors carbon monoxide (0-1000 ppm) from combustion engines and heaters to prevent poisoning |
| **16** | **Motion Detection** | PIR Motion Sensor | PIR Sensor (pir1) | **19** (Digital) | Security & Wildlife | Detects human/animal movement for intrusion alerts and wildlife damage prevention |
| **17** | **Weight/Inventory** | HX711 Load Cell | HX711 (hx1) | **5** (DT), **18** (SCK) | Inventory Tracking | Measures feed, grain, or produce weight (kg/lbs) for supply management and yield tracking |
| **18** | **Alert System** | Active Buzzer | Buzzer (buzzer1) | **23** (PWM) | Audio Warnings | Sounds audible alerts for critical conditions (low water, high gas, motion) using LEDC PWM |

---

## 🔧 Pin Type Reference

| **Pin Type** | **Pins** | **Usage** | **Voltage** |
|-------------|----------|-----------|------------|
| **ADC1** | 32, 33, 34, 35, 36, 39 | Analog sensors (always safe) | 0-3.3V |
| **ADC2** | 0, 2, 4, 12, 13, 14, 15, 25, 26, 27 | Analog sensors (conflicts with WiFi) | 0-3.3V |
| **Digital GPIO** | 5, 14, 18, 19, 23, 25, 26, 27 | Digital I/O, PWM, interrupts | 3.3V logic |
| **I2C** | 21 (SDA), 22 (SCL) | I2C devices (LCD, MLX90614) | 3.3V |
| **OneWire** | 15 | DS18B20 temperature sensors | 3.3V + pull-up |

---

## 📈 Sensor Categories

### **🌱 Soil Health (3 sensors)**
- **Moisture, Temperature, pH** → Complete soil profile for precision agriculture

### **🌿 Plant Health (2 sensors)**
- **Leaf Temperature, Leaf Wetness** → Early disease detection and stress monitoring

### **🌤️ Environmental Conditions (5 sensors)**
- **Air Temp/Humidity, Light, Wind Speed/Direction, Rainfall** → Comprehensive weather station

### **💧 Water Management (1 sensor)**
- **Water Tank Level** → Storage monitoring for irrigation planning

### **⚠️ Safety Monitoring (3 sensors)**
- **Gas (MQ2), CO₂ (MQ135), CO (MQ7)** → Air quality and combustion safety

### **🔒 Security & Inventory (2 sensors)**
- **Motion (PIR), Weight (HX711)** → Theft prevention and stock management

### **🔊 Alert System (1 actuator)**
- **Buzzer** → Multi-pattern audio warnings for critical events

---

## 🎮 Wokwi Simulation Components

| **Component** | **Count** | **Description** |
|--------------|----------|-----------------|
| ESP32 DevKit V1 | 1 | Main microcontroller |
| LCD 20x4 I2C | 1 | Display module (address 0x27) |
| Potentiometers | 7 | Simulate analog sensors (soil moisture, pH, wetness, wind speed, wind direction, CO2, CO) |
| DS18B20 | 1 | Digital temperature sensor |
| DHT22 | 1 | Temperature & humidity sensor |
| LDR | 1 | Light sensor |
| HC-SR04 | 1 | Ultrasonic distance sensor |
| Gas Sensor | 1 | MQ2 gas detection |
| PIR Motion | 1 | Motion detector |
| HX711 + Load Cell | 1 | Weight measurement |
| Buzzer | 1 | Alert system |
| Resistor (4.7kΩ) | 1 | OneWire pull-up |

---

## 📊 Data Output Specifications

### **Update Intervals:**
- Sensor Reading: **2 seconds**
- LCD Screen Rotation: **4 seconds** (14 screens total)
- Alert Cooldown: **5 seconds**

### **Communication Protocols:**
- **I2C:** LCD (0x27), MLX90614 (0x5A)
- **OneWire:** DS18B20 temperature sensors
- **Analog:** 10 sensors via ADC (12-bit, 0-4095)
- **Digital:** PIR, Rain button, PWM simulator
- **Pulse Counting:** Wind speed (interrupt-based)
- **LEDC PWM:** Buzzer (channel 0, 2000 Hz)

### **Serial Monitor Output:**
- **Baud Rate:** 115200
- **Format:** Structured text with headers
- **Update:** Every 2 seconds with all sensor readings

---

## 🎯 Alert Threshold Table

| **Alert Type** | **Condition** | **Threshold** | **Beep Pattern** | **Priority** |
|---------------|--------------|---------------|-----------------|--------------|
| Low Water | Tank level < 25% | < 50 cm | 2 beeps × 200ms | High |
| Low Soil Moisture | Dry soil | < 20% | 2 beeps × 250ms | High |
| Dangerous Gas | Flammable gas detected | > 3000 ppm | 5 rapid beeps × 100ms | Critical |
| High CO₂ | Poor ventilation | > 2000 ppm | 5 rapid beeps × 100ms | Critical |
| High CO | Carbon monoxide poisoning risk | > 50 ppm | 5 rapid beeps × 100ms | Critical |
| Motion Detected | Intrusion/wildlife | PIR triggered | 1 short beep | Medium |
| Overweight | Load cell exceeded | > Max capacity | 4 beeps × 200ms | Medium |

---

## 🔌 Power Requirements

| **Component** | **Voltage** | **Current (typ)** | **Notes** |
|--------------|------------|------------------|-----------|
| ESP32 | 5V (USB) / 3.3V (logic) | 160-260 mA | Operating + WiFi (if enabled) |
| LCD 20x4 | 5V | 40-60 mA | Backlight on |
| DHT22 | 3.3-5V | 2.5 mA | Low power |
| DS18B20 | 3.3-5V | 1.5 mA | Parasitic mode available |
| HC-SR04 | 5V | 15 mA | During measurement |
| MQ Sensors | 5V | 150 mA | Heater element |
| PIR | 5V | 65 μA | Standby |
| HX711 | 3.3V | 1.5 mA | Load cell amplifier |
| Buzzer | 3.3V | 30 mA | PWM driven |
| **Total** | **5V @ 2A** | **~600-800 mA** | **USB power sufficient** |

---

## 🌟 Key Features Summary

✅ **16 Sensors** covering soil, plant, environmental, safety, and security  
✅ **Real-time Monitoring** with 2-second updates  
✅ **14 LCD Screens** rotating display  
✅ **7 Alert Types** with distinct beep patterns  
✅ **Multiple Protocols** (I2C, OneWire, ADC, Digital, PWM)  
✅ **Wokwi Compatible** - Full simulation support  
✅ **Production Ready** - Real hardware sensor specifications included  
✅ **Comprehensive Coverage** - Complete farm monitoring ecosystem  

---

## 📝 Usage Notes

### **For Wokwi Simulation:**
1. Open `diagram.json` in Wokwi simulator
2. Adjust potentiometers to simulate sensor values
3. Monitor Serial output (115200 baud) for detailed readings
4. Watch LCD cycle through 14 information screens

### **For Real Hardware Deployment:**
1. Replace potentiometers with actual sensors per specifications
2. Ensure proper power supply (5V @ 2A minimum)
3. Add pull-up resistors where needed (OneWire, I2C)
4. Calibrate sensors (especially HX711 load cell and pH probe)
5. Weatherproof outdoor sensors appropriately

### **Calibration Requirements:**
- **HX711 Weight:** Run tare and calibration with known weights
- **Soil pH:** 3-point calibration (pH 4, 7, 10)
- **Wind Speed:** Match pulse frequency to anemometer datasheet
- **Water Tank:** Measure actual tank height and adjust in code

---

## 📚 Additional Resources

- **Pin Diagrams:** See ESP32 DevKit V1 pinout reference
- **Sensor Datasheets:** Refer to manufacturer specifications
- **Wokwi Documentation:** https://docs.wokwi.com/
- **ESP32 API:** https://docs.espressif.com/

---

**Created:** November 2025  
**Platform:** ESP32 DevKit V1  
**Framework:** Arduino (PlatformIO)  
**Version:** 2.0 - Complete with 16 Sensors  

🌾 **Smart Agriculture Solution** 🚀
