/*
 * ESP32 #3: GATEWAY NODE (Master)
 * Smart Monitoring Farm - Distributed ESP-NOW & Firebase Architecture
 * * Functions:
 * - Receive data from Soil and Weather nodes via ESP-NOW
 * - Water Resource Availability Monitoring (Ultrasonic)
 * - Fire & Hazardous Gas Detection
 * - Occupational Safety (CO, CO2) Monitoring
 * - Motion Detection & Security
 * - Weight Monitoring (HX711)
 * - Data aggregation and Firebase upload
 * - LCD Display & Alert System
 * * Communication: 
 * - ESP-NOW (Receive from Slaves)
 * - WiFi/Firebase (Send to Cloud)
 */

#include <esp_now.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <Arduino.h>

// ============================================
// FIREBASE CONFIGURATION
// ============================================
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// Replace with your Firebase project credentials
#define FIREBASE_HOST "smartmonitoringfarm.firebaseio.com"
#define API_KEY "AIzaSyCERYj-T7s_ohva1CSUrjfqIaNr5QgCkfU"
#define DATABASE_URL "https://smartmonitoringfarm-default-rtdb.firebaseio.com/"
#define USER_EMAIL "shahdgamal1977@gmail.com"
#define USER_PASSWORD "12345678"

// ============================================
// PIN DEFINITIONS - Gateway Sensors
// ============================================
#define TRIG_PIN 12           // Ultrasonic trigger
#define ECHO_PIN 13           // Ultrasonic echo
#define GAS_PIN 35            // Gas sensor analog (CHANGED from 4 to 35)
// CHANGED: Moved from 0/2 (Strapping pins) to 32/33 (Safe ADC pins)
#define CO2_PIN 32            
#define CO_PIN 33             
#define PIR_PIN 19            // Motion sensor digital
#define HX711_DT 25           // Weight scale data (CHANGED from 5 to 25)
#define HX711_SCK 18          // Weight scale clock
#define BUZZER_PIN 23         // Alert buzzer
#define LED_SOIL 16           // Soil alert LED
#define LED_GAS 17            // Gas alert LED
// CHANGED: Moved from 9/10 (Flash Memory pins) to 26/27 (Safe GPIOs)
#define LED_MOTION 26         
#define LED_OK 27             

// ============================================
// SENSOR SETUP
// ============================================
LiquidCrystal_I2C lcd(0x27, 20, 4);
HX711 scale;

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ============================================
// DATA STRUCTURES
// ============================================
typedef struct soil_data {
  char nodeId[20];
  float soilMoisture;
  float soilPH;
  float soilTemp;
  unsigned long timestamp;
} soil_data;

typedef struct weather_data {
  char nodeId[20];
  float leafWetness;
  float leafTemp;
  float airTemp;
  float humidity;
  float lightIntensity;
  float windSpeed;
  float windDirection;
  float rainfall;
  unsigned long timestamp;
} weather_data;

soil_data receivedSoilData;
weather_data receivedWeatherData;

bool soilDataReceived = false;
bool weatherDataReceived = false;

// ============================================
// SIMULATION MODE - Initialize with test data
// ============================================
void initializeTestData() {
  // Initialize Soil Node data (for testing without ESP-NOW)
  strcpy(receivedSoilData.nodeId, "SOIL_NODE_SIM");
  receivedSoilData.soilMoisture = 65.0;  // Good moisture level
  receivedSoilData.soilPH = 6.5;          // Neutral pH
  receivedSoilData.soilTemp = 22.0;       // Room temperature
  receivedSoilData.timestamp = millis();
  soilDataReceived = true;
  
  // Initialize Weather Node data (for testing without ESP-NOW)
  strcpy(receivedWeatherData.nodeId, "WEATHER_NODE_SIM");
  receivedWeatherData.leafWetness = 45.0;
  receivedWeatherData.leafTemp = 21.0;
  receivedWeatherData.airTemp = 25.0;
  receivedWeatherData.humidity = 60.0;
  receivedWeatherData.lightIntensity = 500.0;
  receivedWeatherData.windSpeed = 3.5;
  receivedWeatherData.windDirection = 180.0;
  receivedWeatherData.rainfall = 2.5;
  receivedWeatherData.timestamp = millis();
  weatherDataReceived = true;
}

