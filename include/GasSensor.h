/*
 * GasSensor.h
 * Driver for MQ2 Gas Sensor
 * 
 * Features:
 * - Analog gas concentration reading
 * - Detection of smoke, LPG, methane, propane
 * - Gas level in ppm estimation
 * - Safety status and warnings
 */

#ifndef GASSENSOR_H
#define GASSENSOR_H

#include <Arduino.h>

class GasSensor {
private:
    uint8_t analogPin;
    int rawValue;
    float gasPPM;
    int samples;

public:
    // Constructor
    GasSensor(uint8_t analogPin, int samples = 10);
    
    // Initialize the sensor
    void begin();
    
    // Read gas concentration
    float readGas();
    
    // Get gas concentration in ppm
    float getGasPPM();
    
    // Get raw ADC value
    int getRawValue();
    
    // Get gas status
    String getGasStatus();
    
    // Check if gas level is dangerous
    bool isDangerous();
};

#endif
