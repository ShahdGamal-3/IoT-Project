#ifndef CONFIG_H
#define CONFIG_H

// ==================== NODE IDENTITY ====================
#define NODE_ID 3
#define NODE_NAME "Gateway_Node"

// ==================== WiFi CONFIGURATION ====================
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_TIMEOUT 10000  // 10 seconds

// ==================== FIREBASE CONFIGURATION ====================
#define FIREBASE_HOST "smartmonitoringfarm-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyCERYj-T7s_ohva1CSUrjfqIaNr5QgCkfU"
#define FIREBASE_DATABASE_URL "https://smartmonitoringfarm-default-rtdb.firebaseio.com/"

// ==================== ESP-NOW CONFIGURATION ====================
#define ESPNOW_CHANNEL 1
#define MAX_RETRY 3
#define ESPNOW_TIMEOUT 5000

// ESP-NOW Peer MAC Addresses (Update with actual node MAC addresses)
#define SOIL_NODE_MAC {0x24, 0x6F, 0x28, 0x12, 0x34, 0x56}  // Replace with actual
#define WEATHER_NODE_MAC {0x24, 0x6F, 0x28, 0x78, 0x9A, 0xBC}  // Replace with actual

// ==================== SENSOR PINS ====================
// HX711 Load Cell
#define HX711_DOUT_PIN 16
#define HX711_SCK_PIN 4

// LCD I2C
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// PIR Motion Sensor
#define PIR_PIN 13

// Ultrasonic Sensor (Water Level)
#define TRIG_PIN 26
#define ECHO_PIN 27

// Status LED
#define LED_PIN 2

// ==================== TIMING CONFIGURATION ====================
#define READING_INTERVAL 5000       // Read sensors every 5 seconds
#define FIREBASE_UPLOAD_INTERVAL 5000  // Upload to Firebase every 5 seconds
#define LCD_UPDATE_INTERVAL 1000    // Update LCD every second
#define RECONNECT_INTERVAL 30000    // Attempt reconnection every 30 seconds

// ==================== SENSOR CALIBRATION ====================
// Load Cell Calibration
#define SCALE_CALIBRATION_FACTOR 420.0  // Adjust based on calibration
#define SCALE_ZERO_OFFSET 0.0

// Water Level Calibration
#define TANK_HEIGHT_CM 100.0        // Tank height in cm
#define MIN_WATER_LEVEL_CM 5.0      // Minimum measurable water level
#define MAX_WATER_LEVEL_CM 95.0     // Maximum water level

// ==================== THRESHOLDS & ALERTS ====================
// Water Level Thresholds
#define WATER_LEVEL_LOW 20.0        // Low water alert threshold (%)
#define WATER_LEVEL_CRITICAL 10.0   // Critical water alert (%)

// Weight Thresholds
#define WEIGHT_MIN 0.0              // Minimum expected weight (kg)
#define WEIGHT_MAX 100.0            // Maximum expected weight (kg)

// Motion Detection
#define MOTION_TIMEOUT 60000        // Reset motion flag after 60 seconds

// ==================== SYSTEM CONFIGURATION ====================
#define SERIAL_BAUD_RATE 115200
#define DEBUG_MODE true

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
    ERROR_WIFI = 1,
    ERROR_FIREBASE = 2,
    ERROR_SENSOR = 3,
    ERROR_ESPNOW = 4,
    ERROR_MEMORY = 5,
    ERROR_LCD = 6
};

// ==================== SYSTEM STATES ====================
enum SystemState {
    STATE_INIT,
    STATE_CONFIG,
    STATE_READY,
    STATE_READ_SENSORS,
    STATE_RECEIVE_DATA,
    STATE_UPLOAD,
    STATE_DISPLAY,
    STATE_ERROR
};

#endif // CONFIG_H
