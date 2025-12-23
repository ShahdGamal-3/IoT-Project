#ifndef CONFIG_H
#define CONFIG_H

// ==================== NODE IDENTITY ====================
#define NODE_ID 1
#define NODE_NAME "Soil_Node"

// ==================== ESP-NOW CONFIGURATION ====================
#define ESPNOW_CHANNEL 1
#define MAX_RETRY 3
#define ESPNOW_TIMEOUT 5000

// Gateway MAC Address (Update with actual gateway MAC)
#define GATEWAY_MAC {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // Broadcast initially

// ==================== SENSOR PINS ====================
// Soil Moisture Sensor (Analog)
#define SOIL_MOISTURE_PIN 34

// DS18B20 Soil Temperature (OneWire)
#define SOIL_TEMP_PIN 4

// pH Sensor (Analog)
#define SOIL_PH_PIN 35

// Leaf Temperature Sensor (Analog or Digital)
#define LEAF_TEMP_PIN 32

// Leaf Wetness Sensor (Analog)
#define LEAF_WETNESS_PIN 33

// Status LED
#define LED_PIN 2

// ==================== TIMING CONFIGURATION ====================
#define READING_INTERVAL 5000       // Read sensors every 5 seconds
#define SEND_INTERVAL 5000          // Send data every 5 seconds
#define SLEEP_ENABLED false         // Enable sleep mode for battery operation
#define SLEEP_DURATION_MS 300000    // Sleep for 5 minutes (if enabled)

// ==================== SENSOR CALIBRATION ====================
// Soil Moisture Calibration
#define SOIL_MOISTURE_MIN 0         // Sensor value in air
#define SOIL_MOISTURE_MAX 4095      // Sensor value in water (12-bit ADC)
#define SOIL_MOISTURE_DRY 3000      // Dry soil threshold
#define SOIL_MOISTURE_WET 1000      // Wet soil threshold

// pH Sensor Calibration
#define PH_SENSOR_MIN 0
#define PH_SENSOR_MAX 4095
#define PH_VOLTAGE_MIN 0.0
#define PH_VOLTAGE_MAX 3.3
#define PH_NEUTRAL_VOLTAGE 1.65     // Voltage at pH 7
#define PH_ACID_VOLTAGE 2.03        // Voltage at pH 4
#define PH_ALKALINE_VOLTAGE 1.27    // Voltage at pH 10

// Leaf Wetness Calibration
#define LEAF_WETNESS_DRY 3500       // Sensor value when dry
#define LEAF_WETNESS_WET 1000       // Sensor value when wet

// Temperature Calibration
#define TEMP_OFFSET 0.0             // Temperature offset correction

// ==================== THRESHOLDS ====================
// Soil Moisture Thresholds
#define MOISTURE_LOW 30.0           // Low moisture warning (%)
#define MOISTURE_OPTIMAL_MIN 40.0   // Optimal range start
#define MOISTURE_OPTIMAL_MAX 70.0   // Optimal range end
#define MOISTURE_HIGH 80.0          // High moisture warning (%)

// Temperature Thresholds
#define TEMP_LOW 15.0               // Low temperature alert (°C)
#define TEMP_OPTIMAL_MIN 20.0       // Optimal range start
#define TEMP_OPTIMAL_MAX 30.0       // Optimal range end
#define TEMP_HIGH 35.0              // High temperature alert (°C)

// pH Thresholds
#define PH_LOW 5.5                  // Acidic alert
#define PH_OPTIMAL_MIN 6.0          // Optimal range start
#define PH_OPTIMAL_MAX 7.5          // Optimal range end
#define PH_HIGH 8.0                 // Alkaline alert

// Leaf Wetness Thresholds
#define LEAF_WETNESS_THRESHOLD 50.0 // Alert if above (%)

// ==================== SYSTEM CONFIGURATION ====================
#define SERIAL_BAUD_RATE 115200
#define DEBUG_MODE true

// Number of samples for averaging
#define SAMPLE_COUNT 10
#define SAMPLE_DELAY_MS 50

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
    ERROR_TEMP_SENSOR = 4,
    ERROR_MOISTURE_SENSOR = 5,
    ERROR_PH_SENSOR = 6
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

#endif // CONFIG_H
