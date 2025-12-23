# Smart Monitoring Farm - Merged Wokwi Version

This folder contains a merged version of all three ESP32 nodes (Gateway, Soil, and Weather) combined into a single Wokwi simulation.

## Files Included

- **smart_farm_merged.ino** - The main Arduino sketch with all sensor code merged
- **diagram.json** - Wokwi circuit diagram with all components
- **libraries.txt** - All required libraries for the project

## How to Run on Wokwi Website

### Method 1: Manual Upload (Recommended)

1. Go to [https://wokwi.com/](https://wokwi.com/)
2. Click "Start from scratch" and select "ESP32"
3. Delete the default code in the editor
4. Copy the contents of `smart_farm_merged.ino` and paste it into the editor
5. Click on the "diagram.json" tab at the bottom
6. Delete all content and paste the contents of our `diagram.json` file
7. Click on "Library Manager" (book icon on the left sidebar)
8. Add the following libraries:
   - Firebase ESP32 Client
   - LiquidCrystal I2C
   - HX711 Arduino Library
   - DallasTemperature
   - OneWire
   - DHT sensor library
9. Click the green "Start Simulation" button

### Method 2: Import from Files

1. Go to [https://wokwi.com/](https://wokwi.com/)
2. Click "Upload Project"
3. Select all three files (smart_farm_merged.ino, diagram.json, libraries.txt)
4. Click "Start Simulation"

## Features

This merged project includes all sensors from the three original nodes:

### Gateway Node Sensors
- **LCD Display (20x4)** - Shows system data in rotating pages
- **Ultrasonic Sensor (HC-SR04)** - Water level monitoring
- **Gas Sensor (MQ-2)** - Hazardous gas detection
- **CO2 Sensor** - Carbon dioxide monitoring
- **CO Sensor** - Carbon monoxide monitoring
- **PIR Motion Sensor** - Security and motion detection
- **HX711 Load Cell** - Weight monitoring
- **Buzzer** - Audio alerts for critical conditions
- **4 LEDs** - Visual indicators (Soil Alert, Gas Alert, Motion, System OK)

### Soil Node Sensors
- **Soil Moisture Sensor** - Measures soil humidity (0-100%)
- **Soil pH Sensor** - Monitors soil acidity/alkalinity
- **DS18B20 Temperature Sensor** - Soil temperature

### Weather Node Sensors
- **Leaf Wetness Sensor** - Fungal disease risk prediction
- **Leaf Temperature Sensor** - Plant surface temperature
- **DHT22** - Air temperature and humidity
- **LDR (Photoresistor)** - Light intensity monitoring
- **Wind Speed Sensor** - Wind speed in m/s
- **Wind Direction Sensor** - Wind direction in degrees
- **Rainfall Sensor** - Precipitation monitoring

## Pin Assignments

### Gateway Pins
- GPIO 12: Ultrasonic Trigger
- GPIO 13: Ultrasonic Echo
- GPIO 35: Gas Sensor (Analog)
- GPIO 32: CO2 Sensor (Analog)
- GPIO 33: CO Sensor (Analog)
- GPIO 19: PIR Motion Sensor
- GPIO 25: HX711 Data
- GPIO 18: HX711 Clock
- GPIO 23: Buzzer
- GPIO 16: Soil Alert LED
- GPIO 17: Gas Alert LED
- GPIO 26: Motion LED
- GPIO 27: System OK LED
- GPIO 21: LCD SDA (I2C)
- GPIO 22: LCD SCL (I2C)

### Soil Pins
- GPIO 34: Soil Moisture (Analog)
- GPIO 35: Soil pH (Analog) - shared with Gas sensor
- GPIO 15: DS18B20 Temperature (OneWire)

### Weather Pins
- GPIO 32: Leaf Wetness (Analog) - shared with CO2
- GPIO 35: Leaf Temperature (Analog) - shared
- GPIO 25: DHT22 Data - shared with HX711
- GPIO 33: LDR Light Sensor (Analog) - shared with CO
- GPIO 36: Wind Speed (Analog)
- GPIO 39: Wind Direction (Analog)
- GPIO 39: Rainfall (Analog) - shared with Wind Direction

**Note:** Some pins are shared between sensors due to ESP32 limitations. In the simulation, readings are taken sequentially.

## Alert Thresholds

The system monitors and alerts on the following conditions:

- **Soil Moisture < 30%** - Triggers Soil Alert LED
- **Gas > 500 ppm** - Triggers Gas Alert LED and Buzzer
- **CO2 > 1000 ppm** - Triggers Gas Alert LED and Buzzer
- **CO > 50 ppm** - Triggers Gas Alert LED and Buzzer
- **Water Level < 10 cm** - Visual indication on LCD
- **Motion Detected** - Triggers Motion LED

## LCD Display Rotation

The LCD cycles through 4 pages every 2 seconds:

1. **Soil Data** - Moisture, pH, Temperature
2. **Weather Data** - Air temp, humidity, light, wind speed
3. **Safety Data** - Water level, gas, CO2, CO levels
4. **More Data** - Motion status, weight, leaf wetness

## Serial Monitor Output

The Serial Monitor provides detailed information including:
- Individual sensor readings
- Formatted data tables
- Alert notifications
- Firebase upload status
- Environmental analysis and recommendations

## Firebase Integration

The system uploads data to Firebase Realtime Database every 30 seconds:

- Sensor readings organized by node (soil, weather, gateway)
- Alert status for all monitored conditions
- System timestamp

Configure your Firebase credentials in the code:
```cpp
#define FIREBASE_HOST "your-project.firebaseio.com"
#define API_KEY "your-api-key"
#define DATABASE_URL "your-database-url"
#define USER_EMAIL "your-email@example.com"
#define USER_PASSWORD "your-password"
```

## Simulation Tips

1. **Adjust Potentiometers** - Use the sliders to simulate different sensor values
2. **Monitor Serial Output** - Check the Serial Monitor for detailed information
3. **Watch LCD Display** - The LCD cycles through different data pages
4. **Trigger PIR Sensor** - Click on the PIR sensor to simulate motion
5. **Adjust DHT22** - Use the temperature and humidity sliders on the DHT22
6. **Check LEDs** - Visual indicators show system status
7. **Listen for Buzzer** - Critical gas alerts trigger the buzzer

## Timing Configuration

- Sensor reading interval: 5 seconds
- LCD update interval: 2 seconds
- Firebase upload interval: 30 seconds
- Buzzer pattern: 500ms ON, 500ms OFF (when triggered)

## Original Project Structure

This merged version combines three separate ESP32 nodes that originally communicated via ESP-NOW:

1. **Gateway Node** - Master node with Firebase connectivity
2. **Soil Node** - Slave node for soil monitoring
3. **Weather Node** - Slave node for weather monitoring

The merged version simulates all functionality on a single ESP32 for Wokwi testing purposes.

## Troubleshooting

- **Compilation Errors**: Ensure all libraries are installed
- **Firebase Connection**: Check WiFi credentials (uses Wokwi-GUEST by default)
- **Sensor Readings**: Some sensors may show default values if not properly initialized
- **Performance**: The simulation may run slower with many components

## License

Smart Monitoring Farm - Educational Project
