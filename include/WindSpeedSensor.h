/*
 * WindSpeedSensor.h
 * Driver for Anemometer wind speed sensor using pulse counting
 * 
 * Features:
 * - Pulse counting via interrupt
 * - Wind speed calculation in m/s, km/h, and mph
 * - Calibration factor support
 * - Wind condition categorization
 * - PWM simulation mode for Wokwi testing
 */

#ifndef WINDSPEEDSENSOR_H
#define WINDSPEEDSENSOR_H

#include <Arduino.h>

class WindSpeedSensor {
private:
    uint8_t pin;
    volatile unsigned long pulseCount;
    unsigned long lastMeasurementTime;
    float windSpeed_ms;  // meters per second
    float calibrationFactor; // pulses per second per m/s
    
    // Simulation mode
    bool simulationMode;
    uint8_t simulatorPin;
    uint8_t potPin;
    hw_timer_t* timer;
    float simulatedWindSpeed;
    volatile bool toggleState;
    
    static WindSpeedSensor* instance;
    static void IRAM_ATTR handleInterrupt();
    static void IRAM_ATTR onSimulatorTimer();

public:
    // Constructor
    WindSpeedSensor(uint8_t pin, float calibrationFactor = 2.4);
    
    // Initialize the sensor
    void begin();
    
    // Enable simulation mode with PWM generator
    void enableSimulation(uint8_t simPin, uint8_t potentiometerPin);
    
    // Update simulated wind speed from potentiometer
    void updateSimulation();
    
    // Calculate wind speed (call periodically)
    float calculateWindSpeed();
    
    // Get wind speed in different units
    float getWindSpeed_ms();    // meters per second
    float getWindSpeed_kmh();   // kilometers per hour
    float getWindSpeed_mph();   // miles per hour
    
    // Get wind condition status
    String getWindStatus();
    
    // Get pulse count
    unsigned long getPulseCount();
    
    // Reset pulse count
    void resetPulseCount();
    
    // Set calibration factor
    void setCalibrationFactor(float factor);
};

#endif
