/*
 * ESP32 #2: WEATHER NODE (Slave)
 * Smart Monitoring Farm - Distributed ESP-NOW & Firebase Architecture
 * 
 * Functions:
 * - Fungal Disease Risk Prediction (Leaf Wetness)
 * - Photosynthetic Light Monitoring
 * - Spray Drift Management (Wind Profiling)
 * - Environmental Monitoring (Air Temp/Humidity)
 * - Rainfall Monitoring
 * 
 * Communication: ESP-NOW (Send to Gateway)
 */

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>

// ============================================
// CONFIGURATION - REPLACE WITH YOUR GATEWAY MAC ADDRESS
// ============================================
uint8_t gatewayAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// ============================================
// PIN DEFINITIONS
// ============================================
#define LEAF_WETNESS_PIN 32   // Analog pin for leaf wetness sensor
#define LEAF_TEMP_PIN 35      // Analog pin for leaf temperature sensor
#define DHT_PIN 25            // Digital pin for DHT22 sensor
#define LDR_PIN 33            // Analog pin for light sensor
#define WIND_SPEED_PIN 36     // Analog pin for wind speed sensor
#define WIND_DIR_PIN 39       // Analog pin for wind direction sensor
#define RAINFALL_PIN 39       // Analog pin for rainfall sensor

// ============================================
// SENSOR SETUP
// ============================================
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

// ============================================
// DATA STRUCTURE FOR ESP-NOW
// ============================================
typedef struct struct_weather_message {
  char nodeId[20];
  float leafWetness;      // Percentage (0-100%)
  float leafTemp;         // Temperature in Celsius
  float airTemp;          // Air temperature in Celsius
  float humidity;         // Relative humidity (0-100%)
  float lightIntensity;   // Light intensity (0-1000 lux approx)
  float windSpeed;        // Wind speed (m/s)
  float windDirection;    // Wind direction (0-360 degrees)
  float rainfall;         // Rainfall (mm)
  unsigned long timestamp;
} struct_weather_message;

struct_weather_message weatherData;
esp_now_peer_info_t peerInfo;

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

float readLeafWetness() {
  int rawValue = analogRead(LEAF_WETNESS_PIN);
  float wetness = map(rawValue, 0, 4095, 0, 100);
  return constrain(wetness, 0, 100);
}

float readLeafTemp() {
  int rawValue = analogRead(LEAF_TEMP_PIN);
  // Map to typical leaf temperature range (-10 to 50°C)
  float temp = map(rawValue, 0, 4095, -10, 50);
  return temp;
}

float readLightIntensity() {
  int rawValue = analogRead(LDR_PIN);
  // Map to approximate lux values (0-1000 lux for this example)
  float lux = map(rawValue, 0, 4095, 0, 1000);
  return lux;
}

float readWindSpeed() {
  int rawValue = analogRead(WIND_SPEED_PIN);
  // Map to wind speed in m/s (0-30 m/s)
  float speed = map(rawValue, 0, 4095, 0, 30);
  return speed;
}

float readWindDirection() {
  int rawValue = analogRead(WIND_DIR_PIN);
  // Map to degrees (0-360)
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
  // Map to rainfall in mm (0-100 mm)
  float rain = map(rawValue, 0, 4095, 0, 100);
  return rain;
}

