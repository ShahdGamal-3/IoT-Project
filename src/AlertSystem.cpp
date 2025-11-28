/*
 * AlertSystem.cpp
 * Implementation of Buzzer Alert System driver
 */

#include "AlertSystem.h"

// Constructor
AlertSystem::AlertSystem(uint8_t buzzerPin, unsigned long alertInterval) {
    this->buzzerPin = buzzerPin;
    this->alertInterval = alertInterval;
    this->muted = false;
    this->currentAlert = ALERT_NONE;
    this->lastAlertTime = 0;
}

// Initialize the alert system
void AlertSystem::begin() {
    pinMode(buzzerPin, OUTPUT);
    
    // Configure LEDC for buzzer (PWM)
    ledcSetup(0, 2000, 8);  // Channel 0, 2000 Hz, 8-bit resolution
    ledcAttachPin(buzzerPin, 0);
    ledcWrite(0, 0);  // Start with buzzer off
    
    Serial.println("[Alert] Buzzer Alert System initialized");
}

// Play short beep
void AlertSystem::beepShort() {
    if (!muted) {
        ledcWriteTone(0, 2000);  // 2000 Hz
        ledcWrite(0, 128);  // 50% duty cycle
        delay(100);
        ledcWrite(0, 0);  // Off
    }
}

// Play long beep
void AlertSystem::beepLong() {
    if (!muted) {
        ledcWriteTone(0, 2000);  // 2000 Hz
        ledcWrite(0, 128);  // 50% duty cycle
        delay(500);
        ledcWrite(0, 0);  // Off
    }
}

// Play beep pattern
void AlertSystem::beepPattern(int times, int duration) {
    if (!muted) {
        for (int i = 0; i < times; i++) {
            ledcWriteTone(0, 2000);  // 2000 Hz
            ledcWrite(0, 128);  // 50% duty cycle
            delay(duration);
            ledcWrite(0, 0);  // Off
            delay(duration);
        }
    }
}

// Trigger alert
void AlertSystem::triggerAlert(AlertType type) {
    if (type == ALERT_NONE) return;
    
    currentAlert = type;
    unsigned long currentTime = millis();
    
    // Only trigger if enough time has passed since last alert
    if (currentTime - lastAlertTime >= alertInterval) {
        lastAlertTime = currentTime;
        
        switch (type) {
            case ALERT_LOW_WATER:
                beepPattern(2, 200);
                Serial.println("[Alert] LOW WATER WARNING!");
                break;
            case ALERT_HIGH_TEMP:
                beepPattern(3, 150);
                Serial.println("[Alert] HIGH TEMPERATURE WARNING!");
                break;
            case ALERT_LOW_TEMP:
                beepPattern(3, 150);
                Serial.println("[Alert] LOW TEMPERATURE WARNING!");
                break;
            case ALERT_LOW_SOIL_MOISTURE:
                beepPattern(2, 250);
                Serial.println("[Alert] LOW SOIL MOISTURE WARNING!");
                break;
            case ALERT_GAS_DETECTED:
                beepPattern(5, 100);
                Serial.println("[Alert] DANGEROUS GAS DETECTED!");
                break;
            case ALERT_MOTION_DETECTED:
                beepShort();
                Serial.println("[Alert] Motion detected");
                break;
            case ALERT_OVERWEIGHT:
                beepPattern(4, 200);
                Serial.println("[Alert] OVERWEIGHT WARNING!");
                break;
            default:
                break;
        }
    }
}

// Update alert (call in loop)
void AlertSystem::update() {
    // Can add continuous alert logic here if needed
}

// Mute alerts
void AlertSystem::mute() {
    muted = true;
    Serial.println("[Alert] Alerts muted");
}

// Unmute alerts
void AlertSystem::unmute() {
    muted = false;
    Serial.println("[Alert] Alerts unmuted");
}

// Check if muted
bool AlertSystem::isMuted() {
    return muted;
}

// Get current alert type
AlertType AlertSystem::getCurrentAlert() {
    return currentAlert;
}

// Clear all alerts
void AlertSystem::clearAlerts() {
    currentAlert = ALERT_NONE;
    digitalWrite(buzzerPin, LOW);
}
