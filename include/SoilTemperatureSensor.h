/**
 * @file SoilTemperatureSensor.h
 * @brief Driver for DS18B20 Digital Temperature Sensor
 * 
 * This driver reads temperature from DS18B20 sensor using OneWire protocol
 * Temperature range: -55°C to +125°C with ±0.5°C accuracy
 */

#ifndef SOIL_TEMPERATURE_SENSOR_H
#define SOIL_TEMPERATURE_SENSOR_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class SoilTemperatureSensor {
private:
    uint8_t pin;                    // Digital pin connected to DS18B20
    OneWire* oneWire;               // OneWire instance
    DallasTemperature* sensors;     // DallasTemperature instance
    float temperatureC;             // Last temperature reading in Celsius
    float temperatureF;             // Last temperature reading in Fahrenheit
    bool sensorFound;               // Flag to indicate if sensor is detected

public:
    /**
     * @brief Constructor for SoilTemperatureSensor
     * @param dataPin The digital pin connected to DS18B20 data line
     */
    SoilTemperatureSensor(uint8_t dataPin);

    /**
     * @brief Initialize the sensor
     */
    void begin();

    /**
     * @brief Read temperature from the sensor
     * @return Temperature in Celsius
     */
    float readTemperature();

    /**
     * @brief Get the last temperature reading in Celsius
     * @return Temperature in Celsius
     */
    float getTemperatureC();

    /**
     * @brief Get the last temperature reading in Fahrenheit
     * @return Temperature in Fahrenheit
     */
    float getTemperatureF();

    /**
     * @brief Check if sensor is connected
     * @return true if sensor is found, false otherwise
     */
    bool isConnected();

    /**
     * @brief Get temperature status as text
     * @return Status string based on soil temperature
     */
    String getTemperatureStatus();

    /**
     * @brief Get number of DS18B20 sensors on the bus
     * @return Number of sensors detected
     */
    int getDeviceCount();
};

#endif // SOIL_TEMPERATURE_SENSOR_H