// ============================================
// ALERT THRESHOLDS
// ============================================
const float MOISTURE_LOW = 30.0;
const float GAS_HIGH = 500.0;
const float CO2_HIGH = 1000.0;
const float CO_HIGH = 50.0;
const float WATER_LOW = 10.0;  // cm
const int TANK_HEIGHT = 200;   // cm (total tank height)

// ============================================
// TIMING
// ============================================
unsigned long lastFirebaseUpdate = 0;
const unsigned long FIREBASE_INTERVAL = 30000;  // Upload every 30 seconds (30000ms)
unsigned long lastLCDUpdate = 0;
const unsigned long LCD_INTERVAL = 2000;  // Update LCD every 2 seconds
int lcdPage = 0;

// ============================================
// ESP-NOW CALLBACK - RECEIVE DATA
// ============================================
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  char nodeId[20];
  memcpy(nodeId, incomingData, 20);
  
  if (strcmp(nodeId, "SOIL_NODE") == 0) {
    memcpy(&receivedSoilData, incomingData, sizeof(receivedSoilData));
    soilDataReceived = true;
    
    Serial.println("\r\n┌──────────────────────────────────────┐");
    Serial.println("│   RECEIVED: Soil Node Data          │");
    Serial.println("├──────────────────────────────────────┤");
    Serial.printf("│ Moisture: %6.2f %%                  │\r\n", receivedSoilData.soilMoisture);
    Serial.printf("│ pH:       %6.2f                     │\r\n", receivedSoilData.soilPH);
    Serial.printf("│ Temp:     %6.2f °C                  │\r\n", receivedSoilData.soilTemp);
    Serial.println("└──────────────────────────────────────┘");
    
    // Check soil alerts
    if (receivedSoilData.soilMoisture < MOISTURE_LOW) {
      digitalWrite(LED_SOIL, HIGH);
      Serial.println("[ALERT] ⚠ Low soil moisture!");
    } else {
      digitalWrite(LED_SOIL, LOW);
    }
  }
  else if (strcmp(nodeId, "WEATHER_NODE") == 0) {
    memcpy(&receivedWeatherData, incomingData, sizeof(receivedWeatherData));
    weatherDataReceived = true;
    
    Serial.println("\r\n┌──────────────────────────────────────┐");
    Serial.println("│   RECEIVED: Weather Node Data       │");
    Serial.println("├──────────────────────────────────────┤");
    Serial.printf("│ Air Temp: %6.2f °C                  │\r\n", receivedWeatherData.airTemp);
    Serial.printf("│ Humidity: %6.2f %%                  │\r\n", receivedWeatherData.humidity);
    Serial.printf("│ Light:    %6.0f lux                 │\r\n", receivedWeatherData.lightIntensity);
    Serial.printf("│ Wind:     %6.2f m/s                 │\r\n", receivedWeatherData.windSpeed);
    Serial.println("└──────────────────────────────────────┘");
  }
}


// ============================================
// GATEWAY SENSOR FUNCTIONS
// ============================================

float readWaterLevel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.034 / 2;  // cm
  
  if (distance == 0 || distance > 400) {
    Serial.println("[DEBUG] Water Level: ERROR - Out of range\r\n");
    return -1;  // Error
  }
  
  // Calculate water level (tank height - distance from sensor)
  float waterLevel = TANK_HEIGHT - distance;
  waterLevel = max(0.0f, waterLevel);
  Serial.printf("[DEBUG] Water Level: %.1f cm (distance: %.1f cm)\r\n", waterLevel, distance);
  return waterLevel;
}

