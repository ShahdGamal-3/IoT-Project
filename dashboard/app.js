// WebSocket Configuration
const ESP32_IP = 'localhost'; // For Serial Bridge, use 'localhost'
const WS_PORT = 8765; // Serial Bridge WebSocket port
const SIMULATION_MODE = true; // Set to true for local testing without ESP32
let ws;
let reconnectInterval;
let reconnectAttempts = 0;
const MAX_RECONNECT_ATTEMPTS = 3;

// Data Storage
let sensorData = {
    soilMoisture: 45.0,
    soilTemp: 22.5,
    soilPH: 6.8,
    leafTemp: 28.0,
    leafWetness: 15.0,
    airTemp: 25.0,
    humidity: 60.0,
    light: 850,
    rainfall: 12.5,
    windSpeed: 5.2,
    windDirection: 315,
    gas: 125,
    co2: 450,
    co: 15,
    waterLevel: 75.0,
    motion: false,
    weight: 45.2
};

// Initialize on page load
document.addEventListener('DOMContentLoaded', function() {
    initializeSliders();
    
    // Check if simulation mode or real hardware mode
    if (SIMULATION_MODE) {
        console.log('Running in SIMULATION MODE - WebSocket disabled');
        updateConnectionStatus(false, true); // false = not connected, true = simulation mode
        
        // Show info banner
        const banner = document.getElementById('simInfoBanner');
        if (banner) {
            banner.style.display = 'block';
        }
        
        addAlert('Running in Simulation Mode - Sliders control display locally', 'info');
    } else {
        initializeWebSocket();
    }
    
    updateAllValues();
    updateLastUpdate();
    setInterval(updateLastUpdate, 1000);
});

// WebSocket Functions
function initializeWebSocket() {
    // Don't attempt connection if in simulation mode
    if (SIMULATION_MODE) {
        console.log('Simulation mode enabled - skipping WebSocket connection');
        return;
    }
    
    // Limit reconnection attempts
    if (reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) {
        console.log('Max reconnection attempts reached. Switching to simulation mode.');
        addAlert('Unable to connect to ESP32. Running in simulation mode.', 'warning');
        return;
    }
    
    console.log(`Attempting to connect to ws://${ESP32_IP}:${WS_PORT} (Attempt ${reconnectAttempts + 1}/${MAX_RECONNECT_ATTEMPTS})`);
    reconnectAttempts++;
    
    try {
        ws = new WebSocket(`ws://${ESP32_IP}:${WS_PORT}`);
        
        ws.onopen = function() {
            console.log('WebSocket connected!');
            updateConnectionStatus(true, false);
            reconnectAttempts = 0; // Reset counter on successful connection
            clearInterval(reconnectInterval);
            addAlert('WebSocket connection established', 'success');
        };
        
        ws.onmessage = function(event) {
            try {
                const data = JSON.parse(event.data);
                console.log('Received data:', data);
                updateSensorData(data);
            } catch (error) {
                console.error('Error parsing WebSocket message:', error);
            }
        };
        
        ws.onerror = function(error) {
            console.error('WebSocket error:', error);
            updateConnectionStatus(false, false);
            // Removed repetitive error alerts
        };
        
        ws.onclose = function() {
            console.log('WebSocket disconnected');
            updateConnectionStatus(false, false);
            
            // Only show alert and reconnect if not at max attempts
            if (reconnectAttempts < MAX_RECONNECT_ATTEMPTS && !SIMULATION_MODE) {
                addAlert('Connection lost. Attempting to reconnect...', 'warning');
                
                // Attempt to reconnect after delay
                setTimeout(function() {
                    initializeWebSocket();
                }, 5000);
            }
        };
    } catch (error) {
        console.error('Failed to create WebSocket:', error);
        updateConnectionStatus(false, false);
    }
}

function sendCommand(command, value) {
    // In simulation mode, commands only update local display
    if (SIMULATION_MODE) {
        console.log('Simulation mode: Command would be sent:', command, value);
        return; // Don't try to send via WebSocket
    }
    
    if (ws && ws.readyState === WebSocket.OPEN) {
        const message = JSON.stringify({
            command: command,
            value: value,
            timestamp: Date.now()
        });
        ws.send(message);
        console.log('Sent command:', message);
    } else {
        console.error('WebSocket not connected - command not sent');
        // Don't show alert in simulation mode
        if (!SIMULATION_MODE) {
            addAlert('Cannot send command: Not connected', 'danger');
        }
    }
}

