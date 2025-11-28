/**
 * @file LeafWetnessSensor.h
 * @brief Driver for Resistive Leaf Wetness Sensor
 * 
 * This driver reads analog resistance from a leaf wetness sensor
 * Higher resistance = dry, Lower resistance = wet
 * Used to detect dew, rain, or irrigation water on plant leaves
 */

#ifndef LEAF_WETNESS_SENSOR_H
#define LEAF_WETNESS_SENSOR_H

#include <Arduino.h>

class LeafWetnessSensor {
private:
    uint8_t pin;              // Analog pin connected to sensor
    int rawValue;             // Last raw ADC reading
    float wetnessPercent;     // Wetness percentage (0-100%)
    int dryValue;             // Calibration value for dry leaf (default: 4095)
    int wetValue;             // Calibration value for wet leaf (default: 1000)

public:
    /**
     * @brief Constructor for LeafWetnessSensor
     * @param analogPin The analog pin connected to the sensor
     * @param dryVal Calibration value for dry leaf (default: 4095)
     * @param wetVal Calibration value for wet leaf (default: 1000)
     */
    LeafWetnessSensor(uint8_t analogPin, int dryVal = 4095, int wetVal = 1000);

    /**
     * @brief Initialize the sensor
     */
    void begin();

    /**
     * @brief Read the current wetness level
     * @return Wetness percentage (0-100%)
     */
    float readWetness();

    /**
     * @brief Get the last wetness percentage
     * @return Wetness percentage (0-100%)
     */
    float getWetnessPercent();

    /**
     * @brief Get the last raw ADC value
     * @return Raw ADC value (0-4095)
     */
    int getRawValue();

    /**
     * @brief Get wetness status as text
     * @return Status string (Dry, Slightly Wet, Wet, Very Wet)
     */
    String getWetnessStatus();

    /**
     * @brief Calibrate the sensor for dry conditions
     * @param value The raw ADC value when leaf is dry
     */
    void calibrateDry(int value);

    /**
     * @brief Calibrate the sensor for wet conditions
     * @param value The raw ADC value when leaf is wet
     */
    void calibrateWet(int value);

    /**
     * @brief Check if leaf is wet (wetness > 30%)
     * @return true if wet, false if dry
     */
    bool isWet();
};

#endif // LEAF_WETNESS_SENSOR_H
