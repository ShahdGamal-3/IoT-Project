/*
 * ESP32 #1: SOIL NODE (Slave)
 * Smart Monitoring Farm - Distributed ESP-NOW & Firebase Architecture
 * 
 * Functions:
 * - Soil Moisture Regulation Monitoring
 * - Soil Thermal Monitoring
 * - Soil Nutrient Availability (pH) Monitoring
 * 
 * Communication: ESP-NOW (Send to Gateway)
 */

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ============================================
// CONFIGURATION - REPLACE WITH YOUR GATEWAY MAC ADDRESS
// ============================================
// To get MAC address: Upload this sketch to Gateway ESP32 first,
// then check Serial Monitor for MAC address on boot
uint8_t gatewayAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// ============================================
// PIN DEFINITIONS
// ============================================
#define SOIL_MOISTURE_PIN 34  // Analog pin for soil moisture sensor
#define SOIL_PH_PIN 35        // Analog pin for soil pH sensor
#define SOIL_TEMP_PIN 15      // Digital pin for DS18B20 temperature sensor

// ============================================
// SENSOR SETUP
// ============================================
OneWire oneWire(SOIL_TEMP_PIN);
DallasTemperature soilTempSensor(&oneWire);

// ============================================
// DATA STRUCTURE FOR ESP-NOW
// ============================================
typedef struct struct_soil_message {
  char nodeId[20];
  float soilMoisture;      // Percentage (0-100%)
  float soilPH;            // pH value (0-14)
  float soilTemp;          // Temperature in Celsius
  unsigned long timestamp; // Milliseconds since boot
} struct_soil_message;

struct_soil_message soilData;
esp_now_peer_info_t peerInfo;

// ============================================
// SENSOR CALIBRATION VALUES
// ============================================
// Calibrate these values based on your actual sensors
const int MOISTURE_DRY = 4095;   // ADC value when sensor is dry
const int MOISTURE_WET = 0;      // ADC value when sensor is wet
const float PH_MIN = 4.0;        // Minimum pH value
const float PH_MAX = 9.0;        // Maximum pH value

// ============================================
// TIMING CONFIGURATION
// ============================================
const unsigned long SEND_INTERVAL = 5000;  // Send data every 5 seconds
unsigned long lastSendTime = 0;

// ============================================
// ESP-NOW CALLBACKS
// ============================================
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\n[ESP-NOW] Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "✓ Success" : "✗ Failed");
}

// ============================================
// SENSOR READING FUNCTIONS
// ============================================

float readSoilMoisture() {
  int rawValue = analogRead(SOIL_MOISTURE_PIN);
  
  // Map ADC value to percentage (0-100%)
  float moisture = map(rawValue, MOISTURE_DRY, MOISTURE_WET, 0, 100);
  
  // Constrain to valid range
  moisture = constrain(moisture, 0, 100);
  
  return moisture;
}

float readSoilPH() {
  int rawValue = analogRead(SOIL_PH_PIN);
  
  // Map ADC value to pH scale (typically 0-14, but we focus on 4-9 for soil)
  float ph = map(rawValue, 0, 4095, PH_MIN * 100, PH_MAX * 100) / 100.0;
  
  // Constrain to valid range
  ph = constrain(ph, 0, 14);
  
  return ph;
}

float readSoilTemp() {
  soilTempSensor.requestTemperatures();
  float temp = soilTempSensor.getTempCByIndex(0);
  
  // Check if reading is valid
  if (temp == DEVICE_DISCONNECTED_C) {
    Serial.println("[WARNING] Soil temperature sensor disconnected!");
    return -127.0; // Error value
  }
  
  return temp;
}

// ============================================
// SETUP
// ============================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\r\n\r\n╔════════════════════════════════════════╗");
  Serial.println("║   ESP32 SOIL NODE - Initializing...   ║");
  Serial.println("╚════════════════════════════════════════╝\r\n");
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
  // Print MAC Address
  Serial.print("[WiFi] This Device MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERROR] ESP-NOW initialization failed!");
    return;
  }
  Serial.println("[ESP-NOW] ✓ Initialized successfully");
  
  // Register send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register Gateway peer
  memcpy(peerInfo.peer_addr, gatewayAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[ERROR] Failed to add Gateway peer!");
    return;
  }
  Serial.println("[ESP-NOW] ✓ Gateway peer registered");
  
  // Initialize sensors
  soilTempSensor.begin();
  Serial.println("[Sensors] ✓ DS18B20 initialized");
  
  // Set node ID
  strcpy(soilData.nodeId, "SOIL_NODE");
  
  Serial.println("\r\n╔════════════════════════════════════════╗");
  Serial.println("║      SOIL NODE Ready - Monitoring     ║");
  Serial.println("╚════════════════════════════════════════╝\r\n");
}

// ============================================
// MAIN LOOP
// ============================================
void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastSendTime >= SEND_INTERVAL) {
    lastSendTime = currentTime;
    
    // Read all sensor data
    soilData.soilMoisture = readSoilMoisture();
    soilData.soilPH = readSoilPH();
    soilData.soilTemp = readSoilTemp();
    soilData.timestamp = currentTime;
    
    // Print data to Serial Monitor
    Serial.println("\r\n┌──────────────────────────────────────┐");
    Serial.println("│       SOIL NODE - Sensor Data       │");
    Serial.println("├──────────────────────────────────────┤");
    Serial.printf("│ Soil Moisture:    %6.2f %%         │\r\n", soilData.soilMoisture);
    Serial.printf("│ Soil pH:          %6.2f            │\r\n", soilData.soilPH);
    Serial.printf("│ Soil Temperature: %6.2f °C         │\r\n", soilData.soilTemp);
    Serial.printf("│ Timestamp:        %lu ms          │\r\n", soilData.timestamp);
    Serial.println("└──────────────────────────────────────┘");
    
    // Interpret soil conditions
    Serial.println("\r\n[Analysis]");
    
    // Moisture analysis
    if (soilData.soilMoisture < 20) {
      Serial.println("  ⚠ Soil is DRY - Irrigation recommended");
    } else if (soilData.soilMoisture > 80) {
      Serial.println("  ⚠ Soil is TOO WET - Check drainage");
    } else {
      Serial.println("  ✓ Soil moisture is optimal");
    }
    
    // pH analysis
    if (soilData.soilPH < 6.0) {
      Serial.println("  ⚠ Soil is ACIDIC - Consider adding lime");
    } else if (soilData.soilPH > 7.5) {
      Serial.println("  ⚠ Soil is ALKALINE - Consider adding sulfur");
    } else {
      Serial.println("  ✓ Soil pH is optimal for most crops");
    }
    
    // Temperature analysis
    if (soilData.soilTemp < 10) {
      Serial.println("  ⚠ Soil is COLD - Plant growth may be slow");
    } else if (soilData.soilTemp > 30) {
      Serial.println("  ⚠ Soil is HOT - Monitor moisture closely");
    } else {
      Serial.println("  ✓ Soil temperature is optimal");
    }
    
    // Send message via ESP-NOW to Gateway
    Serial.println("\r\n[ESP-NOW] Transmitting to Gateway...");
    esp_err_t result = esp_now_send(gatewayAddress, (uint8_t *) &soilData, sizeof(soilData));
    
    if (result != ESP_OK) {
      Serial.println("[ERROR] Failed to send data packet!");
    }
  }
  
  delay(100); // Small delay to prevent watchdog issues
}