// Update Connection Status
function updateConnectionStatus(connected, simulationMode = false) {
    const statusElement = document.getElementById('connectionStatus');
    const statusText = statusElement.querySelector('span');
    
    if (simulationMode) {
        statusElement.classList.remove('connected');
        statusElement.classList.add('simulation');
        statusText.textContent = 'Simulation Mode';
        statusElement.style.background = 'linear-gradient(135deg, #f39c12 0%, #e67e22 100%)';
    } else if (connected) {
        statusElement.classList.add('connected');
        statusElement.classList.remove('simulation');
        statusText.textContent = 'Connected';
        statusElement.style.background = '';
    } else {
        statusElement.classList.remove('connected', 'simulation');
        statusText.textContent = 'Disconnected';
        statusElement.style.background = '';
    }
}

// Update Sensor Data
function updateSensorData(data) {
    // Update stored values
    Object.assign(sensorData, data);
    
    // Update UI
    updateAllValues();
    
    // Check for alerts
    checkAlertConditions(data);
}

// Update All UI Values
function updateAllValues() {
    // Soil Section
    updateValue('soilMoisture', sensorData.soilMoisture, '%', 
        sensorData.soilMoisture < 20 ? 'danger' : sensorData.soilMoisture < 40 ? 'warning' : 'normal',
        sensorData.soilMoisture < 20 ? 'Dry' : sensorData.soilMoisture < 40 ? 'Low' : 'Good');
    
    updateValue('soilTemp', sensorData.soilTemp, '°C',
        sensorData.soilTemp < 10 || sensorData.soilTemp > 35 ? 'warning' : 'normal',
        sensorData.soilTemp < 10 ? 'Cold' : sensorData.soilTemp > 35 ? 'Hot' : 'Optimal');
    
    updateValue('soilPH', sensorData.soilPH, ' pH',
        sensorData.soilPH < 5.5 || sensorData.soilPH > 8.0 ? 'warning' : 'normal',
        sensorData.soilPH < 5.5 ? 'Acidic' : sensorData.soilPH > 8.0 ? 'Alkaline' : 'Optimal');
    
    // Plant Health
    updateValue('leafTemp', sensorData.leafTemp, '°C',
        sensorData.leafTemp < 15 || sensorData.leafTemp > 40 ? 'warning' : 'normal',
        sensorData.leafTemp < 15 ? 'Cold' : sensorData.leafTemp > 40 ? 'Hot' : 'Optimal');
    
    updateValue('leafWetness', sensorData.leafWetness, '%',
        sensorData.leafWetness > 70 ? 'warning' : 'normal',
        sensorData.leafWetness < 30 ? 'Dry' : sensorData.leafWetness < 70 ? 'Moist' : 'Wet');
    
    // Climate
    updateValue('airTemp', sensorData.airTemp, '°C',
        sensorData.airTemp < 10 || sensorData.airTemp > 35 ? 'warning' : 'normal',
        sensorData.airTemp < 10 ? 'Cold' : sensorData.airTemp > 35 ? 'Hot' : 'Comfortable');
    
    updateValue('humidity', sensorData.humidity, '%',
        sensorData.humidity < 30 || sensorData.humidity > 80 ? 'warning' : 'normal',
        sensorData.humidity < 30 ? 'Dry' : sensorData.humidity > 80 ? 'Humid' : 'Normal');
    
    updateValue('light', sensorData.light, ' lux',
        'normal',
        sensorData.light < 200 ? 'Dark' : sensorData.light < 500 ? 'Dim' : sensorData.light < 1000 ? 'Bright' : 'Very Bright');
    
    updateValue('rainfall', sensorData.rainfall, ' mm',
        'normal',
        sensorData.rainfall < 5 ? 'No Rain' : sensorData.rainfall < 20 ? 'Light Rain' : sensorData.rainfall < 50 ? 'Moderate Rain' : 'Heavy Rain');
    
    // Wind
    updateValue('windSpeed', sensorData.windSpeed, ' m/s',
        sensorData.windSpeed > 15 ? 'warning' : 'normal',
        getWindDescription(sensorData.windSpeed));
    
    const windDir = getWindDirection(sensorData.windDirection);
    document.getElementById('windDirection').textContent = windDir.short;
    document.getElementById('windDirStatus').textContent = windDir.long;
    
    // Air Quality
    updateValue('gas', sensorData.gas, ' ppm',
        sensorData.gas > 300 ? 'danger' : sensorData.gas > 150 ? 'warning' : 'normal',
        sensorData.gas > 300 ? 'Danger!' : sensorData.gas > 150 ? 'Warning' : 'Safe');
    
    updateValue('co2', sensorData.co2, ' ppm',
        sensorData.co2 > 2000 ? 'danger' : sensorData.co2 > 1000 ? 'warning' : 'normal',
        sensorData.co2 > 2000 ? 'High' : sensorData.co2 > 1000 ? 'Elevated' : 'Good');
    
    updateValue('co', sensorData.co, ' ppm',
        sensorData.co > 50 ? 'danger' : sensorData.co > 25 ? 'warning' : 'normal',
        sensorData.co > 50 ? 'Danger!' : sensorData.co > 25 ? 'Warning' : 'Safe');
    
    // Resources
    updateValue('waterLevel', sensorData.waterLevel, '%',
        sensorData.waterLevel < 20 ? 'danger' : sensorData.waterLevel < 40 ? 'warning' : 'normal',
        sensorData.waterLevel < 20 ? 'Very Low' : sensorData.waterLevel < 40 ? 'Low' : 'Good');
    
    document.getElementById('motion').textContent = sensorData.motion ? 'Motion Detected!' : 'No Motion';
    document.getElementById('motionStatus').textContent = sensorData.motion ? 'Alert' : 'Clear';
    document.getElementById('motionStatus').className = sensorData.motion ? 'status-badge danger' : 'status-badge';
    
    updateValue('weight', sensorData.weight, ' kg',
        sensorData.weight > 150 ? 'danger' : sensorData.weight > 100 ? 'warning' : 'normal',
        sensorData.weight > 150 ? 'Overload' : sensorData.weight > 100 ? 'High' : 'Normal');
    
    // Update Live Display Panel
    updateLiveDisplayPanel();
}

