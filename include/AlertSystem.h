/*
 * AlertSystem.h
 * Driver for Buzzer Alert System
 * 
 * Features:
 * - Audio alerts for critical conditions
 * - Different beep patterns for different alerts
 * - Alert priority management
 * - Mute/unmute functionality
 */

#ifndef ALERTSYSTEM_H
#define ALERTSYSTEM_H

#include <Arduino.h>

enum AlertType {
    ALERT_NONE = 0,
    ALERT_LOW_WATER,
    ALERT_HIGH_TEMP,
    ALERT_LOW_TEMP,
    ALERT_LOW_SOIL_MOISTURE,
    ALERT_GAS_DETECTED,
    ALERT_MOTION_DETECTED,
    ALERT_OVERWEIGHT
};

class AlertSystem {
private:
    uint8_t buzzerPin;
    bool muted;
    AlertType currentAlert;
    unsigned long lastAlertTime;
    unsigned long alertInterval;
    
    // Play different beep patterns
    void beepShort();
    void beepLong();
    void beepPattern(int times, int duration);

public:
    // Constructor
    AlertSystem(uint8_t buzzerPin, unsigned long alertInterval = 5000);
    
    // Initialize the alert system
    void begin();
    
    // Trigger alert
    void triggerAlert(AlertType type);
    
    // Update alert (call in loop)
    void update();
    
    // Mute/unmute alerts
    void mute();
    void unmute();
    bool isMuted();
    
    // Get current alert type
    AlertType getCurrentAlert();
    
    // Clear all alerts
    void clearAlerts();
};

#endif
