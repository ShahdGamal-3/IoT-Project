/*
 * SMART MONITORING FARM - MERGED WOKWI VERSION
 * 
 * This is a merged version of all three ESP32 nodes (Gateway, Soil, Weather)
 * combined into a single file for Wokwi simulation.
 * 
 * Features Combined:
 * - Gateway Node: Firebase, LCD, Water Level, Gas/CO/CO2, Motion, Weight, LEDs, Buzzer
 * - Soil Node: Soil Moisture, pH, Temperature
 * - Weather Node: Leaf Wetness/Temp, Air Temp/Humidity, Light, Wind, Rainfall
 * 
 * Note: ESP-NOW functionality is simulated with local variables
 */

#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Arduino.h>

// ============================================
// FIREBASE CONFIGURATION
// ============================================
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

#define FIREBASE_HOST "smartmonitoringfarm.firebaseio.com"
#define API_KEY "AIzaSyCERYj-T7s_ohva1CSUrjfqIaNr5QgCkfU"
#define DATABASE_URL "https://smartmonitoringfarm-default-rtdb.firebaseio.com/"
#define USER_EMAIL "shahdgamal1977@gmail.com"
#define USER_PASSWORD "12345678"

// ============================================
// PIN DEFINITIONS - ALL SENSORS
// ============================================
// Gateway Node Sensors
#define TRIG_PIN 12           // Ultrasonic trigger
#define ECHO_PIN 13           // Ultrasonic echo
#define GAS_PIN 35            // Gas sensor analog
#define CO2_PIN 32            // CO2 sensor analog
#define CO_PIN 33             // CO sensor analog
#define PIR_PIN 19            // Motion sensor digital
#define HX711_DT 25           // Weight scale data
#define HX711_SCK 18          // Weight scale clock
#define BUZZER_PIN 23         // Alert buzzer
#define LED_SOIL 16           // Soil alert LED
#define LED_GAS 17            // Gas alert LED
#define LED_MOTION 26         // Motion LED
#define LED_OK 27             // System OK LED

// Soil Node Sensors
#define SOIL_MOISTURE_PIN 34  // Soil moisture sensor
#define SOIL_PH_PIN 35        // Soil pH sensor (shared with GAS_PIN - alternate reading)
#define SOIL_TEMP_PIN 15      // DS18B20 temperature sensor

// Weather Node Sensors
#define LEAF_WETNESS_PIN 32   // Leaf wetness sensor (shared with CO2_PIN)
#define LEAF_TEMP_PIN 35      // Leaf temperature sensor (shared with GAS_PIN/PH)
#define DHT_PIN 25            // DHT22 sensor (shared with HX711_DT)
#define LDR_PIN 33            // Light sensor (shared with CO_PIN)
#define WIND_SPEED_PIN 36     // Wind speed sensor
#define WIND_DIR_PIN 39       // Wind direction sensor
#define RAINFALL_PIN 39       // Rainfall sensor (shared with WIND_DIR)

// ============================================
// SENSOR SETUP
// ============================================
LiquidCrystal_I2C lcd(0x27, 20, 4);
HX711 scale;
OneWire oneWire(SOIL_TEMP_PIN);
DallasTemperature soilTempSensor(&oneWire);
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ============================================
// DATA STRUCTURES
// ============================================
struct SoilData {
  float soilMoisture;
  float soilPH;
  float soilTemp;
  unsigned long timestamp;
} soilData;

struct WeatherData {
  float leafWetness;
  float leafTemp;
  float airTemp;
  float humidity;
  float lightIntensity;
  float windSpeed;
  float windDirection;
  float rainfall;
  unsigned long timestamp;
} weatherData;

struct GatewayData {
  float waterLevel;
  float gasLevel;
  float co2Level;
  float coLevel;
  bool motion;
  float weight;
  unsigned long timestamp;
} gatewayData;

// ============================================
// ALERT THRESHOLDS
// ============================================
const float MOISTURE_LOW = 30.0;
const float GAS_HIGH = 500.0;
const float CO2_HIGH = 1000.0;
const float CO_HIGH = 50.0;
const float WATER_LOW = 10.0;
const int TANK_HEIGHT = 200;

// ============================================
// TIMING CONFIGURATION
// ============================================
const unsigned long SENSOR_READ_INTERVAL = 5000;   // Read sensors every 5 seconds
const unsigned long FIREBASE_INTERVAL = 30000;      // Upload every 30 seconds
const unsigned long LCD_INTERVAL = 2000;            // Update LCD every 2 seconds