// Update Live Display Panel with current sensor values
function updateLiveDisplayPanel() {
    // Soil values
    updateDisplayValue('display-soilMoisture', sensorData.soilMoisture, '%', 
        sensorData.soilMoisture < 20 ? 'danger' : sensorData.soilMoisture < 40 ? 'warning' : 'normal');
    updateDisplayValue('display-soilTemp', sensorData.soilTemp, '°C',
        sensorData.soilTemp < 10 || sensorData.soilTemp > 35 ? 'warning' : 'normal');
    updateDisplayValue('display-soilPH', sensorData.soilPH, ' pH',
        sensorData.soilPH < 5.5 || sensorData.soilPH > 8.0 ? 'warning' : 'normal');
    
    // Plant values
    updateDisplayValue('display-leafTemp', sensorData.leafTemp, '°C',
        sensorData.leafTemp < 15 || sensorData.leafTemp > 40 ? 'warning' : 'normal');
    updateDisplayValue('display-leafWetness', sensorData.leafWetness, '%',
        sensorData.leafWetness > 70 ? 'warning' : 'normal');
    
    // Climate values
    updateDisplayValue('display-airTemp', sensorData.airTemp, '°C',
        sensorData.airTemp < 10 || sensorData.airTemp > 35 ? 'warning' : 'normal');
    updateDisplayValue('display-humidity', sensorData.humidity, '%',
        sensorData.humidity < 30 || sensorData.humidity > 80 ? 'warning' : 'normal');
    updateDisplayValue('display-light', sensorData.light, ' lux', 'normal');
    updateDisplayValue('display-rainfall', sensorData.rainfall, ' mm', 'normal');
    
    // Wind values
    updateDisplayValue('display-windSpeed', sensorData.windSpeed, ' m/s',
        sensorData.windSpeed > 15 ? 'warning' : 'normal');
    const windDir = getWindDirection(sensorData.windDirection);
    const windDirElement = document.getElementById('display-windDirection');
    if (windDirElement) {
        windDirElement.textContent = windDir.short;
        windDirElement.className = 'value normal';
    }
    
    // Air Quality values
    updateDisplayValue('display-gas', sensorData.gas, ' ppm',
        sensorData.gas > 300 ? 'danger' : sensorData.gas > 150 ? 'warning' : 'normal');
    updateDisplayValue('display-co2', sensorData.co2, ' ppm',
        sensorData.co2 > 2000 ? 'danger' : sensorData.co2 > 1000 ? 'warning' : 'normal');
    updateDisplayValue('display-co', sensorData.co, ' ppm',
        sensorData.co > 50 ? 'danger' : sensorData.co > 25 ? 'warning' : 'normal');
    
    // Resources values
    updateDisplayValue('display-waterLevel', sensorData.waterLevel, '%',
        sensorData.waterLevel < 20 ? 'danger' : sensorData.waterLevel < 40 ? 'warning' : 'normal');
    updateDisplayValue('display-weight', sensorData.weight, ' kg',
        sensorData.weight > 150 ? 'danger' : sensorData.weight > 100 ? 'warning' : 'normal');
    
    // Motion status
    const motionElement = document.getElementById('display-motion');
    if (motionElement) {
        motionElement.textContent = sensorData.motion ? 'Detected' : 'None';
        motionElement.className = sensorData.motion ? 'value warning' : 'value normal';
    }
}

