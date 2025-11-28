/*
 * WaterTankSensor.h
 * Driver for HC-SR04 Ultrasonic Water Tank Level Sensor
 * 
 * Features:
 * - Distance measurement using HC-SR04 ultrasonic sensor
 * - Water level calculation in cm and percentage
 * - Tank capacity management
 * - Low water level warning
 * - Real ultrasonic distance measurement
 */

#ifndef WATERTANKSENSOR_H
#define WATERTANKSENSOR_H

#include <Arduino.h>

class WaterTankSensor {
private:
    uint8_t trigPin;
    uint8_t echoPin;
    float tankHeight_cm;
    float tankCapacity_liters;
    float distance_cm;  // Distance from sensor to water surface
    float waterLevel_cm;
    float waterLevel_percent;
    float waterVolume_liters;
    
    // Measure distance using ultrasonic sensor
    float measureDistance();

public:
    // Constructor
    WaterTankSensor(uint8_t trigPin, uint8_t echoPin, float tankHeight_cm = 100.0, float tankCapacity_liters = 1000.0);
    
    // Initialize the sensor
    void begin();
    
    // Read water level
    float readLevel();
    
    // Get distance from sensor to water
    float getDistance_cm();
    
    // Get water level in cm
    float getLevel_cm();
    
    // Get water level in percentage
    float getLevel_percent();
    
    // Get water volume in liters
    float getVolume_liters();
    
    // Get tank status
    String getTankStatus();
    
    // Check if water is low
    bool isLowLevel();
    
    // Set tank parameters
    void setTankHeight(float height_cm);
    void setTankCapacity(float capacity_liters);
};

#endif
