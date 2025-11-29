# 📚 Libraries Required for Wokwi.com

## ✅ Complete List of Libraries

Your `libraries.txt` file contains all the required libraries:

```
OneWire
DallasTemperature
LiquidCrystal I2C
DHT sensor library
HX711
```

---

## 🔧 What Each Library Does

### 1. **OneWire**
- **Purpose:** Communication protocol for DS18B20 temperature sensors
- **Used for:** Soil Temperature Sensor
- **Wokwi:** Auto-installed

### 2. **DallasTemperature**
- **Purpose:** Interface with Dallas/Maxim DS18B20 temperature sensors
- **Used for:** Soil Temperature Sensor (simplifies OneWire usage)
- **Wokwi:** Auto-installed

### 3. **LiquidCrystal I2C**
- **Purpose:** Control I2C LCD displays
- **Used for:** 20x4 LCD Display
- **Wokwi:** Auto-installed

### 4. **DHT sensor library**
- **Purpose:** Read DHT22 temperature and humidity sensor
- **Used for:** Air Temperature & Humidity Sensor
- **Wokwi:** Auto-installed

### 5. **HX711**
- **Purpose:** Interface with HX711 load cell amplifier
- **Used for:** Weight Sensor
- **Wokwi:** Auto-installed

---

## 🚀 How to Use in Wokwi.com

### Option 1: Automatic (Recommended)
Wokwi automatically detects and installs libraries when you:
1. Paste your code
2. Start the simulation
3. **No manual action needed!** ✨

### Option 2: Manual (If Needed)
If libraries don't auto-install:

1. **Click Library Manager** (book icon in left sidebar)
2. **Search and add each library:**
   - Type library name
   - Click "Add to project"
   - Repeat for all 5 libraries

---

## 📝 For Local Development (VS Code/PlatformIO)

Your `platformio.ini` already has the libraries configured:

```ini
lib_deps = 
    paulstoffregen/OneWire@^2.3.7
    milesburton/DallasTemperature@^3.11.0
    marcoschwartz/LiquidCrystal_I2C@^1.1.4
    adafruit/DHT sensor library@^1.4.4
    bogde/HX711@^0.7.5
```

These are automatically downloaded when you build the project.

---

## ✅ Verification Checklist

Before uploading to Wokwi:

- [x] `sketch.ino` - Complete code ✅
- [x] `diagram.json` - Circuit diagram ✅
- [x] `libraries.txt` - Library list ✅
- [ ] Upload to Wokwi.com
- [ ] Start simulation
- [ ] Get shareable link

---

## 🎯 Quick Reference

### Files in `wokwi-upload/` folder:
1. ✅ **sketch.ino** - Main code
2. ✅ **diagram.json** - Circuit
3. ✅ **libraries.txt** - Dependencies
4. ✅ **README.md** - Instructions

### Ready to upload!
Go to: https://wokwi.com/new/esp32

---

## 📚 Library Documentation

- **OneWire:** https://github.com/PaulStoffregen/OneWire
- **DallasTemperature:** https://github.com/milesburton/Arduino-Temperature-Control-Library
- **LiquidCrystal_I2C:** https://github.com/johnrickman/LiquidCrystal_I2C
- **DHT:** https://github.com/adafruit/DHT-sensor-library
- **HX711:** https://github.com/bogde/HX711

---

**All libraries are ready and configured! 🎉**