// Helper function to update display panel values with color coding
function updateDisplayValue(id, value, unit, statusClass) {
    const element = document.getElementById(id);
    if (element) {
        const formattedValue = typeof value === 'number' ? value.toFixed(1) : value;
        element.textContent = formattedValue + unit;
        element.className = 'value ' + statusClass;
    }
}

// Helper function to update value and status
function updateValue(id, value, unit, statusClass, statusText) {
    const valueElement = document.getElementById(id);
    const statusElement = document.getElementById(id + 'Status');
    
    if (valueElement) {
        valueElement.textContent = typeof value === 'number' ? value.toFixed(1) : value;
    }
    
    if (statusElement) {
        statusElement.textContent = statusText;
        statusElement.className = 'status-badge';
        if (statusClass === 'danger') statusElement.classList.add('danger');
        else if (statusClass === 'warning') statusElement.classList.add('warning');
    }
}

// Wind Description
function getWindDescription(speed) {
    if (speed < 0.3) return 'Calm';
    if (speed < 1.6) return 'Light Air';
    if (speed < 3.4) return 'Light Breeze';
    if (speed < 5.5) return 'Gentle Breeze';
    if (speed < 8.0) return 'Moderate Breeze';
    if (speed < 10.8) return 'Fresh Breeze';
    if (speed < 13.9) return 'Strong Breeze';
    if (speed < 17.2) return 'Near Gale';
    return 'Gale';
}

// Wind Direction
function getWindDirection(degrees) {
    const directions = [
        { short: 'N', long: 'North', min: 337.5, max: 22.5 },
        { short: 'NE', long: 'Northeast', min: 22.5, max: 67.5 },
        { short: 'E', long: 'East', min: 67.5, max: 112.5 },
        { short: 'SE', long: 'Southeast', min: 112.5, max: 157.5 },
        { short: 'S', long: 'South', min: 157.5, max: 202.5 },
        { short: 'SW', long: 'Southwest', min: 202.5, max: 247.5 },
        { short: 'W', long: 'West', min: 247.5, max: 292.5 },
        { short: 'NW', long: 'Northwest', min: 292.5, max: 337.5 }
    ];
    
    for (let dir of directions) {
        if (degrees >= dir.min && degrees < dir.max) {
            return dir;
        }
        if (dir.short === 'N' && (degrees >= dir.min || degrees < dir.max)) {
            return dir;
        }
    }
    return { short: 'N', long: 'North' };
}

// Initialize Sliders
function initializeSliders() {
    // Soil sliders
    setupSlider('soilMoisture', 'soilMoistureSlider', '%');
    setupSlider('soilTemp', 'soilTempSlider', '°C');
    setupSlider('soilPH', 'soilPHSlider', ' pH');
    
    // Plant sliders
    setupSlider('leafTemp', 'leafTempSlider', '°C');
    setupSlider('leafWetness', 'leafWetnessSlider', '%');
    
    // Climate sliders
    setupSlider('airTemp', 'airTempSlider', '°C');
    setupSlider('humidity', 'humiditySlider', '%');
    setupSlider('light', 'lightSlider', ' lux');
    setupSlider('rainfall', 'rainfallSlider', ' mm');
    
    // Wind sliders
    setupSlider('windSpeed', 'windSpeedSlider', ' m/s');
    setupSlider('windDir', 'windDirSlider', '°');
    
    // Air quality sliders
    setupSlider('gas', 'gasSlider', ' ppm');
    setupSlider('co2', 'co2Slider', ' ppm');
    setupSlider('co', 'coSlider', ' ppm');
    
    // Resources sliders
    setupSlider('water', 'waterSlider', '%');
    setupSlider('weight', 'weightSlider', ' kg');
    
    // Motion button
    const motionBtn = document.getElementById('motionBtn');
    motionBtn.addEventListener('click', function() {
        sensorData.motion = !sensorData.motion;
        updateAllValues();
        sendCommand('motion', sensorData.motion ? 1 : 0);
        this.classList.toggle('active');
        setTimeout(() => {
            sensorData.motion = false;
            updateAllValues();
            this.classList.remove('active');
        }, 2000);
    });
}