// ============================================
// SETUP
// ============================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\r\n\r\n╔════════════════════════════════════════╗");
  Serial.println("║  ESP32 WEATHER NODE - Initializing... ║");
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
  dht.begin();
  Serial.println("[Sensors] ✓ DHT22 initialized");
  
  // Set node ID
  strcpy(weatherData.nodeId, "WEATHER_NODE");
  
  Serial.println("\r\n╔════════════════════════════════════════╗");
  Serial.println("║    WEATHER NODE Ready - Monitoring    ║");
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
    weatherData.leafWetness = readLeafWetness();
    weatherData.leafTemp = readLeafTemp();
    weatherData.airTemp = dht.readTemperature();
    weatherData.humidity = dht.readHumidity();
    weatherData.lightIntensity = readLightIntensity();
    weatherData.windSpeed = readWindSpeed();
    weatherData.windDirection = readWindDirection();
    weatherData.rainfall = readRainfall();
    weatherData.timestamp = currentTime;
    
    // Check for DHT22 reading errors
    if (isnan(weatherData.airTemp) || isnan(weatherData.humidity)) {
      Serial.println("[WARNING] Failed to read from DHT sensor!");
      weatherData.airTemp = -999;
      weatherData.humidity = -999;
    }
    
    // Print data to Serial Monitor
    Serial.println("\r\n┌──────────────────────────────────────────┐");
    Serial.println("│      WEATHER NODE - Sensor Data         │");
    Serial.println("├──────────────────────────────────────────┤");
    Serial.printf("│ Leaf Wetness:     %6.2f %%             │\r\n", weatherData.leafWetness);
    Serial.printf("│ Leaf Temperature: %6.2f °C             │\r\n", weatherData.leafTemp);
    Serial.printf("│ Air Temperature:  %6.2f °C             │\r\n", weatherData.airTemp);
    Serial.printf("│ Humidity:         %6.2f %%             │\r\n", weatherData.humidity);
    Serial.printf("│ Light Intensity:  %6.0f lux            │\r\n", weatherData.lightIntensity);
    Serial.printf("│ Wind Speed:       %6.2f m/s            │\r\n", weatherData.windSpeed);
    Serial.printf("│ Wind Direction:   %6.1f° (%s)          │\r\n", 
                  weatherData.windDirection, 
                  getWindDirectionName(weatherData.windDirection).c_str());
    Serial.printf("│ Rainfall:         %6.2f mm             │\r\n", weatherData.rainfall);
    Serial.printf("│ Timestamp:        %lu ms              │\r\n", weatherData.timestamp);
    Serial.println("└──────────────────────────────────────────┘");
    
    // Environmental analysis
    Serial.println("\r\n[Analysis]");
    
    // Leaf wetness & disease risk
    if (weatherData.leafWetness > 80) {
      Serial.println("  ⚠ HIGH FUNGAL DISEASE RISK - Leaves very wet");
    } else if (weatherData.leafWetness > 50) {
      Serial.println("  ⚠ MODERATE FUNGAL DISEASE RISK - Monitor closely");
    } else {
      Serial.println("  ✓ Low disease risk - Leaves relatively dry");
    }
    
    // Light conditions
    if (weatherData.lightIntensity < 200) {
      Serial.println("  ⚠ LOW LIGHT - Photosynthesis limited");
    } else if (weatherData.lightIntensity > 800) {
      Serial.println("  ✓ EXCELLENT LIGHT - Optimal photosynthesis");
    } else {
      Serial.println("  ✓ ADEQUATE LIGHT for plant growth");
    }
    
    // Wind conditions for spraying
    if (weatherData.windSpeed > 15) {
      Serial.println("  ⚠ HIGH WIND - DO NOT SPRAY (drift risk)");
    } else if (weatherData.windSpeed > 10) {
      Serial.println("  ⚠ MODERATE WIND - Spraying not recommended");
    } else if (weatherData.windSpeed < 3) {
      Serial.println("  ⚠ LOW WIND - Spraying may have reduced coverage");
    } else {
      Serial.println("  ✓ IDEAL WIND CONDITIONS for spraying");
    }
    
    // Rainfall
    if (weatherData.rainfall > 50) {
      Serial.println("  ⚠ HEAVY RAIN - Field operations suspended");
    } else if (weatherData.rainfall > 10) {
      Serial.println("  ⚠ MODERATE RAIN - Limit field access");
    }
    
    // Temperature & humidity
    if (weatherData.airTemp > 35) {
      Serial.println("  ⚠ HIGH TEMPERATURE - Heat stress possible");
    } else if (weatherData.airTemp < 5) {
      Serial.println("  ⚠ LOW TEMPERATURE - Frost risk");
    }
    
    if (weatherData.humidity < 30) {
      Serial.println("  ⚠ LOW HUMIDITY - Increase irrigation");
    } else if (weatherData.humidity > 80) {
      Serial.println("  ⚠ HIGH HUMIDITY - Monitor for disease");
    }
    
    // Send message via ESP-NOW to Gateway
    Serial.println("\r\n[ESP-NOW] Transmitting to Gateway...");
    esp_err_t result = esp_now_send(gatewayAddress, (uint8_t *) &weatherData, sizeof(weatherData));
    
    if (result != ESP_OK) {
      Serial.println("[ERROR] Failed to send data packet!");
    }
  }
  
  delay(100); // Small delay to prevent watchdog issues
}
