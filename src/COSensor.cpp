/*
 * COSensor.cpp
 * Implementation of MQ7 CO (Carbon Monoxide) Sensor Driver
 */

#include "COSensor.h"

// Constructor
COSensor::COSensor(uint8_t analogPin, int samples) {
    this->analogPin = analogPin;
    this->samples = samples;
    this->rawValue = 0;
    this->coPPM = 0.0;
}

// Initialize sensor
void COSensor::begin() {
    pinMode(analogPin, INPUT);
    Serial.println("CO Sensor (MQ7) initialized on pin " + String(analogPin));
}

// Read CO concentration
float COSensor::readCO() {
    // Take multiple samples and average
    long sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(analogPin);
        delay(10);
    }
    rawValue = sum / samples;
    
    // Convert ADC value (0-4095) to CO ppm (0-1000 ppm)
    // Safe level: 0-9 ppm
    // Acceptable: 10-50 ppm
    // Dangerous: 50-400 ppm
    // Lethal: 400+ ppm
    coPPM = map(rawValue, 0, 4095, 0, 1000);
    
    return coPPM;
}

// Get CO status
String COSensor::getCOStatus() {
    if (coPPM < 9) {
        return "Safe";
    } else if (coPPM < 35) {
        return "Acceptable";
    } else if (coPPM < 100) {
        return "Caution";
    } else if (coPPM < 400) {
        return "Dangerous";
    } else {
        return "LETHAL";
    }
}

// Check if dangerous
bool COSensor::isDangerous() {
    return coPPM > 50; // Above 50 ppm is considered dangerous
}

// Get raw ADC value
int COSensor::getRawValue() {
    return rawValue;
}