// Setup individual slider
function setupSlider(dataKey, sliderId, unit) {
    const slider = document.getElementById(sliderId);
    if (!slider) return;
    
    const valueDisplay = slider.nextElementSibling;
    
    slider.addEventListener('input', function() {
        const value = parseFloat(this.value);
        valueDisplay.textContent = value.toFixed(dataKey === 'light' ? 0 : 1) + unit;
        
        // Update data based on slider ID
        if (sliderId === 'soilMoistureSlider') sensorData.soilMoisture = value;
        else if (sliderId === 'soilTempSlider') sensorData.soilTemp = value;
        else if (sliderId === 'soilPHSlider') sensorData.soilPH = value;
        else if (sliderId === 'leafTempSlider') sensorData.leafTemp = value;
        else if (sliderId === 'leafWetnessSlider') sensorData.leafWetness = value;
        else if (sliderId === 'airTempSlider') sensorData.airTemp = value;
        else if (sliderId === 'humiditySlider') sensorData.humidity = value;
        else if (sliderId === 'lightSlider') sensorData.light = value;
        else if (sliderId === 'rainfallSlider') sensorData.rainfall = value;
        else if (sliderId === 'windSpeedSlider') sensorData.windSpeed = value;
        else if (sliderId === 'windDirSlider') sensorData.windDirection = value;
        else if (sliderId === 'gasSlider') sensorData.gas = value;
        else if (sliderId === 'co2Slider') sensorData.co2 = value;
        else if (sliderId === 'coSlider') sensorData.co = value;
        else if (sliderId === 'waterSlider') sensorData.waterLevel = value;
        else if (sliderId === 'weightSlider') sensorData.weight = value;
        
        updateAllValues();
    });
    
    slider.addEventListener('change', function() {
        // Send command to ESP32 when slider is released
        sendCommand(dataKey, parseFloat(this.value));
    });
}

// Check Alert Conditions
function checkAlertConditions(data) {
    if (data.soilMoisture < 20) {
        addAlert('Low soil moisture detected! Irrigation needed.', 'danger');
    }
    if (data.waterLevel < 20) {
        addAlert('Water tank level critically low!', 'danger');
    }
    if (data.gas > 300) {
        addAlert('Dangerous gas levels detected!', 'danger');
    }
    if (data.co2 > 2000) {
        addAlert('High CO₂ concentration detected!', 'danger');
    }
    if (data.co > 50) {
        addAlert('Dangerous CO levels detected!', 'danger');
    }
    if (data.motion) {
        addAlert('Motion detected in monitored area', 'warning');
    }
}

// Add Alert to UI
function addAlert(message, type = 'info') {
    const alertsContainer = document.getElementById('alertsContainer');
    const alertItem = document.createElement('div');
    alertItem.className = `alert-item ${type}`;
    
    const icon = type === 'danger' ? 'fa-exclamation-triangle' : 
                 type === 'warning' ? 'fa-exclamation-circle' :
                 type === 'success' ? 'fa-check-circle' : 'fa-info-circle';
    
    alertItem.innerHTML = `
        <i class="fas ${icon}"></i>
        <span>${message}</span>
        <span class="alert-time">${new Date().toLocaleTimeString()}</span>
    `;
    
    alertsContainer.insertBefore(alertItem, alertsContainer.firstChild);
    
    // Keep only last 10 alerts
    while (alertsContainer.children.length > 10) {
        alertsContainer.removeChild(alertsContainer.lastChild);
    }
}

// Update Last Update Time
function updateLastUpdate() {
    const now = new Date();
    const timeString = now.toLocaleTimeString();
    document.getElementById('lastUpdate').textContent = timeString;
}

