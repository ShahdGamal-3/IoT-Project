# 🎮 Wokwi Simulation Guide - Smart Farm Monitoring System

## 📋 Overview
This project uses Wokwi for ESP32 simulation with 16 different sensors monitoring farm conditions.

---

## 🚀 How to Run the Simulation

### For Your Team Members:

#### Method 1: Run Locally with VS Code (Requires Setup)
1. **Clone the repository:**
   ```bash
   git clone https://github.com/ShahdGamal-3/IoT-Project.git
   cd IoT-Project
   ```

2. **Install Requirements:**
   - Install [VS Code](https://code.visualstudio.com/)
   - Install [PlatformIO Extension](https://platformio.org/install/ide?install=vscode)
   - Install [Wokwi Extension](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode)

3. **Build and Simulate:**
   - Open project in VS Code
   - Press `Ctrl+Shift+P` → Type "PlatformIO: Build"
   - Press `F1` → Type "Wokwi: Start Simulator"

4. **Interact with Sensors:**
   - Adjust potentiometers for different readings
   - Click motion sensor to trigger
   - Modify DHT22 temperature/humidity in properties
   - See results on 20x4 LCD display

---

#### Method 2: Upload to Wokwi.com (Recommended for Sharing)

**To create a shareable link, follow these steps:**

1. **Visit:** https://wokwi.com/new/esp32
2. **Copy the Arduino code** from `src/main.ino`
3. **Import diagram.json** (use the "Import Project" option)
4. **Add libraries** - Copy all `.h` and `.cpp` files from `include/` and `src/`
5. **Click Share** to get a public link like: `https://wokwi.com/projects/XXXXXX`

---

## 🎯 Sensors You Can Control in Simulation

### Interactive Sensors (Can Change During Simulation):

1. **Soil Moisture** - Potentiometer (pot1)
2. **Soil pH** - Potentiometer (pot2)  
3. **Leaf Wetness** - Potentiometer (pot3)
4. **Leaf Temperature** - Potentiometer (pot9)
5. **Air Temperature & Humidity** - DHT22 (click to edit properties)
6. **Light Intensity** - LDR (photoresistor)
7. **Wind Speed** - Potentiometer (pot4)
8. **Wind Direction** - Potentiometer (pot5)
9. **Rainfall** - Potentiometer (pot8)
10. **Water Tank Level** - HC-SR04 (can modify distance)
11. **Motion Detection** - PIR sensor (click to trigger)
12. **Weight** - HX711 with load cell
13. **Gas (MQ2)** - Simulated via code
14. **CO₂ (MQ135)** - Simulated via code
15. **CO (MQ7)** - Simulated via code

### LCD Display (20x4):
- **Line 1-4:** Shows different sensor values rotating every 2 seconds
- **Alert Indicators:** Shows warnings when values exceed thresholds

### Alert System:
- **Red LED:** Lights up when critical alerts are triggered
- **Buzzer:** Sounds alarm for dangerous conditions

---

## 📊 What Your Team Will See

### Normal Operation:
```
Soil Moist: 45.2%
Soil pH: 6.8
Air Temp: 25.5C
Humidity: 60%
```

### Alert Mode:
```
⚠️ ALERT: Soil Dry
⚠️ ALERT: High CO2
Water Level: Low
Motion Detected!
```

---

## 🎥 Alternative: Screen Recording

If you can't upload to Wokwi.com, share a screen recording:

### Windows (Built-in):
- Press `Win + G` to open Game Bar
- Click Record button
- Run simulation and demonstrate features
- Share video file or upload to YouTube/Google Drive

### OBS Studio (Free):
- Download: https://obsproject.com/
- Record screen while running simulation
- Export and share

---

## 📱 Quick Demo Link

**Live Dashboard (Web Browser):**
https://shahdgamal-3.github.io/IoT-Project/

**Features:**
- ✅ Simulates all 16 sensors
- ✅ Interactive sliders
- ✅ Real-time value updates
- ✅ Color-coded alerts
- ✅ Works on any device (no installation needed)

---

## 💡 Tips for Demonstration

1. **Start with normal values** - Show stable operation
2. **Trigger alerts** - Adjust sensors to extreme values
3. **Show LCD rotation** - Display cycles through sensor groups
4. **Demonstrate motion** - Click PIR sensor
5. **Test buzzer/LED** - Show alert system activation

---

## 📞 Support

For questions or issues:
- Check `README_COMPLETE.md` for full documentation
- Review `SENSOR_REFERENCE_TABLE.md` for pin configurations
- Contact: [Your Team Contact Info]

---

## 🔗 Links

- **GitHub Repository:** https://github.com/ShahdGamal-3/IoT-Project
- **Web Dashboard:** https://shahdgamal-3.github.io/IoT-Project/
- **Wokwi Platform:** https://wokwi.com

---

**Note:** The web dashboard provides the best sharing experience as it requires no installation and works on any device!
