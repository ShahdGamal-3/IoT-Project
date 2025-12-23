#ifndef CONFIG_H
#define CONFIG_H

// ==================== NODE IDENTITY ====================
#define NODE_ID 2
#define NODE_NAME "Weather_Node"

// ==================== ESP-NOW CONFIGURATION ====================
#define ESPNOW_CHANNEL 1
#define MAX_RETRY 3
#define ESPNOW_TIMEOUT 5000

// Gateway MAC Address (Update with actual gateway MAC)
#define GATEWAY_MAC {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // Broadcast initially

// ==================== SENSOR PINS ====================
// DHT22 (Temperature & Humidity)
#define DHT_PIN 15
#define DHT_TYPE DHT22

// LDR (Light Sensor) - Analog
#define LDR_PIN 34

// Rainfall Sensor - Digital
#define RAINFALL_PIN 32

// Wind Speed Sensor (Anemometer) - Pulse counting
#define WIND_SPEED_PIN 25

// Wind Direction Sensor - Analog
#define WIND_DIRECTION_PIN 35

// MQ2 Gas Sensor - Analog
#define MQ2_PIN 33

// MQ135 CO2 Sensor - Analog
#define MQ135_PIN 36

// MQ7 CO Sensor - Analog
#define MQ7_PIN 39

// Status LED
#define LED_PIN 2

// ==================== TIMING CONFIGURATION ====================
#define READING_INTERVAL 5000       // Read sensors every 5 seconds
#define SEND_INTERVAL 5000          // Send data every 5 seconds
#define DHT_READ_DELAY 2000         // DHT22 minimum read interval
#define WIND_SAMPLE_PERIOD 5000     // Wind speed sample period (ms)
#define SLEEP_ENABLED false         // Enable sleep mode
#define SLEEP_DURATION_MS 300000    // Sleep for 5 minutes

// ==================== SENSOR CALIBRATION ====================
// LDR Calibration
#define LDR_MIN 0                   // Dark
#define LDR_MAX 4095                // Bright (12-bit ADC)

// Wind Speed Calibration (for anemometer)
#define ANEMOMETER_FACTOR 2.4       // km/h per Hz
#define WIND_SPEED_OFFSET 0.0       // Offset correction

// Wind Direction Calibration (resistance-based vane)
// Voltage to direction mapping
#define WIND_DIR_N 0                // North (0°)
#define WIND_DIR_NE 45              // Northeast (45°)
#define WIND_DIR_E 90               // East (90°)
#define WIND_DIR_SE 135             // Southeast (135°)
#define WIND_DIR_S 180              // South (180°)
#define WIND_DIR_SW 225             // Southwest (225°)
#define WIND_DIR_W 270              // West (270°)
#define WIND_DIR_NW 315             // Northwest (315°)

// Gas Sensor Calibration (MQ series)
#define MQ2_RL 10.0                 // Load resistance (kΩ)
#define MQ2_RO 10.0                 // Sensor resistance in clean air (kΩ)
#define MQ135_RL 10.0
#define MQ135_RO 10.0
#define MQ7_RL 10.0
#define MQ7_RO 10.0

// Gas concentration calculation constants
#define GAS_CURVE_A 116.6020682     // Curve fitting parameter
#define GAS_CURVE_B -2.769034857    // Curve fitting parameter
#define CO2_CURVE_A 110.47
#define CO2_CURVE_B -2.862
#define CO_CURVE_A 99.042
#define CO_CURVE_B -1.518

// ==================== THRESHOLDS ====================
// Temperature Thresholds (Air)
#define AIR_TEMP_LOW 10.0           // Cold alert (°C)
#define AIR_TEMP_OPTIMAL_MIN 18.0   // Comfortable range start
#define AIR_TEMP_OPTIMAL_MAX 28.0   // Comfortable range end
#define AIR_TEMP_HIGH 35.0          // Hot alert (°C)

// Humidity Thresholds
#define HUMIDITY_LOW 30.0           // Dry alert (%)
#define HUMIDITY_OPTIMAL_MIN 40.0   // Optimal range start
#define HUMIDITY_OPTIMAL_MAX 70.0   // Optimal range end
#define HUMIDITY_HIGH 80.0          // Humid alert (%)

// Light Thresholds
#define LIGHT_DARK 100              // Dark condition
#define LIGHT_LOW 300               // Low light
#define LIGHT_OPTIMAL 500           // Optimal light
#define LIGHT_BRIGHT 800            // Bright light

// Wind Speed Thresholds
#define WIND_CALM 0.5               // Calm (m/s)
#define WIND_LIGHT 3.3              // Light breeze
#define WIND_MODERATE 7.9           // Moderate wind
#define WIND_STRONG 13.8            // Strong wind
#define WIND_GALE 20.7              // Gale

// Air Quality Thresholds
#define GAS_SAFE 100                // Safe gas level (PPM)
#define GAS_WARNING 300             // Warning level
#define GAS_DANGER 500              // Dangerous level

#define CO2_NORMAL 400              // Normal CO2 (PPM)
#define CO2_WARNING 1000            // Warning level
#define CO2_DANGER 2000             // Dangerous level

#define CO_SAFE 9                   // Safe CO level (PPM)
#define CO_WARNING 35               // Warning level
#define CO_DANGER 100               // Dangerous level

// Rainfall Detection
#define RAINFALL_DETECTED LOW       // Digital pin state when raining

// ==================== SYSTEM CONFIGURATION ====================
#define SERIAL_BAUD_RATE 115200
#define DEBUG_MODE true

// Number of samples for averaging
#define SAMPLE_COUNT 10
#define SAMPLE_DELAY_MS 50

// Wind speed pulse counting
#define WIND_DEBOUNCE_MS 10

// Debug printing macro
#if DEBUG_MODE
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTF(...)
#endif

// ==================== ERROR CODES ====================
enum ErrorCode {
    ERROR_NONE = 0,
    ERROR_SENSOR = 1,
    ERROR_ESPNOW = 2,
    ERROR_MEMORY = 3,
    ERROR_DHT = 4,
    ERROR_WIND = 5,
    ERROR_GAS = 6
};

// ==================== SYSTEM STATES ====================
enum SystemState {
    STATE_INIT,
    STATE_CONFIG,
    STATE_READY,
    STATE_READ,
    STATE_PROCESS,
    STATE_SEND,
    STATE_SLEEP,
    STATE_ERROR
};

// ==================== AIR QUALITY INDEX ====================
enum AirQualityLevel {
    AQI_GOOD,           // 0-50
    AQI_MODERATE,       // 51-100
    AQI_UNHEALTHY_SG,   // 101-150 (Sensitive Groups)
    AQI_UNHEALTHY,      // 151-200
    AQI_VERY_UNHEALTHY, // 201-300
    AQI_HAZARDOUS       // 301+
};

#endif // CONFIG_H