// ============================================
// VALUE DISPLAY TABS FUNCTIONALITY
// ============================================

// Initialize tab switching
document.addEventListener('DOMContentLoaded', function() {
    // Tab switching logic
    const tabButtons = document.querySelectorAll('.tab-button');
    const tabPanels = document.querySelectorAll('.tab-panel');
    
    tabButtons.forEach(button => {
        button.addEventListener('click', function() {
            const targetTab = this.getAttribute('data-tab');
            
            // Remove active class from all buttons and panels
            tabButtons.forEach(btn => btn.classList.remove('active'));
            tabPanels.forEach(panel => panel.classList.remove('active'));
            
            // Add active class to clicked button and corresponding panel
            this.classList.add('active');
            document.getElementById(targetTab).classList.add('active');
        });
    });
    
    // Initialize value displays
    updateAllValueDisplays();
});

// Update all value display elements
function updateAllValueDisplays() {
    // Individual tab displays
    updateValueDisplay('soilMoisture', sensorData.soilMoisture, '%');
    updateValueDisplay('soilTemp', sensorData.soilTemp, '°C');
    updateValueDisplay('soilPH', sensorData.soilPH, ' pH');
    updateValueDisplay('leafTemp', sensorData.leafTemp, '°C');
    updateValueDisplay('leafWetness', sensorData.leafWetness, '%');
    updateValueDisplay('airTemp', sensorData.airTemp, '°C');
    updateValueDisplay('humidity', sensorData.humidity, '%');
    updateValueDisplay('light', sensorData.light, ' lux');
    updateValueDisplay('rainfall', sensorData.rainfall, ' mm');
    updateValueDisplay('windSpeed', sensorData.windSpeed, ' km/h');
    
    // Wind direction with compass
    const windDir = sensorData.windDirection;
    const compass = getCompassDirection(windDir);
    updateValueDisplay('windDirection', `${windDir}° (${compass})`, '');
    
    updateValueDisplay('gas', sensorData.gas, ' PPM');
    updateValueDisplay('co2', sensorData.co2, ' PPM');
    updateValueDisplay('co', sensorData.co, ' PPM');
    updateValueDisplay('waterLevel', sensorData.waterLevel, '%');
    
    // Motion sensor
    const motionText = sensorData.motion ? 'Motion Detected' : 'No Motion';
    updateValueDisplay('motion', motionText, '');
    
    updateValueDisplay('weight', sensorData.weight, ' kg');
    
    // Update all-sensors compact view
    updateCompactView();
}

// Update individual value display
function updateValueDisplay(sensor, value, unit) {
    // Update the display card value
    const displayElement = document.getElementById(`display-${sensor}`);
    if (displayElement) {
        if (typeof value === 'number') {
            displayElement.textContent = value.toFixed(1);
        } else {
            displayElement.textContent = value;
        }
    }
    
    // Update status badge
    const statusElement = document.getElementById(`display-${sensor}-status`);
    if (statusElement) {
        const status = getStatusText(sensor, value);
        statusElement.textContent = status.text;
        statusElement.style.background = status.bg;
        statusElement.style.color = status.color;
    }
}

