/**
 * Smart Farm Monitoring System - Complete Wokwi Version
 * All sensors integrated in a single file for easy upload to Wokwi.com
 * 
 * Upload Instructions:
 * 1. Go to https://wokwi.com/new/esp32
 * 2. Replace the sketch.ino content with this file
 * 3. Upload diagram.json from the wokwi-upload folder
 * 4. Click "Start Simulation"
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <HX711.h>

// ==================== PIN DEFINITIONS ====================
#define SOIL_MOISTURE_PIN 34
#define SOIL_TEMP_PIN 15
#define SOIL_PH_PIN 35
#define LEAF_TEMP_PIN 37
#define LEAF_WETNESS_PIN 32
#define DHT_PIN 25
#define DHT_TYPE DHT22
#define LDR_PIN 33
#define WIND_POT_PIN 36
#define WIND_DIR_PIN 39
#define RAIN_PIN 14
#define WATER_TRIG_PIN 12
#define WATER_ECHO_PIN 13
#define GAS_PIN 4
#define CO2_PIN 0
#define CO_PIN 2
#define MOTION_PIN 19
#define WEIGHT_DATA_PIN 5
#define WEIGHT_CLOCK_PIN 18
#define BUZZER_PIN 23

// LED Pins
#define LED_SOIL_PIN 16
#define LED_GAS_PIN 17
#define LED_MOTION_PIN 9
#define LED_SYSTEM_PIN 10

// ==================== SENSOR OBJECTS ====================
LiquidCrystal_I2C lcd(0x27, 20, 4);
OneWire oneWire(SOIL_TEMP_PIN);
DallasTemperature soilTempSensor(&oneWire);
DHT dht(DHT_PIN, DHT_TYPE);
HX711 scale;

// ==================== GLOBAL VARIABLES ====================
unsigned long lastUpdate = 0;
unsigned long lastDisplay = 0;
int displayMode = 0;
const unsigned long UPDATE_INTERVAL = 2000;
const unsigned long DISPLAY_INTERVAL = 4000;

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  Serial.println("\n=================================");
  Serial.println("Smart Farm Monitoring System");
  Serial.println("=================================\n");
  
  // Initialize LCD
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("  Smart Farm");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring System");
  lcd.setCursor(0, 2);
  lcd.print("Initializing...");
  
  // Initialize sensors
  soilTempSensor.begin();
  dht.begin();
  scale.begin(WEIGHT_DATA_PIN, WEIGHT_CLOCK_PIN);
  scale.set_scale(420.0);
  scale.tare();
  
  // Initialize pins
  pinMode(MOTION_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_SOIL_PIN, OUTPUT);
  pinMode(LED_GAS_PIN, OUTPUT);
  pinMode(LED_MOTION_PIN, OUTPUT);
  pinMode(LED_SYSTEM_PIN, OUTPUT);
  pinMode(WATER_TRIG_PIN, OUTPUT);
  pinMode(WATER_ECHO_PIN, INPUT);
  
  delay(2000);
  lcd.clear();
  lcd.print("System Ready!");
  delay(1000);
  
  Serial.println("System initialized!\n");
}

// ==================== SENSOR READING FUNCTIONS ====================

float readSoilMoisture() {
  int raw = analogRead(SOIL_MOISTURE_PIN);
  return map(raw, 4095, 0, 0, 100);
}

float readSoilTemp() {
  soilTempSensor.requestTemperatures();
  return soilTempSensor.getTempCByIndex(0);
}

float readSoilPH() {
  int raw = analogRead(SOIL_PH_PIN);
  float voltage = raw * (3.3 / 4095.0);
  return 7.0 + ((voltage - 1.65) / 0.18);
}

float readLeafTemp() {
  int raw = analogRead(LEAF_TEMP_PIN);
  return map(raw, 0, 4095, 10, 45);
}

float readLeafWetness() {
  int raw = analogRead(LEAF_WETNESS_PIN);
  return map(raw, 4095, 0, 0, 100);
}

float readLight() {
  int raw = analogRead(LDR_PIN);
  return map(raw, 0, 4095, 0, 100);
}

float readWindSpeed() {
  int raw = analogRead(WIND_POT_PIN);
  return map(raw, 0, 4095, 0, 500) / 10.0;
}

int readWindDirection() {
  int raw = analogRead(WIND_DIR_PIN);
  return map(raw, 0, 4095, 0, 360);
}

String getWindCardinal(int degrees) {
  if (degrees < 23 || degrees >= 338) return "N";
  if (degrees < 68) return "NE";
  if (degrees < 113) return "E";
  if (degrees < 158) return "SE";
  if (degrees < 203) return "S";
  if (degrees < 248) return "SW";
  if (degrees < 293) return "W";
  return "NW";
}

float readWaterLevel() {
  digitalWrite(WATER_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(WATER_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(WATER_TRIG_PIN, LOW);
  
  long duration = pulseIn(WATER_ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.034 / 2.0;
  
  if (distance > 100 || distance == 0) distance = 100;
  return 100.0 - distance;
}

float readGas() {
  int raw = analogRead(GAS_PIN);
  return map(raw, 0, 4095, 50, 1000);
}

float readCO2() {
  int raw = analogRead(CO2_PIN);
  return map(raw, 0, 4095, 400, 2000);
}

float readCO() {
  int raw = analogRead(CO_PIN);
  return map(raw, 0, 4095, 0, 200);
}

bool readMotion() {
  return digitalRead(MOTION_PIN) == HIGH;
}

float readWeight() {
  if (scale.is_ready()) {
    float weight = scale.get_units(5);
    return weight > 0 ? weight : 0;
  }
  return 0;
}

// ==================== STATUS FUNCTIONS ====================

String getMoistureStatus(float moisture) {
  if (moisture < 20) return "Dry";
  if (moisture < 40) return "Low";
  if (moisture < 60) return "Good";
  if (moisture < 80) return "Moist";
  return "Wet";
}

String getTempStatus(float temp) {
  if (temp < 10) return "Cold";
  if (temp < 20) return "Cool";
  if (temp < 30) return "Optimal";
  if (temp < 40) return "Warm";
  return "Hot";
}

String getPHStatus(float ph) {
  if (ph < 6.0) return "Acidic";
  if (ph < 7.5) return "Optimal";
  return "Alkaline";
}

String getLightStatus(float light) {
  if (light < 20) return "Dark";
  if (light < 40) return "Dim";
  if (light < 70) return "Bright";
  return "Very Bright";
}

String getGasStatus(float gas) {
  if (gas < 300) return "Clean";
  if (gas < 500) return "Moderate";
  return "Danger!";
}

String getCO2Status(float co2) {
  if (co2 < 800) return "Good";
  if (co2 < 1200) return "Moderate";
  return "Poor!";
}

String getCOStatus(float co) {
  if (co < 35) return "Safe";
  if (co < 100) return "Caution";
  return "Danger!";
}

// ==================== MAIN LOOP ====================

void loop() {
  unsigned long currentTime = millis();
  
  // Update sensor readings
  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = currentTime;
    
    // Read all sensors
    float soilMoist = readSoilMoisture();
    float soilTemp = readSoilTemp();
    float soilPH = readSoilPH();
    float leafTemp = readLeafTemp();
    float leafWet = readLeafWetness();
    float airTemp = dht.readTemperature();
    float humidity = dht.readHumidity();
    float light = readLight();
    float windSpeed = readWindSpeed();
    int windDir = readWindDirection();
    float waterLevel = readWaterLevel();
    float gas = readGas();
    float co2 = readCO2();
    float co = readCO();
    bool motion = readMotion();
    float weight = readWeight();
    
    // Control LEDs and alerts
    digitalWrite(LED_SOIL_PIN, soilMoist < 20 ? HIGH : LOW);
    digitalWrite(LED_GAS_PIN, (gas > 500 || co2 > 1200 || co > 100) ? HIGH : LOW);
    digitalWrite(LED_MOTION_PIN, motion ? HIGH : LOW);
    
    // Heartbeat
    static bool heartbeat = false;
    heartbeat = !heartbeat;
    digitalWrite(LED_SYSTEM_PIN, heartbeat ? HIGH : LOW);
    
    // Buzzer for critical alerts
    if (gas > 500 || co > 100 || soilMoist < 15) {
      tone(BUZZER_PIN, 1000, 200);
    }
    
    // Print to Serial
    Serial.println("========== SENSOR READINGS ==========");
    Serial.printf("Soil Moisture: %.1f%% - %s\n", soilMoist, getMoistureStatus(soilMoist).c_str());
    Serial.printf("Soil Temp: %.1f°C - %s\n", soilTemp, getTempStatus(soilTemp).c_str());
    Serial.printf("Soil pH: %.1f - %s\n", soilPH, getPHStatus(soilPH).c_str());
    Serial.printf("Leaf Temp: %.1f°C - %s\n", leafTemp, getTempStatus(leafTemp).c_str());
    Serial.printf("Leaf Wetness: %.1f%%\n", leafWet);
    Serial.printf("Air Temp: %.1f°C - %s\n", airTemp, getTempStatus(airTemp).c_str());
    Serial.printf("Humidity: %.1f%%\n", humidity);
    Serial.printf("Light: %.1f%% - %s\n", light, getLightStatus(light).c_str());
    Serial.printf("Wind Speed: %.1f km/h\n", windSpeed);
    Serial.printf("Wind Dir: %d° (%s)\n", windDir, getWindCardinal(windDir).c_str());
    Serial.printf("Water Level: %.1f%%\n", waterLevel);
    Serial.printf("Gas: %.0f ppm - %s\n", gas, getGasStatus(gas).c_str());
    Serial.printf("CO2: %.0f ppm - %s\n", co2, getCO2Status(co2).c_str());
    Serial.printf("CO: %.0f ppm - %s\n", co, getCOStatus(co).c_str());
    Serial.printf("Motion: %s\n", motion ? "DETECTED!" : "Clear");
    Serial.printf("Weight: %.2f kg\n", weight);
    Serial.println("=====================================\n");
    
    // Update display mode
    if (currentTime - lastDisplay >= DISPLAY_INTERVAL) {
      lastDisplay = currentTime;
      displayMode = (displayMode + 1) % 8;
      
      lcd.clear();
      
      switch(displayMode) {
        case 0:
          lcd.setCursor(0, 0);
          lcd.print("Soil Moist:");
          lcd.print(soilMoist, 1);
          lcd.print("%");
          lcd.setCursor(0, 1);
          lcd.print(getMoistureStatus(soilMoist));
          lcd.setCursor(0, 2);
          lcd.print("Soil Temp:");
          lcd.print(soilTemp, 1);
          lcd.print("C");
          lcd.setCursor(0, 3);
          lcd.print("pH:");
          lcd.print(soilPH, 1);
          break;
          
        case 1:
          lcd.setCursor(0, 0);
          lcd.print("Leaf Temp:");
          lcd.print(leafTemp, 1);
          lcd.print("C");
          lcd.setCursor(0, 1);
          lcd.print(getTempStatus(leafTemp));
          lcd.setCursor(0, 2);
          lcd.print("Leaf Wet:");
          lcd.print(leafWet, 1);
          lcd.print("%");
          break;
          
        case 2:
          lcd.setCursor(0, 0);
          lcd.print("Air Temp:");
          lcd.print(airTemp, 1);
          lcd.print("C");
          lcd.setCursor(0, 1);
          lcd.print("Humidity:");
          lcd.print(humidity, 1);
          lcd.print("%");
          lcd.setCursor(0, 2);
          lcd.print("Light:");
          lcd.print(light, 1);
          lcd.print("%");
          lcd.setCursor(0, 3);
          lcd.print(getLightStatus(light));
          break;
          
        case 3:
          lcd.setCursor(0, 0);
          lcd.print("Wind Speed:");
          lcd.print(windSpeed, 1);
          lcd.setCursor(0, 1);
          lcd.print("km/h");
          lcd.setCursor(0, 2);
          lcd.print("Direction:");
          lcd.print(windDir);
          lcd.print((char)223);
          lcd.setCursor(0, 3);
          lcd.print(getWindCardinal(windDir));
          break;
          
        case 4:
          lcd.setCursor(0, 0);
          lcd.print("Water Level:");
          lcd.print(waterLevel, 1);
          lcd.print("%");
          lcd.setCursor(0, 2);
          lcd.print("Weight:");
          lcd.print(weight, 1);
          lcd.print("kg");
          break;
          
        case 5:
          lcd.setCursor(0, 0);
          lcd.print("Gas:");
          lcd.print(gas, 0);
          lcd.print(" ppm");
          lcd.setCursor(0, 1);
          lcd.print(getGasStatus(gas));
          lcd.setCursor(0, 2);
          lcd.print("CO2:");
          lcd.print(co2, 0);
          lcd.print(" ppm");
          lcd.setCursor(0, 3);
          lcd.print(getCO2Status(co2));
          break;
          
        case 6:
          lcd.setCursor(0, 0);
          lcd.print("CO:");
          lcd.print(co, 0);
          lcd.print(" ppm");
          lcd.setCursor(0, 1);
          lcd.print(getCOStatus(co));
          lcd.setCursor(0, 2);
          lcd.print("Motion:");
          lcd.print(motion ? "DETECTED!" : "Clear");
          break;
          
        case 7:
          lcd.setCursor(0, 0);
          lcd.print("   System Status");
          lcd.setCursor(0, 1);
          if (gas > 500 || co > 100) {
            lcd.print("   ** ALERT **");
          } else if (soilMoist < 20) {
            lcd.print("  Low Moisture");
          } else if (motion) {
            lcd.print(" Motion Detected");
          } else {
            lcd.print("  All Systems OK");
          }
          lcd.setCursor(0, 3);
          lcd.print("  Smart Farm IoT");
          break;
      }
    }
  }
}
