/*
 * MotionSensor.cpp
 * Implementation of PIR Motion Sensor driver
 */

#include "MotionSensor.h"

// Constructor
MotionSensor::MotionSensor(uint8_t pin, unsigned long debounceDelay) {
    this->pin = pin;
    this->debounceDelay = debounceDelay;
    this->motionDetected = false;
    this->lastMotionTime = 0;
    this->motionCount = 0;
}

// Initialize the sensor
void MotionSensor::begin() {
    pinMode(pin, INPUT);
    Serial.println("[Motion] PIR Motion Sensor initialized");
    Serial.println("[Motion] Allow 30-60 seconds for PIR calibration");
    delay(2000); // Brief calibration delay
}

// Read motion status
bool MotionSensor::readMotion() {
    bool currentState = digitalRead(pin);
    unsigned long currentTime = millis();
    
    // Detect new motion with debouncing
    if (currentState == HIGH && !motionDetected) {
        if (currentTime - lastMotionTime > debounceDelay) {
            motionDetected = true;
            lastMotionTime = currentTime;
            motionCount++;
            return true;
        }
    } else if (currentState == LOW) {
        motionDetected = false;
    }
    
    return motionDetected;
}

// Check if motion is currently detected
bool MotionSensor::isMotionDetected() {
    return digitalRead(pin) == HIGH;
}

// Get time since last motion
unsigned long MotionSensor::getTimeSinceMotion() {
    return millis() - lastMotionTime;
}

// Get total motion count
unsigned long MotionSensor::getMotionCount() {
    return motionCount;
}

// Reset motion count
void MotionSensor::resetCount() {
    motionCount = 0;
}

// Get motion status
String MotionSensor::getMotionStatus() {
    if (isMotionDetected()) {
        return "Motion Detected";
    } else {
        unsigned long timeSince = getTimeSinceMotion();
        if (timeSince < 10000) {
            return "Recent Motion";
        } else {
            return "No Motion";
        }
    }
}
