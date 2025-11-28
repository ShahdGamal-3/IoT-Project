/*
 * WaterTankSensor.cpp
 * Implementation of HC-SR04 Ultrasonic Water Tank Level Sensor driver
 */

#include "WaterTankSensor.h"

// Constructor
WaterTankSensor::WaterTankSensor(uint8_t trigPin, uint8_t echoPin, float tankHeight_cm, float tankCapacity_liters) {
    this->trigPin = trigPin;
    this->echoPin = echoPin;
    this->tankHeight_cm = tankHeight_cm;
    this->tankCapacity_liters = tankCapacity_liters;
    this->distance_cm = 0.0;
    this->waterLevel_cm = 0.0;
    this->waterLevel_percent = 0.0;
    this->waterVolume_liters = 0.0;
}

// Initialize the sensor
void WaterTankSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.println("[WaterTank] HC-SR04 Water Tank Level Sensor initialized");
    Serial.printf("[WaterTank] Tank: %.0f cm height, %.0f L capacity\n", tankHeight_cm, tankCapacity_liters);
}

// Measure distance using ultrasonic sensor
float WaterTankSensor::measureDistance() {
    // Send 10us pulse to trigger
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Read echo pulse duration
    long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
    
    // Calculate distance in cm (speed of sound = 343 m/s or 0.0343 cm/us)
    // Distance = (duration * 0.0343) / 2
    float distance = duration * 0.01715;
    
    // Limit to reasonable range (2cm to 400cm)
    if (distance < 2.0 || distance > 400.0) {
        distance = 0.0;
    }
    
    return distance;
}

// Read water level
float WaterTankSensor::readLevel() {
    // Measure distance from sensor to water surface
    distance_cm = measureDistance();
    
    if (distance_cm > 0) {
        // Calculate water level (tank height - distance from top)
        waterLevel_cm = tankHeight_cm - distance_cm;
        
        // Ensure water level is not negative
        if (waterLevel_cm < 0) {
            waterLevel_cm = 0;
        }
        
        // Calculate water level percentage
        waterLevel_percent = (waterLevel_cm / tankHeight_cm) * 100.0;
        
        // Ensure percentage is within 0-100%
        waterLevel_percent = constrain(waterLevel_percent, 0.0, 100.0);
        
        // Calculate water volume in liters
        waterVolume_liters = (waterLevel_percent / 100.0) * tankCapacity_liters;
    } else {
        // Sensor error
        waterLevel_cm = 0.0;
        waterLevel_percent = 0.0;
        waterVolume_liters = 0.0;
    }
    
    return waterLevel_cm;
}

// Get distance from sensor to water
float WaterTankSensor::getDistance_cm() {
    return distance_cm;
}

// Get water level in cm
float WaterTankSensor::getLevel_cm() {
    return waterLevel_cm;
}

// Get water level in percentage
float WaterTankSensor::getLevel_percent() {
    return waterLevel_percent;
}

// Get water volume in liters
float WaterTankSensor::getVolume_liters() {
    return waterVolume_liters;
}

// Get tank status
String WaterTankSensor::getTankStatus() {
    if (waterLevel_percent < 10) {
        return "Critical Low";
    } else if (waterLevel_percent < 25) {
        return "Low";
    } else if (waterLevel_percent < 50) {
        return "Medium";
    } else if (waterLevel_percent < 75) {
        return "Good";
    } else if (waterLevel_percent < 90) {
        return "High";
    } else {
        return "Full";
    }
}

// Check if water is low
bool WaterTankSensor::isLowLevel() {
    return waterLevel_percent < 25.0;
}

// Set tank height
void WaterTankSensor::setTankHeight(float height_cm) {
    tankHeight_cm = height_cm;
}

// Set tank capacity
void WaterTankSensor::setTankCapacity(float capacity_liters) {
    tankCapacity_liters = capacity_liters;
}
