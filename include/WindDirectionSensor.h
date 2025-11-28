/*
 * WindDirectionSensor.h
 * Driver for Wind Vane direction sensor
 * 
 * Features:
 * - Analog voltage reading for direction detection
 * - 8 or 16-direction wind vane support
 * - Cardinal direction output (N, NE, E, SE, S, SW, W, NW)
 * - Degree output (0-360°)
 * - Potentiometer simulation for Wokwi
 */

#ifndef WINDDIRECTIONSENSOR_H
#define WINDDIRECTIONSENSOR_H

#include <Arduino.h>

class WindDirectionSensor {
private:
    uint8_t analogPin;
    int rawValue;
    float voltage;
    int direction;  // 0-360 degrees
    String cardinalDirection;
    int samples;    // number of samples for averaging
    
    // Convert ADC value to direction
    int voltageToDirection(int adcValue);
    String directionToCardinal(int degrees);

public:
    // Constructor
    WindDirectionSensor(uint8_t analogPin, int samples = 10);
    
    // Initialize the sensor
    void begin();
    
    // Read wind direction
    int readDirection();
    
    // Get direction in degrees (0-360)
    int getDirectionDegrees();
    
    // Get cardinal direction (N, NE, E, SE, S, SW, W, NW, etc.)
    String getCardinalDirection();
    
    // Get raw ADC value
    int getRawValue();
    
    // Get voltage
    float getVoltage();
};

#endif
