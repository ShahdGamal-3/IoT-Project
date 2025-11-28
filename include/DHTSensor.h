/*
 * DHTSensor.h
 * Driver for DHT22 sensor to measure air temperature and humidity
 * 
 * Features:
 * - Air temperature measurement (-40°C to 80°C)
 * - Relative humidity measurement (0-100%)
 * - Temperature and humidity status categorization
 * - Error detection and handling
 */

#ifndef DHTSENSOR_H
#define DHTSENSOR_H

#include <Arduino.h>
#include <DHT.h>

#define DHT_TYPE DHT22

class DHTSensor {
private:
    uint8_t pin;
    DHT* dht;
    float temperature;
    float humidity;
    bool lastReadSuccess;

public:
    // Constructor
    DHTSensor(uint8_t pin);
    
    // Destructor
    ~DHTSensor();
    
    // Initialize the sensor
    void begin();
    
    // Read temperature and humidity from sensor
    bool readSensor();
    
    // Get temperature in Celsius
    float getTemperature();
    
    // Get humidity percentage
    float getHumidity();
    
    // Get temperature status
    String getTemperatureStatus();
    
    // Get humidity status
    String getHumidityStatus();
    
    // Check if last reading was successful
    bool isReadingValid();
    
    // Get heat index (feels like temperature)
    float getHeatIndex();
};

#endif
