/*
 * COSensor.h
 * Driver for MQ7 CO (Carbon Monoxide) Sensor
 * 
 * Features:
 * - Analog CO concentration reading
 * - Detection of carbon monoxide
 * - CO level in ppm estimation
 * - Safety status and warnings
 */

#ifndef COSENSOR_H
#define COSENSOR_H

#include <Arduino.h>

class COSensor {
private:
    uint8_t analogPin;
    int rawValue;
    float coPPM;
    int samples;

public:
    // Constructor
    COSensor(uint8_t analogPin, int samples = 10);
    
    // Initialize the sensor
    void begin();
    
    // Read CO concentration
    float readCO();
    
    // Get CO status
    String getCOStatus();
    
    // Check if CO is at dangerous level
    bool isDangerous();
    
    // Get raw ADC value
    int getRawValue();
};

#endif
