/*
 * WindDirectionSensor.cpp
 * Implementation of Wind Vane direction sensor driver
 */

#include "WindDirectionSensor.h"

// Constructor
WindDirectionSensor::WindDirectionSensor(uint8_t analogPin, int samples) {
    this->analogPin = analogPin;
    this->samples = samples;
    this->rawValue = 0;
    this->voltage = 0.0;
    this->direction = 0;
    this->cardinalDirection = "N";
}

// Initialize the sensor
void WindDirectionSensor::begin() {
    pinMode(analogPin, INPUT);
    Serial.println("[WindDirection] Wind Direction Sensor initialized");
    Serial.println("[WindDirection] Using potentiometer for simulation (0-360°)");
}

// Convert ADC value to direction (0-360 degrees)
int WindDirectionSensor::voltageToDirection(int adcValue) {
    // Map ADC value (0-4095) to degrees (0-359)
    // For potentiometer simulation in Wokwi
    int degrees = map(adcValue, 0, 4095, 0, 359);
    return degrees;
}

// Convert degrees to cardinal direction
String WindDirectionSensor::directionToCardinal(int degrees) {
    // Normalize to 0-359
    degrees = degrees % 360;
    if (degrees < 0) degrees += 360;
    
    // 16-point compass rose
    if (degrees >= 349 || degrees < 11) return "N";
    else if (degrees >= 11 && degrees < 34) return "NNE";
    else if (degrees >= 34 && degrees < 56) return "NE";
    else if (degrees >= 56 && degrees < 79) return "ENE";
    else if (degrees >= 79 && degrees < 101) return "E";
    else if (degrees >= 101 && degrees < 124) return "ESE";
    else if (degrees >= 124 && degrees < 146) return "SE";
    else if (degrees >= 146 && degrees < 169) return "SSE";
    else if (degrees >= 169 && degrees < 191) return "S";
    else if (degrees >= 191 && degrees < 214) return "SSW";
    else if (degrees >= 214 && degrees < 236) return "SW";
    else if (degrees >= 236 && degrees < 259) return "WSW";
    else if (degrees >= 259 && degrees < 281) return "W";
    else if (degrees >= 281 && degrees < 304) return "WNW";
    else if (degrees >= 304 && degrees < 326) return "NW";
    else return "NNW";  // 326-348
}

// Read wind direction
int WindDirectionSensor::readDirection() {
    // Read analog value and average multiple samples
    long sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(analogPin);
        delay(2);
    }
    rawValue = sum / samples;
    
    // Convert to voltage (0-3.3V)
    voltage = (rawValue / 4095.0) * 3.3;
    
    // Convert to direction
    direction = voltageToDirection(rawValue);
    
    // Get cardinal direction
    cardinalDirection = directionToCardinal(direction);
    
    return direction;
}

// Get direction in degrees
int WindDirectionSensor::getDirectionDegrees() {
    return direction;
}

// Get cardinal direction
String WindDirectionSensor::getCardinalDirection() {
    return cardinalDirection;
}

// Get raw ADC value
int WindDirectionSensor::getRawValue() {
    return rawValue;
}

// Get voltage
float WindDirectionSensor::getVoltage() {
    return voltage;
}
