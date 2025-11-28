/**
 * @file SoilMoistureSensor.h
 * @brief Driver for Capacitive Soil Moisture Sensor
 * 
 * This driver reads analog values from a capacitive soil moisture sensor
 * and converts them to moisture percentage (0-100%)
 */

#ifndef SOIL_MOISTURE_SENSOR_H
#define SOIL_MOISTURE_SENSOR_H

#include <Arduino.h>

class SoilMoistureSensor {
private:
    uint8_t pin;              // Analog pin connected to sensor
    int dryValue;             // Sensor value in completely dry soil (default: 4095 for 12-bit ADC)
    int wetValue;             // Sensor value in completely wet soil (default: 1500)
    int rawValue;             // Last raw reading from sensor
    float moisturePercent;    // Last calculated moisture percentage

public:
    /**
     * @brief Constructor for SoilMoistureSensor
     * @param analogPin The analog pin connected to the sensor (e.g., 34, 35, 36, 39)
     * @param dryVal Calibration value for dry soil (default: 4095)
     * @param wetVal Calibration value for wet soil (default: 1500)
     */
    SoilMoistureSensor(uint8_t analogPin, int dryVal = 4095, int wetVal = 1500);

    /**
     * @brief Initialize the sensor
     */
    void begin();

    /**
     * @brief Read the current moisture level
     * @return Moisture percentage (0-100%)
     */
    float readMoisture();

    /**
     * @brief Get the last raw sensor value
     * @return Raw ADC value (0-4095)
     */
    int getRawValue();

    /**
     * @brief Get the last calculated moisture percentage
     * @return Moisture percentage (0-100%)
     */
    float getMoisturePercent();

    /**
     * @brief Calibrate the sensor for dry conditions
     * @param value The raw ADC value in dry soil
     */
    void calibrateDry(int value);

    /**
     * @brief Calibrate the sensor for wet conditions
     * @param value The raw ADC value in wet soil
     */
    void calibrateWet(int value);

    /**
     * @brief Get moisture level status as text
     * @return Status string (Dry, Low, Moderate, High, Wet)
     */
    String getMoistureStatus();
};

#endif // SOIL_MOISTURE_SENSOR_H
