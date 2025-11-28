/*
 * CO2Sensor.cpp
 * Implementation of MQ135 CO2 Sensor Driver
 */

#include "CO2Sensor.h"

// Constructor
CO2Sensor::CO2Sensor(uint8_t analogPin, int samples) {
    this->analogPin = analogPin;
    this->samples = samples;
    this->rawValue = 0;
    this->co2PPM = 0.0;
}

// Initialize sensor
void CO2Sensor::begin() {
    pinMode(analogPin, INPUT);
    Serial.println("CO2 Sensor (MQ135) initialized on pin " + String(analogPin));
}

// Read CO2 concentration
float CO2Sensor::readCO2() {
    // Take multiple samples and average
    long sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(analogPin);
        delay(10);
    }
    rawValue = sum / samples;
    
    // Convert ADC value (0-4095) to CO2 ppm (400-5000 ppm)
    // Normal outdoor CO2: ~400 ppm
    // Indoor acceptable: 400-1000 ppm
    // Poor ventilation: 1000-2000 ppm
    // Unhealthy: 2000-5000 ppm
    co2PPM = map(rawValue, 0, 4095, 400, 5000);
    
    return co2PPM;
}

// Get air quality status
String CO2Sensor::getAirQuality() {
    if (co2PPM < 800) {
        return "Excellent";
    } else if (co2PPM < 1000) {
        return "Good";
    } else if (co2PPM < 1500) {
        return "Fair";
    } else if (co2PPM < 2000) {
        return "Poor";
    } else {
        return "Bad";
    }
}

// Check if dangerous
bool CO2Sensor::isDangerous() {
    return co2PPM > 2000; // Above 2000 ppm is considered unhealthy
}

// Get raw ADC value
int CO2Sensor::getRawValue() {
    return rawValue;
}
