/*
 * DHTSensor.cpp
 * Implementation of DHT22 sensor driver
 */

#include "DHTSensor.h"

// Constructor
DHTSensor::DHTSensor(uint8_t pin) {
    this->pin = pin;
    this->temperature = 0.0;
    this->humidity = 0.0;
    this->lastReadSuccess = false;
    this->dht = new DHT(pin, DHT_TYPE);
}

// Destructor
DHTSensor::~DHTSensor() {
    delete dht;
}

// Initialize the sensor
void DHTSensor::begin() {
    dht->begin();
    delay(2000); // DHT22 needs time to stabilize
}

// Read temperature and humidity from sensor
bool DHTSensor::readSensor() {
    float temp = dht->readTemperature();
    float hum = dht->readHumidity();
    
    // Check if reading failed
    if (isnan(temp) || isnan(hum)) {
        lastReadSuccess = false;
        return false;
    }
    
    temperature = temp;
    humidity = hum;
    lastReadSuccess = true;
    return true;
}

// Get temperature in Celsius
float DHTSensor::getTemperature() {
    return temperature;
}

// Get humidity percentage
float DHTSensor::getHumidity() {
    return humidity;
}

// Get temperature status
String DHTSensor::getTemperatureStatus() {
    if (temperature < 0) {
        return "Freezing";
    } else if (temperature < 10) {
        return "Cold";
    } else if (temperature < 20) {
        return "Cool";
    } else if (temperature < 25) {
        return "Comfortable";
    } else if (temperature < 30) {
        return "Warm";
    } else if (temperature < 35) {
        return "Hot";
    } else {
        return "Very Hot";
    }
}

// Get humidity status
String DHTSensor::getHumidityStatus() {
    if (humidity < 20) {
        return "Very Dry";
    } else if (humidity < 30) {
        return "Dry";
    } else if (humidity < 50) {
        return "Comfortable";
    } else if (humidity < 70) {
        return "Humid";
    } else if (humidity < 85) {
        return "Very Humid";
    } else {
        return "Extremely Humid";
    }
}

// Check if last reading was successful
bool DHTSensor::isReadingValid() {
    return lastReadSuccess;
}

// Get heat index (feels like temperature)
float DHTSensor::getHeatIndex() {
    return dht->computeHeatIndex(temperature, humidity, false);
}