float readGasSensor() {
  int rawValue = analogRead(GAS_PIN);
  float gasLevel = map(rawValue, 0, 4095, 0, 1000);
  Serial.printf("[DEBUG] Gas Sensor: %.0f (raw: %d)\r\n", gasLevel, rawValue);
  return gasLevel;
}

float readCO2() {
  int rawValue = analogRead(CO2_PIN);
  float co2Level = map(rawValue, 0, 4095, 400, 5000);  // ppm
  Serial.printf("[DEBUG] CO2 Level: %.0f ppm (raw: %d)\r\n", co2Level, rawValue);
  return co2Level;
}

float readCO() {
  int rawValue = analogRead(CO_PIN);
  float coLevel = map(rawValue, 0, 4095, 0, 200);  // ppm
  Serial.printf("[DEBUG] CO Level: %.0f ppm (raw: %d)\r\n", coLevel, rawValue);
  return coLevel;
}

bool readMotion() {
  bool motion = digitalRead(PIR_PIN);
  Serial.printf("[DEBUG] Motion Sensor: %s\r\n", motion ? "DETECTED" : "None");
  return motion;
}

float readWeight() {
  if (scale.is_ready()) {
    float weight = scale.get_units(5);  // Average of 5 readings
    Serial.printf("[DEBUG] Weight: %.2f kg\r\n", weight);
    return weight;
  }
  Serial.println("[DEBUG] Weight: Scale not ready\r\n");
  return 0;
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
      lcd.printf("Moist: %.1f%%", receivedSoilData.soilMoisture);
      lcd.setCursor(0, 2);
      lcd.printf("pH: %.2f", receivedSoilData.soilPH);
      lcd.setCursor(0, 3);
      lcd.printf("Temp: %.1fC", receivedSoilData.soilTemp);
      break;
      
    case 1:  // Weather Data
      lcd.print("== WEATHER DATA ==");
      lcd.setCursor(0, 1);
      lcd.printf("Air: %.1fC H:%.0f%%", receivedWeatherData.airTemp, receivedWeatherData.humidity);
      lcd.setCursor(0, 2);
      lcd.printf("Light: %.0f lux", receivedWeatherData.lightIntensity);
      lcd.setCursor(0, 3);
      lcd.printf("Wind: %.1f m/s", receivedWeatherData.windSpeed);
      break;
      
    case 2:  // Gateway Sensors
      lcd.print("== SAFETY DATA ==");
      lcd.setCursor(0, 1);
      lcd.printf("Water: %.1f cm", readWaterLevel());
      lcd.setCursor(0, 2);
      lcd.printf("Gas: %.0f", readGasSensor());
      lcd.setCursor(0, 3);
      lcd.printf("CO2: %.0f CO: %.0f", readCO2(), readCO());
      break;
  }
  
  lcdPage = (lcdPage + 1) % 3;
}

