# Quick Start Guide - Wokwi Website

## Step-by-Step Instructions

### 1. Open Wokwi
Go to: **https://wokwi.com/**

### 2. Create New Project
- Click **"+ New Project"**
- Select **"Arduino ESP32"**

### 3. Upload the Code
- Open the file `smart_farm_merged.ino` from this folder
- Select ALL the code (Ctrl+A) and copy it (Ctrl+C)
- In Wokwi editor, select all existing code and paste the copied code

### 4. Update the Diagram
- At the bottom of the Wokwi page, click on **"diagram.json"** tab
- Delete all existing content
- Open the file `diagram.json` from this folder
- Copy all content and paste it into the Wokwi diagram.json tab

### 5. Add Libraries
Click on the **"Library Manager"** icon (📚 book icon) on the left sidebar and add these libraries:

1. **Firebase ESP32 Client** (by Mobizt)
2. **LiquidCrystal I2C** (by Frank de Brabander)
3. **HX711 Arduino Library** (by Bogdan Necula)
4. **DallasTemperature** (by Miles Burton)
5. **OneWire** (by Paul Stoffregen)
6. **DHT sensor library** (by Adafruit)

**Alternative:** Click on **"libraries.txt"** tab and paste the content from our `libraries.txt` file.

### 6. Start Simulation
- Click the green **"Start Simulation"** ▶ button at the top
- Wait for compilation to complete
- Watch the simulation run!

---

## What to Expect

### Visual Elements
- **LCD Screen**: Will show rotating pages of sensor data
- **4 LEDs**: Status indicators (Red: Soil/Gas alerts, Yellow: Motion, Green: System OK)
- **Buzzer**: Will sound if gas levels are high
- **Multiple Potentiometers**: Adjust these to simulate different sensor values

### Serial Monitor
- Click on the **Serial Monitor** at the bottom to see detailed output
- Shows formatted sensor readings every 5 seconds
- Firebase upload confirmations every 30 seconds

### Interactive Elements
You can interact with:
- **Potentiometers**: Drag sliders to change sensor values
- **PIR Sensor**: Click to trigger motion detection
- **DHT22**: Use built-in controls to change temperature/humidity
- **Ultrasonic**: Change distance property

---

## Testing the Alerts

### Trigger Soil Alert (Red LED)
- Adjust the "Soil Moisture" potentiometer to the left (low value)
- The red "Soil Alert" LED should turn on

### Trigger Gas Alert (Red LED + Buzzer)
- Adjust the "Gas Sensor" analog value to high
- OR adjust "CO2 Level" potentiometer to the right (high value)
- OR adjust "CO Level" potentiometer to the right (high value)
- The red "Gas Alert" LED and Buzzer should activate

### Trigger Motion Alert (Yellow LED)
- Click on the PIR Motion Sensor component
- The yellow "Motion" LED should turn on

### System OK (Green LED)
- When no alerts are active, the green "System OK" LED stays on
- When any alert triggers, it turns off

---

## Troubleshooting

### "Library not found" error
- Make sure all 6 libraries are added
- Check spelling matches exactly
- Try closing and reopening the Library Manager

### "Compilation failed"
- Verify you copied the entire .ino file content
- Check that no characters were lost during copy/paste
- Make sure you selected "ESP32" board, not Arduino Uno

### Simulation won't start
- Check browser console for errors (F12)
- Try refreshing the page
- Clear browser cache and try again

### No serial output
- Click on the "Serial Monitor" tab at the bottom
- Check baud rate is set to 115200
- Wait a few seconds after simulation starts

### Firebase not connecting
- This is normal in Wokwi simulation
- Firebase requires actual internet which Wokwi simulates
- The code will continue running without Firebase

---

## Customization Options

### Change Alert Thresholds
In the code, find these lines and adjust values:
```cpp
const float MOISTURE_LOW = 30.0;
const float GAS_HIGH = 500.0;
const float CO2_HIGH = 1000.0;
const float CO_HIGH = 50.0;
```

### Change Update Intervals
```cpp
const unsigned long SENSOR_READ_INTERVAL = 5000;   // 5 seconds
const unsigned long FIREBASE_INTERVAL = 30000;     // 30 seconds
const unsigned long LCD_INTERVAL = 2000;           // 2 seconds
```

### Modify LCD Display
Find the `updateLCD()` function to customize what's shown on each page.

---

## Sharing Your Project

1. Once simulation is running, click **"Share"** button at top
2. Choose "Public link" or "Embed code"
3. Copy the link to share with others
4. Others can view and simulate without creating their own project

---

## Save Your Work

- Wokwi automatically saves to your account if logged in
- Click **"Save"** button to ensure changes are saved
- Give your project a descriptive name like "Smart Monitoring Farm"

---

## Need Help?

- Check the main README.md for detailed documentation
- Review pin assignments and sensor descriptions
- Check Serial Monitor for error messages
- Verify all connections in the diagram.json

---

**Enjoy exploring the Smart Monitoring Farm system! 🌾🚜📊**
