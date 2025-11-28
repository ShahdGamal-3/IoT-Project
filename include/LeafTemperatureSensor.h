/**
 * @file LeafTemperatureSensor.h
 * @brief Driver for Leaf Temperature Sensor (Simulated with Potentiometer in Wokwi)
 * 
 * This driver simulates infrared leaf temperature measurement using a potentiometer
 * In real hardware, would use MLX90614 infrared sensor
 * Simulated temperature range: 10°C to 50°C
 */

#ifndef LEAF_TEMPERATURE_SENSOR_H
#define LEAF_TEMPERATURE_SENSOR_H

#include <Arduino.h>

class LeafTemperatureSensor {
private:
    uint8_t analogPin;           // Analog pin for potentiometer
    float objectTempC;           // Last object temperature in Celsius
    float objectTempF;           // Last object temperature in Fahrenheit
    float ambientTempC;          // Last ambient temperature in Celsius
    float ambientTempF;          // Last ambient temperature in Fahrenheit

public:
    /**
     * @brief Constructor for LeafTemperatureSensor
     * @param pin Analog pin connected to potentiometer
     */
    LeafTemperatureSensor(uint8_t pin);

    /**
     * @brief Initialize the sensor
     * @return true always (simulation mode)
     */
    bool begin();

    /**
     * @brief Read temperature from the sensor
     * @return Object (leaf) temperature in Celsius
     */
    float readTemperature();

    /**
     * @brief Get the last object temperature in Celsius
     * @return Object temperature in Celsius
     */
    float getObjectTempC();

    /**
     * @brief Get the last object temperature in Fahrenheit
     * @return Object temperature in Fahrenheit
     */
    float getObjectTempF();

    /**
     * @brief Get the last ambient temperature in Celsius
     * @return Ambient temperature in Celsius
     */
    float getAmbientTempC();

    /**
     * @brief Get the last ambient temperature in Fahrenheit
     * @return Ambient temperature in Fahrenheit
     */
    float getAmbientTempF();

    /**
     * @brief Check if sensor is connected
     * @return true if sensor is found, false otherwise
     */
    bool isConnected();

    /**
     * @brief Get leaf temperature status as text
     * @return Status string based on leaf temperature
     */
    String getTemperatureStatus();
};

#endif // LEAF_TEMPERATURE_SENSOR_H