// ============================================
// FIREBASE FUNCTIONS
// ============================================
void uploadToFirebase() {
  if (!Firebase.ready()) {
    Serial.println("[Firebase] Not ready yet...");
    return;
  }
  
  // Read Gateway sensor data first
  float waterLevel = readWaterLevel();
  float gasLevel = readGasSensor();
  float co2Level = readCO2();
  float coLevel = readCO();
  bool motion = readMotion();
  float weight = readWeight();
  
  // Print formatted sensor data
  Serial.println("\r\n┌────────────────────────────────────────┐");
  Serial.println("│    GATEWAY NODE - Sensor Data         │");
  Serial.println("├──────────────────────────────────────┤");
  Serial.printf("│ Water Level:      %6.1f cm           │\r\n", waterLevel);
  Serial.printf("│ Gas Sensor:       %6.0f ppm          │\r\n", gasLevel);
  Serial.printf("│ CO2 Level:        %6.0f ppm          │\r\n", co2Level);
  Serial.printf("│ CO Level:         %6.0f ppm          │\r\n", coLevel);
  Serial.printf("│ Motion:           %s                 │\r\n", motion ? "DETECTED" : "None    ");
  Serial.printf("│ Weight:           %6.2f kg           │\r\n", weight);
  Serial.println("└──────────────────────────────────────┘");
  
  Serial.println("\r\n[Firebase] Uploading aggregated data...");
  
  // Create timestamp
  String timestamp = String(millis());
  
  // Upload Soil Node data
  if (soilDataReceived) {
    Firebase.setFloat(fbdo, "/sensors/soil/moisture", receivedSoilData.soilMoisture);
    Firebase.setFloat(fbdo, "/sensors/soil/ph", receivedSoilData.soilPH);
    Firebase.setFloat(fbdo, "/sensors/soil/temperature", receivedSoilData.soilTemp);
  }
  
  // Upload Weather Node data
  if (weatherDataReceived) {
    Firebase.setFloat(fbdo, "/sensors/weather/airTemp", receivedWeatherData.airTemp);
    Firebase.setFloat(fbdo, "/sensors/weather/humidity", receivedWeatherData.humidity);
    Firebase.setFloat(fbdo, "/sensors/weather/leafWetness", receivedWeatherData.leafWetness);
    Firebase.setFloat(fbdo, "/sensors/weather/light", receivedWeatherData.lightIntensity);
    Firebase.setFloat(fbdo, "/sensors/weather/windSpeed", receivedWeatherData.windSpeed);
    Firebase.setFloat(fbdo, "/sensors/weather/windDirection", receivedWeatherData.windDirection);
    Firebase.setFloat(fbdo, "/sensors/weather/rainfall", receivedWeatherData.rainfall);
  }
  
  // Upload Gateway sensor data (already read above)
  Firebase.setFloat(fbdo, "/sensors/gateway/waterLevel", waterLevel);
  Firebase.setFloat(fbdo, "/sensors/gateway/gas", gasLevel);
  Firebase.setFloat(fbdo, "/sensors/gateway/co2", co2Level);
  Firebase.setFloat(fbdo, "/sensors/gateway/co", coLevel);
  Firebase.setBool(fbdo, "/sensors/gateway/motion", motion);
  Firebase.setFloat(fbdo, "/sensors/gateway/weight", weight);
  
  // Upload alert status
  Firebase.setBool(fbdo, "/alerts/soilMoistureLow", receivedSoilData.soilMoisture < MOISTURE_LOW);
  Firebase.setBool(fbdo, "/alerts/gasHigh", gasLevel > GAS_HIGH);
  Firebase.setBool(fbdo, "/alerts/co2High", co2Level > CO2_HIGH);
  Firebase.setBool(fbdo, "/alerts/coHigh", coLevel > CO_HIGH);
  Firebase.setBool(fbdo, "/alerts/waterLow", waterLevel < WATER_LOW);
  Firebase.setBool(fbdo, "/alerts/motionDetected", motion);
  
  // Update timestamp
  Firebase.setString(fbdo, "/system/lastUpdate", timestamp);
  
  Serial.println("[Firebase] ✓ Data uploaded successfully");
}

// ============================================
// ALERT SYSTEM
// ============================================
void checkAlerts() {
  bool alertActive = false;
  
  // Check all alert conditions
  if (receivedSoilData.soilMoisture < MOISTURE_LOW) {
    digitalWrite(LED_SOIL, HIGH);
    alertActive = true;
  } else {
    digitalWrite(LED_SOIL, LOW);
  }
  
  float gasLevel = readGasSensor();
  float co2Level = readCO2();
  float coLevel = readCO();
  
  if (gasLevel > GAS_HIGH || co2Level > CO2_HIGH || coLevel > CO_HIGH) {
    digitalWrite(LED_GAS, HIGH);
    alertActive = true;
  } else {
    digitalWrite(LED_GAS, LOW);
  }
  
  if (readMotion()) {
    digitalWrite(LED_MOTION, HIGH);
  } else {
    digitalWrite(LED_MOTION, LOW);
  }
  
  // System OK LED (inverse of alert)
  digitalWrite(LED_OK, !alertActive ? HIGH : LOW);
  
  // Buzzer for critical alerts
  static unsigned long buzzerStartTime = 0;
  static bool buzzerOn = false;
  
  if (gasLevel > GAS_HIGH || co2Level > CO2_HIGH || coLevel > CO_HIGH) {
    unsigned long currentTime = millis();
    
    if (!buzzerOn && (currentTime - buzzerStartTime > 1000)) {
      // Turn buzzer ON for 500ms
      ledcWriteTone(0, 1000);  // 1kHz tone
      buzzerOn = true;
      buzzerStartTime = currentTime;
    } else if (buzzerOn && (currentTime - buzzerStartTime > 500)) {
      // Turn buzzer OFF for 500ms
      ledcWriteTone(0, 0);  // Stop tone
      buzzerOn = false;
      buzzerStartTime = currentTime;
    }
  } else {
    ledcWriteTone(0, 0);  // Ensure buzzer is off when no alert
    buzzerOn = false;
  }
}

