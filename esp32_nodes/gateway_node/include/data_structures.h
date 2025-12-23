#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdint.h>

// ==================== PACKET HEADER ====================
struct PacketHeader {
    uint8_t nodeId;          // Node identifier (1=Soil, 2=Weather, 3=Gateway)
    uint8_t packetType;      // Type of data packet
    uint32_t timestamp;      // Unix timestamp
    uint16_t sequence;       // Packet sequence number
    uint8_t checksum;        // Data integrity check
} __attribute__((packed));

// ==================== SOIL NODE DATA ====================
struct SoilNodeData {
    PacketHeader header;
    float soilMoisture;      // 0-100%
    float soilTemp;          // Celsius
    float soilPH;            // 0-14
    float leafTemp;          // Celsius
    float leafWetness;       // 0-100%
} __attribute__((packed));

// ==================== WEATHER NODE DATA ====================
struct WeatherNodeData {
    PacketHeader header;
    float airTemp;           // Celsius
    float humidity;          // 0-100%
    uint16_t light;          // Lux (0-1023)
    float rainfall;          // mm
    float windSpeed;         // m/s
    uint16_t windDirection;  // 0-360 degrees
    uint16_t gas;            // PPM
    uint16_t co2;            // PPM
    uint16_t co;             // PPM
} __attribute__((packed));

// ==================== GATEWAY LOCAL DATA ====================
struct GatewayLocalData {
    float waterLevel;        // 0-100%
    bool motion;             // true/false
    float weight;            // kg
    uint32_t lastMotionTime; // Timestamp of last motion detection
} __attribute__((packed));

// ==================== AGGREGATED SENSOR DATA ====================
struct AllSensorData {
    // Soil Node Data
    float soilMoisture;
    float soilTemp;
    float soilPH;
    float leafTemp;
    float leafWetness;
    
    // Weather Node Data
    float airTemp;
    float humidity;
    uint16_t light;
    float rainfall;
    float windSpeed;
    uint16_t windDirection;
    uint16_t gas;
    uint16_t co2;
    uint16_t co;
    
    // Gateway Local Data
    float waterLevel;
    bool motion;
    float weight;
    
    // Metadata
    uint32_t timestamp;
    bool soilNodeConnected;
    bool weatherNodeConnected;
};

// ==================== ALERT STRUCTURE ====================
struct Alert {
    char message[64];
    uint8_t severity;  // 0=Info, 1=Warning, 2=Critical
    uint32_t timestamp;
};

// ==================== STATISTICS ====================
struct NodeStatistics {
    uint32_t packetsReceived;
    uint32_t packetsLost;
    uint32_t lastRSSI;
    uint32_t uptimeSeconds;
};

#endif // DATA_STRUCTURES_H
