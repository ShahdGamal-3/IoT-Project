/*
 * MotionSensor.h
 * Driver for PIR Motion Sensor
 * 
 * Features:
 * - Motion detection
 * - Activity monitoring
 * - Intruder/animal detection
 * - Event counting
 */

#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include <Arduino.h>

class MotionSensor {
private:
    uint8_t pin;
    bool motionDetected;
    unsigned long lastMotionTime;
    unsigned long motionCount;
    unsigned long debounceDelay;

public:
    // Constructor
    MotionSensor(uint8_t pin, unsigned long debounceDelay = 2000);
    
    // Initialize the sensor
    void begin();
    
    // Read motion status
    bool readMotion();
    
    // Check if motion is currently detected
    bool isMotionDetected();
    
    // Get time since last motion (ms)
    unsigned long getTimeSinceMotion();
    
    // Get total motion count
    unsigned long getMotionCount();
    
    // Reset motion count
    void resetCount();
    
    // Get motion status
    String getMotionStatus();
};

#endif
