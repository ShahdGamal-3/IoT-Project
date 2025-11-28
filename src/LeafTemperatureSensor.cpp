/**
 * @file LeafTemperatureSensor.cpp
 * @brief Implementation of Leaf Temperature Sensor (Potentiometer Simulation)
 */

#include "LeafTemperatureSensor.h"

LeafTemperatureSensor::LeafTemperatureSensor(uint8_t pin) : analogPin(pin) {
    objectTempC = 0.0;
    objectTempF = 0.0;
    ambientTempC = 0.0;
    ambientTempF = 0.0;
}

bool LeafTemperatureSensor::begin() {
    pinMode(analogPin, INPUT);
    Serial.println("Leaf Temperature Sensor initialized (Potentiometer simulation)");
    return true;
}

float LeafTemperatureSensor::readTemperature() {
    // Read potentiometer value (0-4095 on ESP32)
    int rawValue = analogRead(analogPin);
    
    // Map to temperature range: 10°C to 50°C
    objectTempC = map(rawValue, 0, 4095, 10, 50);
    objectTempF = (objectTempC * 9.0 / 5.0) + 32.0;
    
    // Simulate ambient temperature (slightly lower than object temp)
    ambientTempC = objectTempC - 2.0;
    ambientTempF = (ambientTempC * 9.0 / 5.0) + 32.0;
    
    return objectTempC;
}

float LeafTemperatureSensor::getObjectTempC() {
    return objectTempC;
}

float LeafTemperatureSensor::getObjectTempF() {
    return objectTempF;
}

float LeafTemperatureSensor::getAmbientTempC() {
    return ambientTempC;
}

float LeafTemperatureSensor::getAmbientTempF() {
    return ambientTempF;
}

bool LeafTemperatureSensor::isConnected() {
    return true;  // Always connected in simulation mode
}

String LeafTemperatureSensor::getTemperatureStatus() {
    if (objectTempC < 10) {
        return "Too Cold";
    } else if (objectTempC < 20) {
        return "Cold";
    } else if (objectTempC < 30) {
        return "Optimal";
    } else if (objectTempC < 40) {
        return "Warm";
    } else {
        return "Too Hot";
    }
}
