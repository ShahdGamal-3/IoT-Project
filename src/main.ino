/**
 * @file main.ino
 * @brief Farm Monitoring System - Complete Implementation
 * 
 * Current Step: All Sensors + Alert System
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "SoilMoistureSensor.h"
#include "SoilTemperatureSensor.h"
#include "SoilPHSensor.h"
#include "LeafTemperatureSensor.h"
#include "LeafWetnessSensor.h"
#include "DHTSensor.h"
#include "LightSensor.h"
#include "WindSpeedSensor.h"
#include "WindDirectionSensor.h"
#include "RainfallSensor.h"
#include "WaterTankSensor.h"
#include "GasSensor.h"
#include "CO2Sensor.h"
#include "COSensor.h"
#include "MotionSensor.h"
#include "WeightSensor.h"
#include "AlertSystem.h"

// 20x4 LCD Configuration (I2C address 0x27, 20 columns, 4 rows)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Sensor Pins
#define SOIL_MOISTURE_PIN 34  // ADC1 pin for soil moisture
#define SOIL_TEMP_PIN 15      // OneWire pin for DS18B20
#define SOIL_PH_PIN 35        // ADC1 pin for pH sensor
#define LEAF_TEMP_PIN 37      // ADC1 pin for leaf temperature (potentiometer simulation)
#define LEAF_WETNESS_PIN 32   // ADC1 pin for leaf wetness
#define DHT_PIN 25            // GPIO pin for DHT22
#define LDR_PIN 33            // ADC1 pin for LDR
#define WIND_SPEED_PIN 27     // GPIO pin for anemometer (pulse input)
#define WIND_SIM_PIN 26       // GPIO pin for PWM simulator output
#define WIND_POT_PIN 36       // ADC1 pin for wind speed potentiometer
#define WIND_DIR_PIN 39       // ADC1 pin for wind direction vane
#define RAIN_PIN 14           // GPIO pin for rain gauge (pulse input)
#define WATER_TRIG_PIN 12     // GPIO pin for HC-SR04 trigger
#define WATER_ECHO_PIN 13     // GPIO pin for HC-SR04 echo
#define GAS_PIN 4             // ADC2 pin for MQ2 gas sensor
#define CO2_PIN 0             // ADC2 pin for MQ135 CO2 sensor
#define CO_PIN 2              // ADC2 pin for MQ7 CO sensor
#define MOTION_PIN 19         // GPIO pin for PIR motion sensor
#define WEIGHT_DATA_PIN 5     // GPIO pin for HX711 data
#define WEIGHT_CLOCK_PIN 18   // GPIO pin for HX711 clock
#define BUZZER_PIN 23         // GPIO pin for buzzer

// LED Indicator Pins
#define LED_SOIL_PIN 16       // Red LED - Soil moisture warning
#define LED_GAS_PIN 17        // Red LED - Gas/CO/CO2 danger
#define LED_MOTION_PIN 9      // Yellow LED - Motion detected
#define LED_SYSTEM_PIN 10     // Green LED - System status (heartbeat)

// Sensor Objects
SoilMoistureSensor soilMoisture(SOIL_MOISTURE_PIN);
SoilTemperatureSensor soilTemp(SOIL_TEMP_PIN);
SoilPHSensor soilPH(SOIL_PH_PIN);
LeafTemperatureSensor leafTemp(LEAF_TEMP_PIN);
LeafWetnessSensor leafWetness(LEAF_WETNESS_PIN);
DHTSensor dhtSensor(DHT_PIN);
LightSensor lightSensor(LDR_PIN);
WindSpeedSensor windSpeed(WIND_SPEED_PIN);
WindDirectionSensor windDirection(WIND_DIR_PIN);
RainfallSensor rainfall(RAIN_PIN);
WaterTankSensor waterTank(WATER_TRIG_PIN, WATER_ECHO_PIN, 100.0, 1000.0);  // 100cm height, 1000L capacity
GasSensor gasSensor(GAS_PIN);
CO2Sensor co2Sensor(CO2_PIN);
COSensor coSensor(CO_PIN);
MotionSensor motionSensor(MOTION_PIN);
WeightSensor weightSensor(WEIGHT_DATA_PIN, WEIGHT_CLOCK_PIN);
AlertSystem alertSystem(BUZZER_PIN);

// Timing
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 2000; // Update every 2 seconds

// Display mode
int displayMode = 0;
unsigned long lastModeSwitch = 0;
const unsigned long MODE_SWITCH_INTERVAL = 4000; // Switch display every 4 seconds

// Global variables to store sensor readings
bool dhtValid = false;
float airTemp = 0.0;
float humidity = 0.0;
String airTempStatus = "";
String humidityStatus = "";

float lightPercent = 0.0;
String lightStatus = "";

float windSpeed_ms = 0.0;
float windSpeed_kmh = 0.0;
String windStatus = "";

int windDir_degrees = 0;
String windDir_cardinal = "";

float rainfall_mm = 0.0;
float rainRate = 0.0;
String rainStatus = "";

float waterLevel_cm = 0.0;
float waterLevel_percent = 0.0;
float waterVolume_liters = 0.0;
String tankStatus = "";

float gasPPM = 0.0;
String gasStatus = "";

float co2PPM = 0.0;
String airQuality = "";

float coPPM = 0.0;
String coStatus = "";

bool motionDetected = false;
String motionStatus = "";

float weight_kg = 0.0;
String weightStatus = "";

// Remote control variables (values received from dashboard via Serial)
bool remoteControlActive = false;
float remote_soilMoisture = -1;
float remote_soilTemp = -1;
float remote_soilPH = -1;
float remote_leafTemp = -1;
float remote_leafWetness = -1;
float remote_airTemp = -1;
float remote_humidity = -1;
float remote_light = -1;
float remote_rainfall = -1;
float remote_windSpeed = -1;
float remote_windDirection = -1;
float remote_gas = -1;
float remote_co2 = -1;
float remote_co = -1;
float remote_waterLevel = -1;
float remote_weight = -1;

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);
    Serial.println("\n=================================" );
    Serial.println("Farm Monitoring System - v2.0");
    Serial.println("Complete with All Sensors!");
    Serial.println("Serial Protocol: JSON");
    Serial.println("=================================\n");

    // Initialize LED indicator pins
    pinMode(LED_SOIL_PIN, OUTPUT);
    pinMode(LED_GAS_PIN, OUTPUT);
    pinMode(LED_MOTION_PIN, OUTPUT);
    pinMode(LED_SYSTEM_PIN, OUTPUT);
    
    // Turn off all LEDs initially
    digitalWrite(LED_SOIL_PIN, LOW);
    digitalWrite(LED_GAS_PIN, LOW);
    digitalWrite(LED_MOTION_PIN, LOW);
    digitalWrite(LED_SYSTEM_PIN, LOW);
    
    Serial.println("LED indicators initialized");

    // Initialize I2C for 20x4 LCD
    Wire.begin();
    
    // Initialize 20x4 LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Farm Monitor");
    lcd.setCursor(0, 1);
    lcd.print("Initializing...");
    
    delay(2000);

    // Initialize Sensors
    soilMoisture.begin();
    soilTemp.begin();
    soilPH.begin();
    leafTemp.begin();
    leafWetness.begin();
    dhtSensor.begin();
    lightSensor.begin();
    windSpeed.begin();
    windDirection.begin();
    rainfall.begin();
    waterTank.begin();
    gasSensor.begin();
    co2Sensor.begin();
    coSensor.begin();
    motionSensor.begin();
    weightSensor.begin();
    alertSystem.begin();
    
    // Enable wind speed simulation mode for Wokwi
    windSpeed.enableSimulation(WIND_SIM_PIN, WIND_POT_PIN);
    
    Serial.println("LDR Light Sensor initialized");
    Serial.println("Wind Speed Sensor initialized with PWM simulation");
    Serial.println("Wind Direction Sensor initialized");
    Serial.println("Rainfall Sensor initialized");
    Serial.println("Water Tank Level Sensor initialized");
    Serial.println("Gas Sensor initialized");
    Serial.println("Motion Sensor initialized");
    Serial.println("Weight Sensor initialized");
    Serial.println("Alert System initialized");

    // Display ready message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready!");
    delay(1000);

    Serial.println("\nSystem initialized successfully!");
    Serial.println("Starting sensor readings...\n");
}

void loop() {
    unsigned long currentTime = millis();

    // Check for incoming Serial commands from dashboard
    checkSerialCommands();

    // Update wind speed simulation (read potentiometer)
    windSpeed.updateSimulation();

    // Update readings at specified interval
    if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
        lastUpdate = currentTime;

        // Read sensor data
        float moisture = soilMoisture.readMoisture();
        int rawMoisture = soilMoisture.getRawValue();
        String moistureStatus = soilMoisture.getMoistureStatus();

        float tempC = soilTemp.readTemperature();
        float tempF = soilTemp.getTemperatureF();
        String tempStatus = soilTemp.getTemperatureStatus();

        float pH = soilPH.readPH();
        float phVoltage = soilPH.getVoltage();
        String phStatus = soilPH.getPHStatus();

        float leafTempC = leafTemp.readTemperature();
        float leafTempF = leafTemp.getObjectTempF();
        String leafTempStatus = leafTemp.getTemperatureStatus();

        float leafWet = leafWetness.readWetness();
        String leafWetStatus = leafWetness.getWetnessStatus();

        dhtValid = dhtSensor.readSensor();
        airTemp = dhtSensor.getTemperature();
        humidity = dhtSensor.getHumidity();
        airTempStatus = dhtSensor.getTemperatureStatus();
        humidityStatus = dhtSensor.getHumidityStatus();

        lightPercent = lightSensor.readLight();
        lightStatus = lightSensor.getLightStatus();

        windSpeed_ms = windSpeed.calculateWindSpeed();
        windSpeed_kmh = windSpeed.getWindSpeed_kmh();
        windStatus = windSpeed.getWindStatus();

        windDir_degrees = windDirection.readDirection();
        
        // Override with remote values if available
        if (remoteControlActive) {
            if (remote_soilMoisture >= 0) moisture = remote_soilMoisture;
            if (remote_soilTemp >= -10) tempC = remote_soilTemp;
            if (remote_soilPH >= 0) pH = remote_soilPH;
            if (remote_leafTemp >= 0) leafTempC = remote_leafTemp;
            if (remote_leafWetness >= 0) leafWet = remote_leafWetness;
            if (remote_airTemp >= -20) airTemp = remote_airTemp;
            if (remote_humidity >= 0) humidity = remote_humidity;
            if (remote_light >= 0) lightPercent = remote_light;
            if (remote_rainfall >= 0) rainfall_mm = remote_rainfall;
            if (remote_windSpeed >= 0) windSpeed_kmh = remote_windSpeed;
            if (remote_windDirection >= 0) windDir_degrees = (int)remote_windDirection;
            if (remote_gas >= 0) gasPPM = remote_gas;
            if (remote_co2 >= 0) co2PPM = remote_co2;
            if (remote_co >= 0) coPPM = remote_co;
            if (remote_waterLevel >= 0) waterLevel_percent = remote_waterLevel;
            if (remote_weight >= 0) weight_kg = remote_weight;
        }
        windDir_cardinal = windDirection.getCardinalDirection();

        rainfall.update();
        rainfall_mm = rainfall.getRainfall_mm();
        rainRate = rainfall.getRainRate();
        rainStatus = rainfall.getRainStatus();

        waterLevel_cm = waterTank.readLevel();
        waterLevel_percent = waterTank.getLevel_percent();
        waterVolume_liters = waterTank.getVolume_liters();
        tankStatus = waterTank.getTankStatus();

        gasPPM = gasSensor.readGas();
        gasStatus = gasSensor.getGasStatus();

        co2PPM = co2Sensor.readCO2();
        airQuality = co2Sensor.getAirQuality();

        coPPM = coSensor.readCO();
        coStatus = coSensor.getCOStatus();

        motionDetected = motionSensor.readMotion();
        motionStatus = motionSensor.getMotionStatus();

        weight_kg = weightSensor.readWeight();
        weightStatus = weightSensor.getWeightStatus();

        // Control LED indicators and check for alert conditions
        // Soil moisture LED (Red)
        if (soilMoisture.getMoisturePercent() < 20) {
            digitalWrite(LED_SOIL_PIN, HIGH);
            alertSystem.triggerAlert(ALERT_LOW_SOIL_MOISTURE);
        } else {
            digitalWrite(LED_SOIL_PIN, LOW);
        }
        
        // Gas/CO/CO2 danger LED (Red)
        if (gasSensor.isDangerous() || co2Sensor.isDangerous() || coSensor.isDangerous()) {
            digitalWrite(LED_GAS_PIN, HIGH);
            if (gasSensor.isDangerous()) {
                alertSystem.triggerAlert(ALERT_GAS_DETECTED);
            }
            if (co2Sensor.isDangerous()) {
                alertSystem.triggerAlert(ALERT_GAS_DETECTED);
            }
            if (coSensor.isDangerous()) {
                alertSystem.triggerAlert(ALERT_GAS_DETECTED);
            }
        } else {
            digitalWrite(LED_GAS_PIN, LOW);
        }
        
        // Motion detection LED (Yellow)
        if (motionDetected) {
            digitalWrite(LED_MOTION_PIN, HIGH);
            alertSystem.triggerAlert(ALERT_MOTION_DETECTED);
        } else {
            digitalWrite(LED_MOTION_PIN, LOW);
        }
        
        // Water tank warning
        if (waterTank.isLowLevel()) {
            alertSystem.triggerAlert(ALERT_LOW_WATER);
        }
        
        // Weight overload warning
        if (weightSensor.isOverloaded()) {
            alertSystem.triggerAlert(ALERT_OVERWEIGHT);
        }
        
        // System heartbeat LED (Green) - blink every cycle
        static bool heartbeatState = false;
        heartbeatState = !heartbeatState;
        digitalWrite(LED_SYSTEM_PIN, heartbeatState ? HIGH : LOW);

        // Display on Serial Monitor
        Serial.println("========== SENSOR READINGS ==========");
        
        Serial.println("--- SOIL MOISTURE ---");
        Serial.print("Raw ADC Value: ");
        Serial.println(rawMoisture);
        Serial.print("Moisture: ");
        Serial.print(moisture, 1);
        Serial.println(" %");
        Serial.print("Status: ");
        Serial.println(moistureStatus);
        
        Serial.println("\n--- SOIL TEMPERATURE ---");
        Serial.print("Temperature: ");
        Serial.print(tempC, 1);
        Serial.print(" °C (");
        Serial.print(tempF, 1);
        Serial.println(" °F)");
        Serial.print("Status: ");
        Serial.println(tempStatus);
        
        Serial.println("\n--- SOIL pH ---");
        Serial.print("pH Value: ");
        Serial.println(pH, 2);
        Serial.print("Voltage: ");
        Serial.print(phVoltage, 3);
        Serial.println(" V");
        Serial.print("Status: ");
        Serial.println(phStatus);
        
        Serial.println("\n--- LEAF TEMPERATURE ---");
        Serial.print("Leaf Temp: ");
        Serial.print(leafTempC, 1);
        Serial.print(" °C (");
        Serial.print(leafTempF, 1);
        Serial.println(" °F)");
        Serial.print("Status: ");
        Serial.println(leafTempStatus);
        
        Serial.println("\n--- LEAF WETNESS ---");
        Serial.print("Wetness: ");
        Serial.print(leafWet, 1);
        Serial.println(" %");
        Serial.print("Status: ");
        Serial.println(leafWetStatus);
        
        Serial.println("\n--- AIR TEMPERATURE & HUMIDITY ---");
        if (dhtValid) {
            Serial.print("Air Temperature: ");
            Serial.print(airTemp, 1);
            Serial.println(" °C");
            Serial.print("Status: ");
            Serial.println(airTempStatus);
            Serial.print("Humidity: ");
            Serial.print(humidity, 1);
            Serial.println(" %");
            Serial.print("Status: ");
            Serial.println(humidityStatus);
        } else {
            Serial.println("DHT22 reading failed!");
        }
        
        Serial.println("\n--- LIGHT INTENSITY ---");
        Serial.print("Light Level: ");
        Serial.print(lightPercent, 1);
        Serial.println(" %");
        Serial.print("Status: ");
        Serial.println(lightStatus);
        
        Serial.println("\n--- WIND SPEED ---");
        Serial.print("Wind Speed: ");
        Serial.print(windSpeed_kmh, 1);
        Serial.print(" km/h (");
        Serial.print(windSpeed_ms, 1);
        Serial.println(" m/s)");
        Serial.print("Status: ");
        Serial.println(windStatus);
        
        Serial.println("\n--- WIND DIRECTION ---");
        Serial.print("Direction: ");
        Serial.print(windDir_degrees);
        Serial.print("° (");
        Serial.print(windDir_cardinal);
        Serial.println(")");
        
        Serial.println("\n--- RAINFALL ---");
        Serial.print("Total Rainfall: ");
        Serial.print(rainfall_mm, 2);
        Serial.print(" mm (");
        Serial.print(rainfall.getRainfall_inches(), 2);
        Serial.println(" in)");
        Serial.print("Rain Rate: ");
        Serial.print(rainRate, 1);
        Serial.println(" mm/h");
        Serial.print("Status: ");
        Serial.println(rainStatus);
        Serial.print("Intensity: ");
        Serial.println(rainfall.getRainIntensity());
        
        Serial.println("\n--- WATER TANK LEVEL ---");
        Serial.print("Water Level: ");
        Serial.print(waterLevel_cm, 1);
        Serial.print(" cm (");
        Serial.print(waterLevel_percent, 1);
        Serial.println(" %)");
        Serial.print("Volume: ");
        Serial.print(waterVolume_liters, 0);
        Serial.println(" L");
        Serial.print("Status: ");
        Serial.println(tankStatus);
        if (waterTank.isLowLevel()) {
            Serial.println("WARNING: Low water level!");
        }
        
        Serial.println("\n--- GAS SENSOR ---");
        Serial.print("Gas Concentration: ");
        Serial.print(gasPPM, 0);
        Serial.println(" ppm");
        Serial.print("Status: ");
        Serial.println(gasStatus);
        if (gasSensor.isDangerous()) {
            Serial.println("DANGER: High gas level detected!");
        }
        
        Serial.println("\n--- CO2 SENSOR ---");
        Serial.print("CO2 Concentration: ");
        Serial.print(co2PPM, 0);
        Serial.println(" ppm");
        Serial.print("Air Quality: ");
        Serial.println(airQuality);
        if (co2Sensor.isDangerous()) {
            Serial.println("WARNING: High CO2 level - Poor ventilation!");
        }
        
        Serial.println("\n--- CO SENSOR ---");
        Serial.print("CO Concentration: ");
        Serial.print(coPPM, 0);
        Serial.println(" ppm");
        Serial.print("Status: ");
        Serial.println(coStatus);
        if (coSensor.isDangerous()) {
            Serial.println("DANGER: High CO level - Carbon Monoxide detected!");
        }
        
        Serial.println("\n--- MOTION SENSOR ---");
        Serial.print("Status: ");
        Serial.println(motionStatus);
        Serial.print("Total Motion Events: ");
        Serial.println(motionSensor.getMotionCount());
        
        Serial.println("\n--- WEIGHT SENSOR ---");
        Serial.print("Weight: ");
        Serial.print(weight_kg, 2);
        Serial.print(" kg (");
        Serial.print(weightSensor.getWeight_lbs(), 2);
        Serial.println(" lbs)");
        Serial.print("Status: ");
        Serial.println(weightStatus);
        
        Serial.println("=====================================\n");
    }

    // Switch 20x4 LCD display mode (show 2 sensors at once)
    if (currentTime - lastModeSwitch >= MODE_SWITCH_INTERVAL) {
        lastModeSwitch = currentTime;
        displayMode = (displayMode + 1) % 14;  // 14 screens total
        
        lcd.clear();
        
        if (displayMode == 0) {
            // Screen 1: Soil Moisture
            lcd.setCursor(0, 0);
            lcd.print("SoilMoist:");
            lcd.print(soilMoisture.getMoisturePercent(), 1);
            lcd.print("%");
            
            lcd.setCursor(0, 1);
            lcd.print(soilMoisture.getMoistureStatus());
            
        } else if (displayMode == 1) {
            // Screen 2: Soil Temperature
            lcd.setCursor(0, 0);
            lcd.print("SoilTemp:");
            lcd.print(soilTemp.getTemperatureC(), 1);
            lcd.print("C");
            
            lcd.setCursor(0, 1);
            lcd.print(soilTemp.getTemperatureStatus());
            
            lcd.setCursor(0, 1);
            lcd.print("Status: ");
            lcd.print(soilPH.getPHStatus());
            
            lcd.setCursor(0, 2);
            lcd.print("Leaf Temp: ");
            lcd.print(leafTemp.getObjectTempC(), 1);
            lcd.print("C");
            
            lcd.setCursor(0, 3);
            lcd.print("Status: ");
            lcd.print(leafTemp.getTemperatureStatus());
        } else if (displayMode == 2) {
            // Screen 3: Air Temperature & Humidity
            lcd.setCursor(0, 0);
            lcd.print("Air Temp: ");
            if (dhtValid) {
                lcd.print(airTemp, 1);
                lcd.print("C");
            } else {
                lcd.print("Error!");
            }
            
            lcd.setCursor(0, 1);
            lcd.print("Status: ");
            if (dhtValid) {
                lcd.print(airTempStatus);
            } else {
                lcd.print("N/A");
            }
            
            lcd.setCursor(0, 2);
            lcd.print("Air Humidity: ");
            if (dhtValid) {
                lcd.print(humidity, 1);
                lcd.print("%");
            } else {
                lcd.print("Error!");
            }
            
            lcd.setCursor(0, 3);
            lcd.print("Status: ");
            if (dhtValid) {
                lcd.print(humidityStatus);
            } else {
                lcd.print("N/A");
            }
        } else if (displayMode == 3) {
            // Screen 4: Light Intensity
            lcd.setCursor(0, 0);
            lcd.print("Light: ");
            lcd.print(lightPercent, 1);
            lcd.print(" %");
            
            lcd.setCursor(0, 1);
            lcd.print("Status: ");
            lcd.print(lightStatus);
        } else if (displayMode == 4) {
            // Screen 5: Wind Speed
            lcd.setCursor(0, 0);
            lcd.print("Wind Speed:");
            
            lcd.setCursor(0, 1);
            lcd.print(windSpeed_kmh, 1);
            lcd.print(" km/h");
            
            lcd.setCursor(0, 2);
            lcd.print(windSpeed_ms, 2);
            lcd.print(" m/s");
            
            lcd.setCursor(0, 3);
            lcd.print(windStatus);
        } else if (displayMode == 5) {
            // Screen 6: Wind Direction
            lcd.setCursor(0, 0);
            lcd.print("=== WIND DIRECTION ==");
            
            lcd.setCursor(0, 1);
            lcd.print("Angle: ");
            lcd.print(windDir_degrees);
            lcd.print((char)223);  // degree symbol
            
            lcd.setCursor(0, 2);
            lcd.print("Direction: ");
            lcd.print(windDir_cardinal);
            
        } else if (displayMode == 6) {
            // Screen 7: Rainfall
            lcd.setCursor(0, 0);
            lcd.print("===== RAINFALL =====");
            
            lcd.setCursor(0, 1);
            lcd.print("Total: ");
            lcd.print(rainfall_mm, 2);
            lcd.print(" mm");
            
            lcd.setCursor(0, 2);
            lcd.print("Rate: ");
            lcd.print(rainRate, 1);
            lcd.print(" mm/h");
            
            lcd.setCursor(0, 3);
            lcd.print(rainStatus);
        } else if (displayMode == 7) {
            // Screen 8: Water Tank Level
            lcd.setCursor(0, 0);
            lcd.print("=== WATER TANK ====");
            
            lcd.setCursor(0, 1);
            lcd.print("Level: ");
            lcd.print(waterLevel_percent, 1);
            lcd.print(" %");
            
            lcd.setCursor(0, 2);
            lcd.print("Volume: ");
            lcd.print(waterVolume_liters, 0);
            lcd.print(" L");
            
            lcd.setCursor(0, 3);
            lcd.print(tankStatus);
            if (waterTank.isLowLevel()) {
                lcd.print(" - LOW!");
            }
        } else if (displayMode == 8) {
            // Screen 9: Gas Sensor
            lcd.setCursor(0, 0);
            lcd.print("==== GAS SENSOR ====");
            
            lcd.setCursor(0, 1);
            lcd.print("Gas: ");
            lcd.print(gasPPM, 0);
            lcd.print(" ppm");
            
            lcd.setCursor(0, 2);
            lcd.print("Status: ");
            lcd.print(gasStatus);
            
            lcd.setCursor(0, 3);
            if (gasSensor.isDangerous()) {
                lcd.print("DANGER!");
            } else {
                lcd.print("Safe");
            }
        } else if (displayMode == 9) {
            // Screen 10: CO2 Sensor
            lcd.setCursor(0, 0);
            lcd.print("==== CO2 SENSOR ====");
            
            lcd.setCursor(0, 1);
            lcd.print("CO2: ");
            lcd.print(co2PPM, 0);
            lcd.print(" ppm");
            
            lcd.setCursor(0, 2);
            lcd.print("Air Quality: ");
            lcd.print(airQuality);
            
            lcd.setCursor(0, 3);
            if (co2Sensor.isDangerous()) {
                lcd.print("Poor Ventilation!");
            } else {
                lcd.print("Good");
            }
        } else if (displayMode == 10) {
            // Screen 11: CO Sensor
            lcd.setCursor(0, 0);
            lcd.print("===== CO SENSOR ====");
            
            lcd.setCursor(0, 1);
            lcd.print("CO: ");
            lcd.print(coPPM, 0);
            lcd.print(" ppm");
            
            lcd.setCursor(0, 2);
            lcd.print("Status: ");
            lcd.print(coStatus);
            
            lcd.setCursor(0, 3);
            if (coSensor.isDangerous()) {
                lcd.print("*** DANGER ***");
            } else {
                lcd.print("Safe");
            }
        } else if (displayMode == 11) {
            // Screen 12: Motion Sensor
            lcd.setCursor(0, 0);
            lcd.print("=== MOTION SENSOR ==");
            
            lcd.setCursor(0, 1);
            lcd.print(motionStatus);
            
            lcd.setCursor(0, 2);
            lcd.print("Events: ");
            lcd.print(motionSensor.getMotionCount());
            
            lcd.setCursor(0, 3);
            if (motionDetected) {
                lcd.print("*** ALERT ***");
            } else {
                lcd.print("All clear");
            }
        } else if (displayMode == 12) {
            // Screen 13: Weight Sensor
            lcd.setCursor(0, 0);
            lcd.print("=== WEIGHT SENSOR ==");
            
            lcd.setCursor(0, 1);
            lcd.print("Weight: ");
            lcd.print(weight_kg, 1);
            lcd.print(" kg");
            
            lcd.setCursor(0, 2);
            lcd.print(weightSensor.getWeight_lbs(), 1);
            lcd.print(" lbs");
            
            lcd.setCursor(0, 3);
            lcd.print(weightStatus);
        } else {
            // Screen 14: Leaf Wetness (single sensor display)
            lcd.setCursor(0, 0);
            lcd.print("=== LEAF WETNESS ===");
            
            lcd.setCursor(0, 1);
            lcd.print("Wetness: ");
            lcd.print(leafWetness.getWetnessPercent(), 1);
            lcd.print("%");
            
            lcd.setCursor(0, 2);
            lcd.print("Status: ");
            lcd.print(leafWetness.getWetnessStatus());
            
            lcd.setCursor(0, 3);
            if (leafWetness.isWet()) {
                lcd.print("Leaf is WET");
            } else {
                lcd.print("Leaf is DRY");
            }
        }
    }
}

/**
 * Check for incoming Serial commands from dashboard
 * Expected JSON format: {"sensor":"soilMoisture","value":45.5}
 */
