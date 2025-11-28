/*
 * RainfallSensor.h
 * Driver for Rain Gauge/Rain Sensor
 * 
 * Features:
 * - Analog rainfall sensor simulation
 * - Continuous rainfall measurement
 * - Rainfall calculation in mm and inches
 * - Rain rate calculation (mm/hour)
 * - Potentiometer simulation for Wokwi testing
 */

#ifndef RAINFALLSENSOR_H
#define RAINFALLSENSOR_H

#include <Arduino.h>

class RainfallSensor {
private:
    uint8_t analogPin;
    float rainfall_mm;        // Total rainfall in mm
    float rainRate;           // Rainfall rate in mm/min
    unsigned long lastReading;

public:
    // Constructor
    RainfallSensor(uint8_t analogPin);
    
    // Initialize the sensor
    void begin();
    
    // Update rainfall calculations (call periodically)
    void update();
    
    // Get total rainfall in mm
    float getRainfall_mm();
    
    // Get total rainfall in inches
    float getRainfall_inches();
    
    // Get rain rate
    float getRainRate();
    
    // Get rain status
    String getRainStatus();
    
    // Get rain intensity description
    String getRainIntensity();
};

#endif
