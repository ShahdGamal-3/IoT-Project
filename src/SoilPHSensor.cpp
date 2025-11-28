/**
 * @file SoilPHSensor.cpp
 * @brief Implementation of Soil pH Electrode Sensor driver
 */

#include "SoilPHSensor.h"

SoilPHSensor::SoilPHSensor(uint8_t analogPin) {
    pin = analogPin;
    phValue = 7.0;
    rawValue = 0;
    voltage = 0.0;
    
    // Default calibration values (adjust based on actual calibration)
    acidVoltage = 2.032;      // pH 4.0
    neutralVoltage = 1.500;   // pH 7.0
    alkalineVoltage = 0.968;  // pH 10.0
}

void SoilPHSensor::begin() {
    pinMode(pin, INPUT);
    analogReadResolution(12);  // 12-bit ADC (0-4095)
    Serial.println("Soil pH Sensor initialized on pin " + String(pin));
}

float SoilPHSensor::readPH() {
    // Read analog value
    rawValue = analogRead(pin);
    
    // Convert to voltage (ESP32: 0-3.3V for 0-4095)
    voltage = (rawValue / 4095.0) * 3.3;
    
    // Convert voltage to pH using linear interpolation
    // pH electrode typically: higher voltage = lower pH (acidic)
    if (voltage >= neutralVoltage) {
        // Acidic range (pH 4.0 to 7.0)
        phValue = 7.0 - ((voltage - neutralVoltage) / (acidVoltage - neutralVoltage)) * 3.0;
    } else {
        // Alkaline range (pH 7.0 to 10.0)
        phValue = 7.0 + ((neutralVoltage - voltage) / (neutralVoltage - alkalineVoltage)) * 3.0;
    }
    
    // Constrain to valid pH range
    phValue = constrain(phValue, 0.0, 14.0);
    
    return phValue;
}

float SoilPHSensor::getPH() {
    return phValue;
}

int SoilPHSensor::getRawValue() {
    return rawValue;
}

float SoilPHSensor::getVoltage() {
    return voltage;
}

String SoilPHSensor::getPHStatus() {
    if (phValue < 4.5) {
        return "Very Acidic";
    } else if (phValue < 5.5) {
        return "Acidic";
    } else if (phValue < 6.5) {
        return "Slightly Acidic";
    } else if (phValue < 7.5) {
        return "Neutral";
    } else if (phValue < 8.5) {
        return "Slightly Alkaline";
    } else if (phValue < 9.5) {
        return "Alkaline";
    } else {
        return "Very Alkaline";
    }
}

void SoilPHSensor::calibrateAcid(float voltage) {
    acidVoltage = voltage;
    Serial.println("pH 4.0 calibrated to voltage: " + String(voltage, 3) + "V");
}

void SoilPHSensor::calibrateNeutral(float voltage) {
    neutralVoltage = voltage;
    Serial.println("pH 7.0 calibrated to voltage: " + String(voltage, 3) + "V");
}

void SoilPHSensor::calibrateAlkaline(float voltage) {
    alkalineVoltage = voltage;
    Serial.println("pH 10.0 calibrated to voltage: " + String(voltage, 3) + "V");
}