unsigned long lastSensorRead = 0;
unsigned long lastFirebaseUpdate = 0;
unsigned long lastLCDUpdate = 0;
int lcdPage = 0;

// Calibration values for soil sensors
const int MOISTURE_DRY = 4095;
const int MOISTURE_WET = 0;
const float PH_MIN = 4.0;
const float PH_MAX = 9.0;

// ============================================
// SOIL NODE SENSOR FUNCTIONS
// ============================================
float readSoilMoisture() {
  int rawValue = analogRead(SOIL_MOISTURE_PIN);
  float moisture = map(rawValue, MOISTURE_DRY, MOISTURE_WET, 0, 100);
  moisture = constrain(moisture, 0, 100);
  return moisture;
}

float readSoilPH() {
  // Note: In actual implementation, you'd need to switch analog readings
  // For Wokwi, we'll use a different approach
  int rawValue = analogRead(SOIL_PH_PIN);
  float ph = map(rawValue, 0, 4095, PH_MIN * 100, PH_MAX * 100) / 100.0;
  ph = constrain(ph, 0, 14);
  return ph;
}

float readSoilTemp() {
  soilTempSensor.requestTemperatures();
  float temp = soilTempSensor.getTempCByIndex(0);
  if (temp == DEVICE_DISCONNECTED_C) {
    return 22.0; // Default value
  }
  return temp;
}

// ============================================
// WEATHER NODE SENSOR FUNCTIONS
// ============================================
float readLeafWetness() {
  int rawValue = analogRead(LEAF_WETNESS_PIN);
  float wetness = map(rawValue, 0, 4095, 0, 100);
  return constrain(wetness, 0, 100);
}

float readLeafTemp() {
  int rawValue = analogRead(LEAF_TEMP_PIN);
  float temp = map(rawValue, 0, 4095, -10, 50);
  return temp;
}

float readLightIntensity() {
  int rawValue = analogRead(LDR_PIN);
  float lux = map(rawValue, 0, 4095, 0, 1000);
  return lux;
}

float readWindSpeed() {
  int rawValue = analogRead(WIND_SPEED_PIN);
  float speed = map(rawValue, 0, 4095, 0, 30);
  return speed;
}

float readWindDirection() {
  int rawValue = analogRead(WIND_DIR_PIN);
  float direction = map(rawValue, 0, 4095, 0, 360);
  return direction;
}

String getWindDirectionName(float degrees) {
  if (degrees >= 337.5 || degrees < 22.5) return "N";
  else if (degrees >= 22.5 && degrees < 67.5) return "NE";
  else if (degrees >= 67.5 && degrees < 112.5) return "E";
  else if (degrees >= 112.5 && degrees < 157.5) return "SE";
  else if (degrees >= 157.5 && degrees < 202.5) return "S";
  else if (degrees >= 202.5 && degrees < 247.5) return "SW";
  else if (degrees >= 247.5 && degrees < 292.5) return "W";
  else return "NW";
}

float readRainfall() {
  int rawValue = analogRead(RAINFALL_PIN);
  float rain = map(rawValue, 0, 4095, 0, 100);
  return rain;
}

// ============================================
// GATEWAY NODE SENSOR FUNCTIONS
// ============================================
float readWaterLevel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.034 / 2;
  
  if (distance == 0 || distance > 400) {
    return -1;
  }
  
  float waterLevel = TANK_HEIGHT - distance;
  waterLevel = max(0.0f, waterLevel);
  return waterLevel;
}

float readGasSensor() {
  int rawValue = analogRead(GAS_PIN);
  float gasLevel = map(rawValue, 0, 4095, 0, 1000);
  return gasLevel;
}

float readCO2() {
  int rawValue = analogRead(CO2_PIN);
  float co2Level = map(rawValue, 0, 4095, 400, 5000);
  return co2Level;
}

float readCO() {
  int rawValue = analogRead(CO_PIN);
  float coLevel = map(rawValue, 0, 4095, 0, 200);
  return coLevel;
}

bool readMotion() {
  return digitalRead(PIR_PIN);
}

float readWeight() {
  if (scale.is_ready()) {
    float weight = scale.get_units(5);
    return weight;
  }
  return 0;
}

