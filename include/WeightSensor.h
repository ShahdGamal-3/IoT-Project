/*
 * WeightSensor.h
 * Driver for HX711 Load Cell Weight Sensor
 * 
 * Features:
 * - Weight measurement using HX711 and load cell
 * - Calibration support
 * - Weight in kg and pounds
 * - Tare function
 * - Feed/grain inventory monitoring
 */

#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H

#include <Arduino.h>
#include <HX711.h>

class WeightSensor {
private:
    HX711 scale;
    uint8_t dataPin;
    uint8_t clockPin;
    float weight_kg;
    float calibrationFactor;
    float maxCapacity_kg;

public:
    // Constructor
    WeightSensor(uint8_t dataPin, uint8_t clockPin, float calibrationFactor = 2280.0, float maxCapacity_kg = 200.0);
    
    // Initialize the sensor
    void begin();
    
    // Tare/zero the scale
    void tare();
    
    // Read weight
    float readWeight();
    
    // Get weight in kg
    float getWeight_kg();
    
    // Get weight in pounds
    float getWeight_lbs();
    
    // Get weight status
    String getWeightStatus();
    
    // Set calibration factor
    void setCalibrationFactor(float factor);
    
    // Check if overloaded
    bool isOverloaded();
};

#endif
