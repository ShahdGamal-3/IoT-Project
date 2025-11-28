/*
 * WindSpeedSensor.cpp
 * Implementation of Anemometer wind speed sensor driver
 */

#include "WindSpeedSensor.h"

// Static member initialization
WindSpeedSensor* WindSpeedSensor::instance = nullptr;

// Constructor
WindSpeedSensor::WindSpeedSensor(uint8_t pin, float calibrationFactor) {
    this->pin = pin;
    this->calibrationFactor = calibrationFactor;
    this->pulseCount = 0;
    this->windSpeed_ms = 0.0;
    this->lastMeasurementTime = 0;
    this->simulationMode = false;
    this->simulatorPin = 0;
    this->potPin = 0;
    this->timer = nullptr;
    this->simulatedWindSpeed = 0.0;
    this->toggleState = false;
    instance = this;
}

// Static interrupt handler
void IRAM_ATTR WindSpeedSensor::handleInterrupt() {
    if (instance != nullptr) {
        instance->pulseCount++;
    }
}

// Initialize the sensor
void WindSpeedSensor::begin() {
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt, FALLING);
    lastMeasurementTime = millis();
    Serial.println("[WindSpeed] Sensor initialized (Pulse Counting Mode)");
}

// Enable simulation mode with PWM generator
void WindSpeedSensor::enableSimulation(uint8_t simPin, uint8_t potentiometerPin) {
    simulationMode = true;
    simulatorPin = simPin;
    potPin = potentiometerPin;
    
    pinMode(simulatorPin, OUTPUT);
    digitalWrite(simulatorPin, LOW);
    pinMode(potPin, INPUT);
    
    // Create hardware timer (Timer 0, prescaler 80 = 1MHz, count up)
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onSimulatorTimer, true);
    timerAlarmWrite(timer, 10000, true); // Start with 100Hz (0.01s = 10000us)
    timerAlarmEnable(timer);
    
    Serial.println("[WindSpeed] Simulation mode enabled");
    Serial.println("[WindSpeed] PWM generator active on pin " + String(simulatorPin));
    Serial.println("[WindSpeed] Control via potentiometer on pin " + String(potPin));
}

// Timer interrupt for PWM generation
void IRAM_ATTR WindSpeedSensor::onSimulatorTimer() {
    if (instance && instance->simulationMode) {
        instance->toggleState = !instance->toggleState;
        digitalWrite(instance->simulatorPin, instance->toggleState ? HIGH : LOW);
    }
}

// Update simulated wind speed from potentiometer
void WindSpeedSensor::updateSimulation() {
    if (!simulationMode) return;
    
    // Read potentiometer value (0-4095)
    int potValue = analogRead(potPin);
    
    // Map to wind speed (0-50 m/s)
    float targetSpeed = (potValue / 4095.0) * 50.0;
    
    // Only update if change is significant (> 1.0 m/s to reduce noise)
    if (abs(targetSpeed - simulatedWindSpeed) > 1.0) {
        simulatedWindSpeed = targetSpeed;
        
        // Calculate required pulse frequency
        // pulses_per_second = wind_speed * calibrationFactor
        float pulsesPerSecond = simulatedWindSpeed * calibrationFactor;
        
        if (pulsesPerSecond > 0.1) {
            // Timer period in microseconds for toggle (half period)
            uint64_t timerPeriod = (1000000.0 / pulsesPerSecond) / 2.0;
            timerAlarmWrite(timer, timerPeriod, true);
        } else {
            // Very slow or stopped - set to very long period
            timerAlarmWrite(timer, 1000000, true);
        }
        
        // Remove continuous printing - only print on significant changes
        // Serial.printf("[WindSpeed] Simulated speed: %.1f m/s (%.1f Hz)\n", 
        //               simulatedWindSpeed, pulsesPerSecond);
    }
}

// Calculate wind speed
float WindSpeedSensor::calculateWindSpeed() {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - lastMeasurementTime;
    
    if (elapsedTime >= 1000) { // Calculate every second
        // Calculate rotations per second
        float rotationsPerSecond = (float)pulseCount / ((float)elapsedTime / 1000.0);
        
        // Convert to wind speed (m/s)
        // Formula: wind_speed = rotations_per_second / calibration_factor
        windSpeed_ms = rotationsPerSecond / calibrationFactor;
        
        // Reset for next measurement
        pulseCount = 0;
        lastMeasurementTime = currentTime;
    }
    
    return windSpeed_ms;
}

// Get wind speed in meters per second
float WindSpeedSensor::getWindSpeed_ms() {
    return windSpeed_ms;
}

// Get wind speed in kilometers per hour
float WindSpeedSensor::getWindSpeed_kmh() {
    return windSpeed_ms * 3.6;
}

// Get wind speed in miles per hour
float WindSpeedSensor::getWindSpeed_mph() {
    return windSpeed_ms * 2.23694;
}

// Get wind condition status
String WindSpeedSensor::getWindStatus() {
    float kmh = getWindSpeed_kmh();
    
    if (kmh < 1) {
        return "Calm";
    } else if (kmh < 5) {
        return "Light Air";
    } else if (kmh < 12) {
        return "Light Breeze";
    } else if (kmh < 20) {
        return "Gentle Breeze";
    } else if (kmh < 29) {
        return "Moderate Breeze";
    } else if (kmh < 39) {
        return "Fresh Breeze";
    } else if (kmh < 50) {
        return "Strong Breeze";
    } else if (kmh < 62) {
        return "Near Gale";
    } else if (kmh < 75) {
        return "Gale";
    } else if (kmh < 89) {
        return "Strong Gale";
    } else if (kmh < 103) {
        return "Storm";
    } else {
        return "Violent Storm";
    }
}

// Get pulse count
unsigned long WindSpeedSensor::getPulseCount() {
    return pulseCount;
}

// Reset pulse count
void WindSpeedSensor::resetPulseCount() {
    pulseCount = 0;
    lastMeasurementTime = millis();
}

// Set calibration factor
void WindSpeedSensor::setCalibrationFactor(float factor) {
    calibrationFactor = factor;
}