// ============================================
// READ ALL SENSORS
// ============================================
void readAllSensors() {
  Serial.println("\n╔════════════════════════════════════════════════════╗");
  Serial.println("║      READING ALL SENSORS - MERGED SYSTEM          ║");
  Serial.println("╚════════════════════════════════════════════════════╝\n");
  
  unsigned long currentTime = millis();
  
  // Read Soil Sensors
  Serial.println("┌─────── SOIL NODE DATA ───────┐");
  soilData.soilMoisture = readSoilMoisture();
  soilData.soilPH = readSoilPH();
  soilData.soilTemp = readSoilTemp();
  soilData.timestamp = currentTime;
  
  Serial.printf("│ Moisture:    %6.2f %%       │\n", soilData.soilMoisture);
  Serial.printf("│ pH:          %6.2f          │\n", soilData.soilPH);
  Serial.printf("│ Temperature: %6.2f °C       │\n", soilData.soilTemp);
  Serial.println("└──────────────────────────────┘\n");
  
  // Read Weather Sensors
  Serial.println("┌─────── WEATHER NODE DATA ────────┐");
  weatherData.leafWetness = readLeafWetness();
  weatherData.leafTemp = readLeafTemp();
  weatherData.airTemp = dht.readTemperature();
  weatherData.humidity = dht.readHumidity();
  weatherData.lightIntensity = readLightIntensity();
  weatherData.windSpeed = readWindSpeed();
  weatherData.windDirection = readWindDirection();
  weatherData.rainfall = readRainfall();
  weatherData.timestamp = currentTime;
  
  // Handle DHT errors
  if (isnan(weatherData.airTemp)) weatherData.airTemp = 25.0;
  if (isnan(weatherData.humidity)) weatherData.humidity = 50.0;
  
  Serial.printf("│ Leaf Wetness: %6.2f %%       │\n", weatherData.leafWetness);
  Serial.printf("│ Leaf Temp:    %6.2f °C       │\n", weatherData.leafTemp);
  Serial.printf("│ Air Temp:     %6.2f °C       │\n", weatherData.airTemp);
  Serial.printf("│ Humidity:     %6.2f %%       │\n", weatherData.humidity);
  Serial.printf("│ Light:        %6.0f lux      │\n", weatherData.lightIntensity);
  Serial.printf("│ Wind Speed:   %6.2f m/s      │\n", weatherData.windSpeed);
  Serial.printf("│ Wind Dir:     %6.1f° (%s)    │\n", 
                weatherData.windDirection, 
                getWindDirectionName(weatherData.windDirection).c_str());
  Serial.printf("│ Rainfall:     %6.2f mm       │\n", weatherData.rainfall);
  Serial.println("└──────────────────────────────────┘\n");
  
  // Read Gateway Sensors
  Serial.println("┌─────── GATEWAY NODE DATA ────────┐");
  gatewayData.waterLevel = readWaterLevel();
  gatewayData.gasLevel = readGasSensor();
  gatewayData.co2Level = readCO2();
  gatewayData.coLevel = readCO();
  gatewayData.motion = readMotion();
  gatewayData.weight = readWeight();
  gatewayData.timestamp = currentTime;
  
  Serial.printf("│ Water Level:  %6.1f cm       │\n", gatewayData.waterLevel);
  Serial.printf("│ Gas:          %6.0f ppm      │\n", gatewayData.gasLevel);
  Serial.printf("│ CO2:          %6.0f ppm      │\n", gatewayData.co2Level);
  Serial.printf("│ CO:           %6.0f ppm      │\n", gatewayData.coLevel);
  Serial.printf("│ Motion:       %s              │\n", gatewayData.motion ? "DETECTED" : "None    ");
  Serial.printf("│ Weight:       %6.2f kg       │\n", gatewayData.weight);
  Serial.println("└──────────────────────────────────┘\n");
}

// ============================================
// LCD DISPLAY FUNCTIONS
// ============================================
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  switch (lcdPage) {
    case 0:  // Soil Data
      lcd.print("=== SOIL DATA ===");
      lcd.setCursor(0, 1);
      lcd.printf("Moist: %.1f%%", soilData.soilMoisture);
      lcd.setCursor(0, 2);
      lcd.printf("pH: %.2f", soilData.soilPH);
      lcd.setCursor(0, 3);
      lcd.printf("Temp: %.1fC", soilData.soilTemp);
      break;
      
    case 1:  // Weather Data
      lcd.print("== WEATHER DATA ==");
      lcd.setCursor(0, 1);
      lcd.printf("Air:%.1fC H:%.0f%%", weatherData.airTemp, weatherData.humidity);
      lcd.setCursor(0, 2);
      lcd.printf("Light: %.0f lux", weatherData.lightIntensity);
      lcd.setCursor(0, 3);
      lcd.printf("Wind: %.1f m/s", weatherData.windSpeed);
      break;
      
    case 2:  // Gateway Sensors 1
      lcd.print("== SAFETY DATA ==");
      lcd.setCursor(0, 1);
      lcd.printf("Water: %.1f cm", gatewayData.waterLevel);
      lcd.setCursor(0, 2);
      lcd.printf("Gas: %.0f", gatewayData.gasLevel);
      lcd.setCursor(0, 3);
      lcd.printf("CO2:%.0f CO:%.0f", gatewayData.co2Level, gatewayData.coLevel);
      break;
      
    case 3:  // Gateway Sensors 2
      lcd.print("=== MORE DATA ===");
      lcd.setCursor(0, 1);
      lcd.printf("Motion: %s", gatewayData.motion ? "YES" : "NO ");
      lcd.setCursor(0, 2);
      lcd.printf("Weight: %.2f kg", gatewayData.weight);
      lcd.setCursor(0, 3);
      lcd.printf("Leaf W: %.1f%%", weatherData.leafWetness);
      break;
  }
  
  lcdPage = (lcdPage + 1) % 4;
}

