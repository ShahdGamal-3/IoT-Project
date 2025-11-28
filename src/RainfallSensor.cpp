/*
 * RainfallSensor.cpp
 * Implementation of Analog Rainfall Sensor Driver (Potentiometer simulation)
 */

#include "RainfallSensor.h"

// Constructor
RainfallSensor::RainfallSensor(uint8_t analogPin) {
    this->analogPin = analogPin;
    this->rainfall_mm = 0.0;
    this->rainRate = 0.0;
    this->lastReading = 0;
}

// Initialize sensor
void RainfallSensor::begin() {
    pinMode(analogPin, INPUT);
    Serial.println("Rainfall Sensor (Analog) initialized on pin " + String(analogPin));
} 

// Update rainfall reading
void RainfallSensor::update() {
    // Read analog value (0-4095 for ESP32)
    int rawValue = analogRead(analogPin);
    
    // Map to rainfall in mm (0-100mm range)
    rainfall_mm = map(rawValue, 0, 4095, 0, 10000) / 100.0; // Convert to float with 2 decimals
    
    // Calculate rain rate (simplified - assume reading per minute)
    unsigned long currentTime = millis();
    if (lastReading > 0) {
        float timeDiff = (currentTime - lastReading) / 60000.0; // Convert to minutes
        if (timeDiff > 0) {
            float prevRainfall = rainfall_mm;
            rainRate = (rainfall_mm - prevRainfall) / timeDiff; // mm per minute
        }
    }
    lastReading = currentTime;
}

// Get total rainfall in mm
float RainfallSensor::getRainfall_mm() {
    return rainfall_mm;
}

// Get total rainfall in inches
float RainfallSensor::getRainfall_inches() {
    return rainfall_mm * 0.0393701; // Convert mm to inches
}

// Get rain rate
float RainfallSensor::getRainRate() {
    return rainRate;
}

// Get rain status
String RainfallSensor::getRainStatus() {
    if (rainfall_mm < 1.0) {
        return "No Rain";
    } else if (rainfall_mm < 10.0) {
        return "Light Rain";
    } else if (rainfall_mm < 30.0) {
        return "Moderate Rain";
    } else if (rainfall_mm < 50.0) {
        return "Heavy Rain";
    } else {
        return "Very Heavy";
    }
}

// Get rain intensity description
String RainfallSensor::getRainIntensity() {
    if (rainRate < 0.5) {
        return "Drizzle";
    } else if (rainRate < 2.0) {
        return "Light";
    } else if (rainRate < 10.0) {
        return "Moderate";
    } else if (rainRate < 50.0) {
        return "Heavy";
    } else {
        return "Violent";
    }
}
