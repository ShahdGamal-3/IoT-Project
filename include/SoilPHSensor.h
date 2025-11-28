/**
 * @file SoilPHSensor.h
 * @brief Driver for Soil pH Electrode Sensor
 * 
 * This driver reads analog voltage from a pH electrode sensor
 * and converts it to pH value (0-14 scale)
 * Typical pH range for soil: 4.0 (very acidic) to 9.0 (very alkaline)
 */

#ifndef SOIL_PH_SENSOR_H
#define SOIL_PH_SENSOR_H

#include <Arduino.h>

class SoilPHSensor {
private:
    uint8_t pin;              // Analog pin connected to pH sensor
    float phValue;            // Last pH reading
    int rawValue;             // Last raw ADC reading
    float voltage;            // Last voltage reading
    
    // Calibration values (can be adjusted based on calibration)
    float acidVoltage;        // Voltage at pH 4.0 (default: 2.032V)
    float neutralVoltage;     // Voltage at pH 7.0 (default: 1.500V)
    float alkalineVoltage;    // Voltage at pH 10.0 (default: 0.968V)

public:
    /**
     * @brief Constructor for SoilPHSensor
     * @param analogPin The analog pin connected to the pH sensor
     */
    SoilPHSensor(uint8_t analogPin);

    /**
     * @brief Initialize the sensor
     */
    void begin();

    /**
     * @brief Read the current pH value
     * @return pH value (0-14 scale)
     */
    float readPH();

    /**
     * @brief Get the last pH value
     * @return pH value (0-14 scale)
     */
    float getPH();

    /**
     * @brief Get the last raw ADC value
     * @return Raw ADC value (0-4095)
     */
    int getRawValue();

    /**
     * @brief Get the last voltage reading
     * @return Voltage in volts
     */
    float getVoltage();

    /**
     * @brief Get pH status as text
     * @return Status string describing soil pH condition
     */
    String getPHStatus();

    /**
     * @brief Calibrate the sensor at pH 4.0 (acidic)
     * @param voltage The measured voltage at pH 4.0
     */
    void calibrateAcid(float voltage);

    /**
     * @brief Calibrate the sensor at pH 7.0 (neutral)
     * @param voltage The measured voltage at pH 7.0
     */
    void calibrateNeutral(float voltage);

    /**
     * @brief Calibrate the sensor at pH 10.0 (alkaline)
     * @param voltage The measured voltage at pH 10.0
     */
    void calibrateAlkaline(float voltage);
};

#endif // SOIL_PH_SENSOR_H
