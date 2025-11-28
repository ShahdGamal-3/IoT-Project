/*
 * WeightSensor.cpp
 * Implementation of HX711 Load Cell Weight Sensor driver
 */

#include "WeightSensor.h"

// Constructor
WeightSensor::WeightSensor(uint8_t dataPin, uint8_t clockPin, float calibrationFactor, float maxCapacity_kg) {
    this->dataPin = dataPin;
    this->clockPin = clockPin;
    this->calibrationFactor = calibrationFactor;
    this->maxCapacity_kg = maxCapacity_kg;
    this->weight_kg = 0.0;
}

// Initialize the sensor
void WeightSensor::begin() {
    scale.begin(dataPin, clockPin);
    
    Serial.println("[Weight] HX711 Weight Sensor initialized");
    Serial.println("[Weight] Calibrating... Please ensure scale is empty");
    
    if (scale.wait_ready_timeout(1000)) {
        scale.set_scale(calibrationFactor);
        scale.tare(); // Reset scale to 0
        Serial.println("[Weight] Calibration complete!");
    } else {
        Serial.println("[Weight] WARNING: HX711 not ready!");
    }
}

// Tare/zero the scale
void WeightSensor::tare() {
    if (scale.wait_ready_timeout(1000)) {
        scale.tare();
        Serial.println("[Weight] Scale tared (zeroed)");
    }
}

// Read weight
float WeightSensor::readWeight() {
    if (scale.wait_ready_timeout(200)) {
        weight_kg = scale.get_units(5); // Average of 5 readings
        
        // Ensure weight is not negative
        if (weight_kg < 0) {
            weight_kg = 0;
        }
        
        return weight_kg;
    } else {
        Serial.println("[Weight] Sensor not ready");
        return -1.0;
    }
}

// Get weight in kg
float WeightSensor::getWeight_kg() {
    return weight_kg;
}

// Get weight in pounds
float WeightSensor::getWeight_lbs() {
    return weight_kg * 2.20462;
}

// Get weight status
String WeightSensor::getWeightStatus() {
    float percent = (weight_kg / maxCapacity_kg) * 100.0;
    
    if (weight_kg < 0.1) {
        return "Empty";
    } else if (percent < 25) {
        return "Low";
    } else if (percent < 50) {
        return "Quarter Full";
    } else if (percent < 75) {
        return "Half Full";
    } else if (percent < 90) {
        return "Nearly Full";
    } else if (percent < 100) {
        return "Full";
    } else {
        return "OVERLOADED!";
    }
}

// Set calibration factor
void WeightSensor::setCalibrationFactor(float factor) {
    calibrationFactor = factor;
    scale.set_scale(calibrationFactor);
}

// Check if overloaded
bool WeightSensor::isOverloaded() {
    return weight_kg > maxCapacity_kg;
}
