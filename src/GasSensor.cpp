/*
 * GasSensor.cpp
 * Implementation of MQ2 Gas Sensor driver
 */

#include "GasSensor.h"

// Constructor
GasSensor::GasSensor(uint8_t analogPin, int samples) {
    this->analogPin = analogPin;
    this->samples = samples;
    this->rawValue = 0;
    this->gasPPM = 0.0;
}

// Initialize the sensor
void GasSensor::begin() {
    pinMode(analogPin, INPUT);
    Serial.println("[Gas] MQ2 Gas Sensor initialized");
    Serial.println("[Gas] Warming up... (Allow 20-30 seconds for calibration)");
}

// Read gas concentration
float GasSensor::readGas() {
    // Read analog value and average multiple samples
    long sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(analogPin);
        delay(2);
    }
    rawValue = sum / samples;
    
    // Convert to approximate PPM (simplified calculation)
    // MQ2 has non-linear response, this is an approximation
    // Map 0-4095 ADC to 0-10000 ppm range
    gasPPM = map(rawValue, 0, 4095, 0, 10000);
    
    return gasPPM;
}

// Get gas concentration in ppm
float GasSensor::getGasPPM() {
    return gasPPM;
}

// Get raw ADC value
int GasSensor::getRawValue() {
    return rawValue;
}

// Get gas status
String GasSensor::getGasStatus() {
    if (gasPPM < 300) {
        return "Clean Air";
    } else if (gasPPM < 1000) {
        return "Slight Gas";
    } else if (gasPPM < 3000) {
        return "Moderate Gas";
    } else if (gasPPM < 5000) {
        return "High Gas";
    } else {
        return "DANGEROUS!";
    }
}

// Check if gas level is dangerous
bool GasSensor::isDangerous() {
    return gasPPM > 3000;
}
