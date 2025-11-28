/**
 * @file SoilTemperatureSensor.cpp
 * @brief Implementation of DS18B20 Digital Temperature Sensor driver
 */

#include "SoilTemperatureSensor.h"

SoilTemperatureSensor::SoilTemperatureSensor(uint8_t dataPin) {
    pin = dataPin;
    temperatureC = 0.0;
    temperatureF = 0.0;
    sensorFound = false;
    oneWire = nullptr;
    sensors = nullptr;
}

void SoilTemperatureSensor::begin() {
    // Create OneWire instance
    oneWire = new OneWire(pin);
    sensors = new DallasTemperature(oneWire);
    
    // Start the library
    sensors->begin();
    
    // Check if sensor is connected
    sensorFound = (sensors->getDeviceCount() > 0);
    
    if (sensorFound) {
        Serial.println("DS18B20 Soil Temperature Sensor initialized on pin " + String(pin));
        Serial.print("Sensors found: ");
        Serial.println(sensors->getDeviceCount());
    } else {
        Serial.println("WARNING: No DS18B20 sensor detected on pin " + String(pin));
    }
}

float SoilTemperatureSensor::readTemperature() {
    if (!sensorFound) {
        Serial.println("Error: No DS18B20 sensor found!");
        return -127.0; // Error value
    }
    
    // Request temperature reading
    sensors->requestTemperatures();
    
    // Read temperature in Celsius
    temperatureC = sensors->getTempCByIndex(0);
    
    // Convert to Fahrenheit
    temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
    
    // Check for reading error
    if (temperatureC == DEVICE_DISCONNECTED_C) {
        Serial.println("Error: Failed to read temperature!");
        return -127.0;
    }
    
    return temperatureC;
}

float SoilTemperatureSensor::getTemperatureC() {
    return temperatureC;
}

float SoilTemperatureSensor::getTemperatureF() {
    return temperatureF;
}

bool SoilTemperatureSensor::isConnected() {
    return sensorFound;
}

String SoilTemperatureSensor::getTemperatureStatus() {
    if (temperatureC < 5) {
        return "Too Cold";
    } else if (temperatureC < 15) {
        return "Cold";
    } else if (temperatureC < 25) {
        return "Optimal";
    } else if (temperatureC < 35) {
        return "Warm";
    } else {
        return "Too Hot";
    }
}

int SoilTemperatureSensor::getDeviceCount() {
    if (sensors) {
        return sensors->getDeviceCount();
    }
    return 0;
}
