# 🚀 Wokwi.com Upload Package

## Files in this folder:
1. **sketch.ino** - Complete single-file Arduino code (all sensors merged)
2. **diagram.json** - Circuit diagram with all connections

---

## 📋 How to Upload to Wokwi.com and Get Shareable Link

### Step 1: Go to Wokwi.com
Open this link in your browser:
```
https://wokwi.com/new/esp32
```

### Step 2: Replace the Code
1. You'll see a default ESP32 project with `sketch.ino`
2. **Delete all the default code**
3. Open `sketch.ino` from this folder
4. **Copy ALL the content** (Ctrl+A, Ctrl+C)
5. **Paste into Wokwi editor** (Ctrl+V)

### Step 3: Import Circuit Diagram
1. In Wokwi, click the **blue "+" button** (top left, near the diagram area)
2. Select "Import diagram.json"
3. Click "Choose File"
4. Select the `diagram.json` file from this folder
5. Click "Import"

Your complete circuit with all 16 sensors will appear!

### Step 4: Install Required Libraries
Wokwi will automatically detect and install:
- Wire (built-in)
- LiquidCrystal_I2C
- OneWire
- DallasTemperature
- DHT sensor library
- HX711

(This happens automatically when you start the simulation)

### Step 5: Start Simulation
1. Click the **green "Start Simulation" button** (top bar)
2. Wait 5-10 seconds for compilation
3. Watch your sensors come alive!

### Step 6: Get Your Shareable Link
1. Once simulation is running, click the **"Share" button** (top right)
2. You'll get a link like: `https://wokwi.com/projects/YOUR_PROJECT_ID`
3. **Copy this link** and share it with your team!

---

## 🎮 How to Interact with the Simulation

### Sensors You Can Control:

**Potentiometers** (drag the knob):
- Soil Moisture
- Soil pH  
- Leaf Wetness
- Leaf Temperature (pot9)
- Wind Speed
- Wind Direction
- CO2 Level (pot6)
- CO Level (pot7)

**DHT22** (click sensor, edit properties):
- Temperature
- Humidity

**LDR** (Light Sensor):
- Click to change light level

**PIR Motion Sensor**:
- Click to trigger motion

**HC-SR04** (Ultrasonic - Water Tank):
- Edit distance property

**Gas Sensor (MQ2)**:
- Simulated via code

---

## 📺 What You'll See

### 20x4 LCD Display:
Rotates through 8 screens every 4 seconds:
1. Soil sensors (moisture, temp, pH)
2. Leaf sensors (temp, wetness)
3. Air conditions (temp, humidity, light)
4. Wind (speed, direction)
5. Water & Weight
6. Gas & CO2
7. CO & Motion
8. System Status

### LEDs:
- **Red LED 1** (Pin 16): Soil moisture alert
- **Red LED 2** (Pin 17): Gas/CO/CO2 danger
- **Yellow LED** (Pin 9): Motion detected
- **Green LED** (Pin 10): System heartbeat

### Buzzer:
Sounds alarm when:
- Gas > 500 ppm
- CO > 100 ppm
- Soil moisture < 15%

### Serial Monitor:
Click "Serial Monitor" tab to see detailed readings every 2 seconds.

---

## ✅ Expected Link Format

After uploading, your shareable link will be:
```
https://wokwi.com/projects/XXXXXXXXXXXXXXXXX
```

**Example:** `https://wokwi.com/projects/123456789012345678`

---

## 📧 Share With Your Team

Once you have the link, send this message:

> Hi Team,
> 
> Check out our Smart Farm Monitoring System simulation:
> 
> **🔗 Live Wokwi Simulation:**  
> https://wokwi.com/projects/YOUR_PROJECT_ID
> 
> **How to use:**
> - Click "Start Simulation" if not running
> - Drag potentiometer knobs to change sensor values
> - Click DHT22 sensor to edit temperature/humidity
> - Click PIR sensor to trigger motion
> - Watch the 20x4 LCD display cycle through sensors
> - Check Serial Monitor for detailed readings
> 
> **Features:**
> - ✅ 16 fully interactive sensors
> - ✅ Real-time LCD display
> - ✅ LED indicators for alerts
> - ✅ Buzzer for critical warnings
> - ✅ Complete IoT farm monitoring
> 
> Enjoy! 🌱

---

## 🔧 Troubleshooting

**Compilation Error?**
- Make sure you copied ALL the code from sketch.ino
- Check that libraries are installed (Wokwi does this automatically)

**Circuit Not Showing?**
- Make sure you imported diagram.json correctly
- Try refreshing the page and importing again

**Sensors Not Working?**
- Click "Stop" then "Start Simulation" again
- Check Serial Monitor for error messages

---

## 📚 Additional Resources

- **GitHub Repository:** https://github.com/ShahdGamal-3/IoT-Project
- **Web Dashboard:** https://shahdgamal-3.github.io/IoT-Project/
- **Full Documentation:** See README_COMPLETE.md in repository

---

**Created:** November 29, 2025  
**Project:** Smart Farm Monitoring System  
**Platform:** Wokwi IoT Simulator
