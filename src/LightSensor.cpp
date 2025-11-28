/*
 * LightSensor.cpp
 * Implementation of LDR light sensor driver
 */

#include "LightSensor.h"

// Constructor
LightSensor::LightSensor(uint8_t pin) {
    this->pin = pin;
    this->rawValue = 0;
    this->lightPercent = 0.0;
    this->darkValue = 0;      // Calibration for dark (low light)
    this->brightValue = 4095; // Calibration for bright (full light)
}

// Initialize the sensor
void LightSensor::begin() {
    pinMode(pin, INPUT);
}

// Read light intensity from sensor
float LightSensor::readLight() {
    rawValue = analogRead(pin);
    
    // Map ADC value to percentage (0-100%)
    // Higher ADC value = more light for typical LDR circuit
    lightPercent = map(rawValue, darkValue, brightValue, 0, 100);
    
    // Constrain to valid range
    if (lightPercent < 0) lightPercent = 0;
    if (lightPercent > 100) lightPercent = 100;
    
    return lightPercent;
}

// Get light intensity as percentage
float LightSensor::getLightPercent() {
    return lightPercent;
}

// Get raw ADC value
int LightSensor::getRawValue() {
    return rawValue;
}

// Get light condition status
String LightSensor::getLightStatus() {
    if (lightPercent < 10) {
        return "Very Dark";
    } else if (lightPercent < 25) {
        return "Dark";
    } else if (lightPercent < 40) {
        return "Dim";
    } else if (lightPercent < 60) {
        return "Moderate";
    } else if (lightPercent < 75) {
        return "Bright";
    } else if (lightPercent < 90) {
        return "Very Bright";
    } else {
        return "Intense";
    }
}

// Calibration methods
void LightSensor::calibrateDark(int value) {
    darkValue = value;
}

void LightSensor::calibrateBright(int value) {
    brightValue = value;
}

// Check if it's dark
bool LightSensor::isDark() {
    return lightPercent < 25;
}