// Get status text and color for sensor
function getStatusText(sensor, value) {
    const statuses = {
        soilMoisture: [
            { max: 20, text: 'Dry', bg: 'rgba(231, 76, 60, 0.2)', color: '#e74c3c' },
            { max: 40, text: 'Low', bg: 'rgba(243, 156, 18, 0.2)', color: '#f39c12' },
            { max: 70, text: 'Normal', bg: 'rgba(46, 204, 113, 0.2)', color: '#2ecc71' },
            { max: 100, text: 'Wet', bg: 'rgba(52, 152, 219, 0.2)', color: '#3498db' }
        ],
        soilTemp: [
            { max: 15, text: 'Cold', bg: 'rgba(52, 152, 219, 0.2)', color: '#3498db' },
            { max: 30, text: 'Optimal', bg: 'rgba(46, 204, 113, 0.2)', color: '#2ecc71' },
            { max: 100, text: 'Hot', bg: 'rgba(231, 76, 60, 0.2)', color: '#e74c3c' }
        ],
        soilPH: [
            { max: 5.5, text: 'Acidic', bg: 'rgba(243, 156, 18, 0.2)', color: '#f39c12' },
            { max: 7.5, text: 'Optimal', bg: 'rgba(46, 204, 113, 0.2)', color: '#2ecc71' },
            { max: 14, text: 'Alkaline', bg: 'rgba(243, 156, 18, 0.2)', color: '#f39c12' }
        ],
        leafTemp: [
            { max: 20, text: 'Cool', bg: 'rgba(52, 152, 219, 0.2)', color: '#3498db' },
            { max: 35, text: 'Healthy', bg: 'rgba(46, 204, 113, 0.2)', color: '#2ecc71' },
            { max: 100, text: 'Hot', bg: 'rgba(231, 76, 60, 0.2)', color: '#e74c3c' }
        ],
        leafWetness: [
            { max: 30, text: 'Dry', bg: 'rgba(46, 204, 113, 0.2)', color: '#2ecc71' },
            { max: 70, text: 'Moist', bg: 'rgba(243, 156, 18, 0.2)', color: '#f39c12' },
            { max: 100, text: 'Wet', bg: 'rgba(231, 76, 60, 0.2)', color: '#e74c3c' }
        ],
        humidity: [
            { max: 30, text: 'Low', bg: 'rgba(243, 156, 18, 0.2)', color: '#f39c12' },
            { max: 70, text: 'Normal', bg: 'rgba(46, 204, 113, 0.2)', color: '#2ecc71' },
            { max: 100, text: 'High', bg: 'rgba(52, 152, 219, 0.2)', color: '#3498db' }
        ],
        waterLevel: [
            { max: 20, text: 'Low', bg: 'rgba(231, 76, 60, 0.2)', color: '#e74c3c' },
            { max: 50, text: 'Medium', bg: 'rgba(243, 156, 18, 0.2)', color: '#f39c12' },
            { max: 100, text: 'Good', bg: 'rgba(46, 204, 113, 0.2)', color: '#2ecc71' }
        ]
    };
    
    if (statuses[sensor]) {
        for (let status of statuses[sensor]) {
            if (value <= status.max) return status;
        }
    }
    
    return { text: 'Normal', bg: 'rgba(46, 204, 113, 0.2)', color: '#2ecc71' };
}

// Get compass direction from degrees
function getCompassDirection(degrees) {
    const directions = ['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW'];
    const index = Math.round(degrees / 45) % 8;
    return directions[index];
}

// Update compact all-sensors view
function updateCompactView() {
    document.getElementById('all-soilMoisture').textContent = sensorData.soilMoisture.toFixed(1) + '%';
    document.getElementById('all-soilTemp').textContent = sensorData.soilTemp.toFixed(1) + '°C';
    document.getElementById('all-soilPH').textContent = sensorData.soilPH.toFixed(1) + ' pH';
    document.getElementById('all-leafTemp').textContent = sensorData.leafTemp.toFixed(1) + '°C';
    document.getElementById('all-leafWetness').textContent = sensorData.leafWetness.toFixed(1) + '%';
    document.getElementById('all-airTemp').textContent = sensorData.airTemp.toFixed(1) + '°C';
    document.getElementById('all-humidity').textContent = sensorData.humidity.toFixed(1) + '%';
    document.getElementById('all-light').textContent = sensorData.light + ' lux';
    document.getElementById('all-rainfall').textContent = sensorData.rainfall.toFixed(1) + ' mm';
    document.getElementById('all-windSpeed').textContent = sensorData.windSpeed.toFixed(1) + ' km/h';
    
    const windDir = sensorData.windDirection;
    const compass = getCompassDirection(windDir);
    document.getElementById('all-windDirection').textContent = `${windDir}° (${compass})`;
    
    document.getElementById('all-gas').textContent = sensorData.gas + ' PPM';
    document.getElementById('all-co2').textContent = sensorData.co2 + ' PPM';
    document.getElementById('all-co').textContent = sensorData.co + ' PPM';
    document.getElementById('all-waterLevel').textContent = sensorData.waterLevel.toFixed(1) + '%';
    
    const motionText = sensorData.motion ? 'Motion Detected' : 'No Motion';
    document.getElementById('all-motion').textContent = motionText;
    
    document.getElementById('all-weight').textContent = sensorData.weight.toFixed(1) + ' kg';
}

// Override the updateAllValues function to also update displays
const originalUpdateAllValues = updateAllValues;
updateAllValues = function() {
    originalUpdateAllValues();
    updateAllValueDisplays();
};