// ============================================
// SETUP
// ============================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\r\n\n╔══════════════════════════════════════╗");
  Serial.println("║  ESP32 GATEWAY - Initializing...       ║");
  Serial.println("╚════════════════════════════════════════╝\r\n");
  
  // Pin setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_SOIL, OUTPUT);
  pinMode(LED_GAS, OUTPUT);
  pinMode(LED_MOTION, OUTPUT);
  pinMode(LED_OK, OUTPUT);
  
  // Configure LEDC for buzzer (channel 0, 1kHz, 8-bit resolution)
  ledcSetup(0, 1000, 8);
  ledcAttachPin(BUZZER_PIN, 0);
  
  // Initialize all LEDs OFF
  digitalWrite(LED_SOIL, LOW);
  digitalWrite(LED_GAS, LOW);
  digitalWrite(LED_MOTION, LOW);
  digitalWrite(LED_OK, HIGH);
  
  // Initialize test data for simulation (since ESP-NOW is disabled)
  initializeTestData();
  Serial.println("[SIMULATION] Test data initialized for soil and weather nodes");
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Gateway Booting...");
  
  // Initialize HX711
  scale.begin(HX711_DT, HX711_SCK);
  scale.set_scale(12387.f);  // Calibration factor adjusted for Wokwi simulation (was 2280)
  scale.tare();
  Serial.println("[DEBUG] HX711 Scale initialized and tared\r\n");
  
  // Print MAC Address
  Serial.print("[WiFi] This Device MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println("[INFO] Copy this MAC and update slave nodes!");
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERROR] ESP-NOW initialization failed!");
    return;
  }
  Serial.println("[ESP-NOW] ✓ Initialized successfully");
  
  // Register receive callback
  esp_now_register_recv_cb(OnDataRecv);
  
  Serial.println("[ESP-NOW] Skipped for simulation mode");
  
  // Connect to WiFi
  Serial.print("[WiFi] Connecting to ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\r\n[WiFi] ✓ Connected!");
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
    
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
    Serial.println("\r\n[WiFi] ✗ Connection failed!");
    Serial.println("[INFO] Continuing without cloud connectivity...");
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GATEWAY READY");
  
  Serial.println("\r\n╔════════════════════════════════════════╗");
  Serial.println("║     GATEWAY NODE Ready - Listening     ║");
  Serial.println("╚════════════════════════════════════════╝\r\n");
}

// ============================================
// MAIN LOOP
// ============================================
void loop() {
  unsigned long currentTime = millis();
  
  // Update LCD periodically
  if (currentTime - lastLCDUpdate >= LCD_INTERVAL) {
    lastLCDUpdate = currentTime;
    updateLCD();
  }
  
  // Check alerts
  checkAlerts();
  
  // Upload to Firebase periodically
  if (WiFi.status() == WL_CONNECTED && currentTime - lastFirebaseUpdate >= FIREBASE_INTERVAL) {
    lastFirebaseUpdate = currentTime;
    uploadToFirebase();
  }
  
  delay(100);
}