// ============================================
// ALERT SYSTEM
// ============================================
void checkAlerts() {
  bool alertActive = false;
  
  // Check soil moisture alert
  if (soilData.soilMoisture < MOISTURE_LOW) {
    digitalWrite(LED_SOIL, HIGH);
    alertActive = true;
  } else {
    digitalWrite(LED_SOIL, LOW);
  }
  
  // Check gas alerts
  if (gatewayData.gasLevel > GAS_HIGH || 
      gatewayData.co2Level > CO2_HIGH || 
      gatewayData.coLevel > CO_HIGH) {
    digitalWrite(LED_GAS, HIGH);
    alertActive = true;
  } else {
    digitalWrite(LED_GAS, LOW);
  }
  
  // Check motion
  if (gatewayData.motion) {
    digitalWrite(LED_MOTION, HIGH);
  } else {
    digitalWrite(LED_MOTION, LOW);
  }
  
  // System OK LED
  digitalWrite(LED_OK, !alertActive ? HIGH : LOW);
  
  // Buzzer for critical alerts
  static unsigned long buzzerStartTime = 0;
  static bool buzzerOn = false;
  
  if (gatewayData.gasLevel > GAS_HIGH || 
      gatewayData.co2Level > CO2_HIGH || 
      gatewayData.coLevel > CO_HIGH) {
    unsigned long currentTime = millis();
    
    if (!buzzerOn && (currentTime - buzzerStartTime > 1000)) {
      ledcWriteTone(BUZZER_PIN, 1000);  // 1kHz tone
      buzzerOn = true;
      buzzerStartTime = currentTime;
    } else if (buzzerOn && (currentTime - buzzerStartTime > 500)) {
      ledcWriteTone(BUZZER_PIN, 0);
      buzzerOn = false;
      buzzerStartTime = currentTime;
    }
  } else {
    ledcWriteTone(BUZZER_PIN, 0);
    buzzerOn = false;
  }
}

// ============================================
// FIREBASE UPLOAD
// ============================================
void uploadToFirebase() {
  if (!Firebase.ready()) {
    Serial.println("[Firebase] Not ready yet...");
    return;
  }
  
  Serial.println("\n[Firebase] Uploading aggregated data...");
  
  String timestamp = String(millis());
  
  // Upload Soil Data
  Firebase.setFloat(fbdo, "/sensors/soil/moisture", soilData.soilMoisture);
  Firebase.setFloat(fbdo, "/sensors/soil/ph", soilData.soilPH);
  Firebase.setFloat(fbdo, "/sensors/soil/temperature", soilData.soilTemp);
  
  // Upload Weather Data
  Firebase.setFloat(fbdo, "/sensors/weather/airTemp", weatherData.airTemp);
  Firebase.setFloat(fbdo, "/sensors/weather/humidity", weatherData.humidity);
  Firebase.setFloat(fbdo, "/sensors/weather/leafWetness", weatherData.leafWetness);
  Firebase.setFloat(fbdo, "/sensors/weather/leafTemp", weatherData.leafTemp);
  Firebase.setFloat(fbdo, "/sensors/weather/light", weatherData.lightIntensity);
  Firebase.setFloat(fbdo, "/sensors/weather/windSpeed", weatherData.windSpeed);
  Firebase.setFloat(fbdo, "/sensors/weather/windDirection", weatherData.windDirection);
  Firebase.setFloat(fbdo, "/sensors/weather/rainfall", weatherData.rainfall);
  
  // Upload Gateway Data
  Firebase.setFloat(fbdo, "/sensors/gateway/waterLevel", gatewayData.waterLevel);
  Firebase.setFloat(fbdo, "/sensors/gateway/gas", gatewayData.gasLevel);
  Firebase.setFloat(fbdo, "/sensors/gateway/co2", gatewayData.co2Level);
  Firebase.setFloat(fbdo, "/sensors/gateway/co", gatewayData.coLevel);
  Firebase.setBool(fbdo, "/sensors/gateway/motion", gatewayData.motion);
  Firebase.setFloat(fbdo, "/sensors/gateway/weight", gatewayData.weight);
  
  // Upload Alerts
  Firebase.setBool(fbdo, "/alerts/soilMoistureLow", soilData.soilMoisture < MOISTURE_LOW);
  Firebase.setBool(fbdo, "/alerts/gasHigh", gatewayData.gasLevel > GAS_HIGH);
  Firebase.setBool(fbdo, "/alerts/co2High", gatewayData.co2Level > CO2_HIGH);
  Firebase.setBool(fbdo, "/alerts/coHigh", gatewayData.coLevel > CO_HIGH);
  Firebase.setBool(fbdo, "/alerts/waterLow", gatewayData.waterLevel < WATER_LOW);
  Firebase.setBool(fbdo, "/alerts/motionDetected", gatewayData.motion);
  
  Firebase.setString(fbdo, "/system/lastUpdate", timestamp);
  
  Serial.println("[Firebase] ✓ Data uploaded successfully\n");
}

