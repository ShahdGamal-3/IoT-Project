/*
 * CO2Sensor.h
 * Driver for MQ135 CO2 Sensor
 * 
 * Features:
 * - Analog CO2 concentration reading
 * - Detection of CO2, NH3, NOx, alcohol, benzene, smoke
 * - CO2 level in ppm estimation
 * - Air quality status and warnings
 */

#ifndef CO2SENSOR_H
#define CO2SENSOR_H

#include <Arduino.h>

class CO2Sensor {
private:
    uint8_t analogPin;
    int rawValue;
    float co2PPM;
    int samples;

public:
    // Constructor
    CO2Sensor(uint8_t analogPin, int samples = 10);
    
    // Initialize the sensor
    void begin();
    
    // Read CO2 concentration
    float readCO2();
    
    // Get air quality status
    String getAirQuality();
    
    // Check if CO2 is at dangerous level
    bool isDangerous();
    
    // Get raw ADC value
    int getRawValue();
};

#endif
