/*
 * LightSensor.h
 * Driver for LDR (Light Dependent Resistor) light intensity sensor
 * 
 * Features:
 * - Analog reading of light intensity
 * - Percentage conversion (0-100%)
 * - Light condition categorization
 * - Calibration support
 */

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <Arduino.h>

class LightSensor {
private:
    uint8_t pin;
    int rawValue;
    float lightPercent;
    int darkValue;    // ADC value in dark
    int brightValue;  // ADC value in bright light

public:
    // Constructor
    LightSensor(uint8_t pin);
    
    // Initialize the sensor
    void begin();
    
    // Read light intensity from sensor
    float readLight();
    
    // Get light intensity as percentage
    float getLightPercent();
    
    // Get raw ADC value
    int getRawValue();
    
    // Get light condition status
    String getLightStatus();
    
    // Calibration methods
    void calibrateDark(int value);
    void calibrateBright(int value);
    
    // Check if it's dark
    bool isDark();
};

#endif