void checkSerialCommands() {
    if (Serial.available() > 0) {
        String jsonData = Serial.readStringUntil('\n');
        jsonData.trim();
        
        if (jsonData.length() > 0) {
            // Simple JSON parsing (looking for "sensor" and "value")
            int sensorStart = jsonData.indexOf("\"sensor\":\"") + 10;
            int sensorEnd = jsonData.indexOf("\"", sensorStart);
            int valueStart = jsonData.indexOf("\"value\":") + 8;
            int valueEnd = jsonData.indexOf("}", valueStart);
            
            if (sensorStart > 9 && sensorEnd > sensorStart && valueStart > 7) {
                String sensor = jsonData.substring(sensorStart, sensorEnd);
                String valueStr = jsonData.substring(valueStart, valueEnd);
                float value = valueStr.toFloat();
                
                // Enable remote control mode
                remoteControlActive = true;
                
                // Update the appropriate remote value
                if (sensor == "soilMoisture") {
                    remote_soilMoisture = value;
                    Serial.print("✓ Soil Moisture set to: ");
                } else if (sensor == "soilTemp") {
                    remote_soilTemp = value;
                    Serial.print("✓ Soil Temp set to: ");
                } else if (sensor == "soilPH") {
                    remote_soilPH = value;
                    Serial.print("✓ Soil pH set to: ");
                } else if (sensor == "leafTemp") {
                    remote_leafTemp = value;
                    Serial.print("✓ Leaf Temp set to: ");
                } else if (sensor == "leafWetness") {
                    remote_leafWetness = value;
                    Serial.print("✓ Leaf Wetness set to: ");
                } else if (sensor == "airTemp") {
                    remote_airTemp = value;
                    Serial.print("✓ Air Temp set to: ");
                } else if (sensor == "humidity") {
                    remote_humidity = value;
                    Serial.print("✓ Humidity set to: ");
                } else if (sensor == "light") {
                    remote_light = value;
                    Serial.print("✓ Light set to: ");
                } else if (sensor == "rainfall") {
                    remote_rainfall = value;
                    Serial.print("✓ Rainfall set to: ");
                } else if (sensor == "windSpeed") {
                    remote_windSpeed = value;
                    Serial.print("✓ Wind Speed set to: ");
                } else if (sensor == "windDirection") {
                    remote_windDirection = value;
                    Serial.print("✓ Wind Direction set to: ");
                } else if (sensor == "gas") {
                    remote_gas = value;
                    Serial.print("✓ Gas set to: ");
                } else if (sensor == "co2") {
                    remote_co2 = value;
                    Serial.print("✓ CO2 set to: ");
                } else if (sensor == "co") {
                    remote_co = value;
                    Serial.print("✓ CO set to: ");
                } else if (sensor == "waterLevel") {
                    remote_waterLevel = value;
                    Serial.print("✓ Water Level set to: ");
                } else if (sensor == "weight") {
                    remote_weight = value;
                    Serial.print("✓ Weight set to: ");
                } else {
                    Serial.print("✗ Unknown sensor: ");
                    Serial.println(sensor);
                    return;
                }
                
                Serial.println(value);
            }
        }
    }
}
