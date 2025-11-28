/**
 * @file SoilMoistureSensor.cpp
 * @brief Implementation of Capacitive Soil Moisture Sensor driver
 */

#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(uint8_t analogPin, int dryVal, int wetVal) {
    pin = analogPin;
    dryValue = dryVal;
    wetValue = wetVal;
    rawValue = 0;
    moisturePercent = 0.0;
}

void SoilMoistureSensor::begin() {
    pinMode(pin, INPUT);
    // Set ADC resolution to 12-bit (0-4095)
    analogReadResolution(12);
    Serial.println("Soil Moisture Sensor initialized on pin " + String(pin));
}

float SoilMoistureSensor::readMoisture() {
    // Read analog value
    rawValue = analogRead(pin);
    
    // Convert to percentage (inverted: lower value = more moisture)
    moisturePercent = map(rawValue, dryValue, wetValue, 0, 100);
    
    // Constrain to valid range
    moisturePercent = constrain(moisturePercent, 0, 100);
    
    return moisturePercent;
}

int SoilMoistureSensor::getRawValue() {
    return rawValue;
}

float SoilMoistureSensor::getMoisturePercent() {
    return moisturePercent;
}

void SoilMoistureSensor::calibrateDry(int value) {
    dryValue = value;
    Serial.println("Dry value calibrated to: " + String(dryValue));
}

void SoilMoistureSensor::calibrateWet(int value) {
    wetValue = value;
    Serial.println("Wet value calibrated to: " + String(wetValue));
}

String SoilMoistureSensor::getMoistureStatus() {
    if (moisturePercent < 20) {
        return "Dry";
    } else if (moisturePercent < 40) {
        return "Low";
    } else if (moisturePercent < 60) {
        return "Moderate";
    } else if (moisturePercent < 80) {
        return "High";
    } else {
        return "Wet";
    }
}