// ============================================
// SETUP
// ============================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n╔════════════════════════════════════════════════════╗");
  Serial.println("║   SMART MONITORING FARM - MERGED WOKWI VERSION     ║");
  Serial.println("║        All Sensors Combined - Initializing        ║");
  Serial.println("╚════════════════════════════════════════════════════╝\n");
  
  // Pin setup - Gateway
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_SOIL, OUTPUT);
  pinMode(LED_GAS, OUTPUT);
  pinMode(LED_MOTION, OUTPUT);
  pinMode(LED_OK, OUTPUT);
  
  // Configure LEDC for buzzer (ESP32 Arduino 3.x compatible)
  ledcAttach(BUZZER_PIN, 1000, 8);
  
  // Initialize LEDs
  digitalWrite(LED_SOIL, LOW);
  digitalWrite(LED_GAS, LOW);
  digitalWrite(LED_MOTION, LOW);
  digitalWrite(LED_OK, HIGH);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Farm Merged");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  // Initialize HX711
  scale.begin(HX711_DT, HX711_SCK);
  scale.set_scale(12387.f);
  scale.tare();
  Serial.println("[✓] HX711 Scale initialized");
  
  // Initialize Soil Temperature Sensor
  soilTempSensor.begin();
  Serial.println("[✓] DS18B20 Soil Temp initialized");
  
  // Initialize DHT22
  dht.begin();
  Serial.println("[✓] DHT22 Weather Sensor initialized");
  
  // Print MAC Address
  WiFi.mode(WIFI_STA);
  Serial.print("[WiFi] MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  // Connect to WiFi
  Serial.printf("[WiFi] Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] ✓ Connected!");
    Serial.printf("[WiFi] IP Address: %s\n", WiFi.localIP().toString().c_str());
    
    // Configure Firebase
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.token_status_callback = tokenStatusCallback;
    
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    
    Serial.println("[Firebase] ✓ Configured");
  } else {
    Serial.println("\n[WiFi] ✗ Connection failed!");
    Serial.println("[INFO] Continuing without cloud connectivity...");
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMART FARM READY");
  lcd.setCursor(0, 1);
  lcd.print("All Systems Online");
  
  Serial.println("\n╔════════════════════════════════════════════════════╗");
  Serial.println("║          SMART FARM READY - MONITORING            ║");
  Serial.println("╚════════════════════════════════════════════════════╝\n");
  
  delay(2000);
}

// ============================================
// MAIN LOOP
// ============================================
void loop() {
  unsigned long currentTime = millis();
  
  // Read all sensors periodically
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    lastSensorRead = currentTime;
    readAllSensors();
  }
  
  // Update LCD periodically
  if (currentTime - lastLCDUpdate >= LCD_INTERVAL) {
    lastLCDUpdate = currentTime;
    updateLCD();
  }
  
  // Check alerts continuously
  checkAlerts();
  
  // Upload to Firebase periodically
  if (WiFi.status() == WL_CONNECTED && 
      currentTime - lastFirebaseUpdate >= FIREBASE_INTERVAL) {
    lastFirebaseUpdate = currentTime;
    uploadToFirebase();
  }
  
  delay(100);
}
