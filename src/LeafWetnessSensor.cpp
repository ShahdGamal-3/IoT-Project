/**
 * @file LeafWetnessSensor.cpp
 * @brief Implementation of Resistive Leaf Wetness Sensor driver
 */

#include "LeafWetnessSensor.h"

LeafWetnessSensor::LeafWetnessSensor(uint8_t analogPin, int dryVal, int wetVal) {
    pin = analogPin;
    rawValue = 0;
    wetnessPercent = 0.0;
    dryValue = dryVal;
    wetValue = wetVal;
}

void LeafWetnessSensor::begin() {
    pinMode(pin, INPUT);
    analogReadResolution(12);  // 12-bit ADC (0-4095)
    Serial.println("Leaf Wetness Sensor initialized on pin " + String(pin));
}

float LeafWetnessSensor::readWetness() {
    // Read analog value
    rawValue = analogRead(pin);
    
    // Convert to percentage (inverted: lower resistance = more wetness)
    wetnessPercent = map(rawValue, dryValue, wetValue, 0, 100);
    
    // Constrain to valid range
    wetnessPercent = constrain(wetnessPercent, 0, 100);
    
    return wetnessPercent;
}

float LeafWetnessSensor::getWetnessPercent() {
    return wetnessPercent;
}

int LeafWetnessSensor::getRawValue() {
    return rawValue;
}

String LeafWetnessSensor::getWetnessStatus() {
    if (wetnessPercent < 20) {
        return "Dry";
    } else if (wetnessPercent < 50) {
        return "Slightly Wet";
    } else if (wetnessPercent < 80) {
        return "Wet";
    } else {
        return "Very Wet";
    }
}

void LeafWetnessSensor::calibrateDry(int value) {
    dryValue = value;
    Serial.println("Dry value calibrated to: " + String(dryValue));
}

void LeafWetnessSensor::calibrateWet(int value) {
    wetValue = value;
    Serial.println("Wet value calibrated to: " + String(wetValue));
}

bool LeafWetnessSensor::isWet() {
    return (wetnessPercent > 30);
}
