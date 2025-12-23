# ESP32 Smart Farm - Quick Start Guide

## 📋 Prerequisites

1. **Install PlatformIO:**
   ```powershell
   pip install platformio
   ```

2. **Install required tools:**
   - Python 3.7+
   - Git
   - ESP32 USB drivers

## 🚀 Quick Start

### 1. Build All Nodes
```powershell
.\build_all.ps1
```

This will compile firmware for all three nodes:
- Gateway Node
- Soil Node  
- Weather Node

### 2. Upload Firmware

**Upload all nodes** (update COM ports in script first):
```powershell
.\upload_all.ps1
```

**Upload individual node:**
```powershell
cd gateway_node
pio run --target upload --upload-port COM3
```

### 3. Monitor Serial Output

**Monitor specific node:**
```powershell
.\monitor.ps1 -Node gateway
.\monitor.ps1 -Node soil
.\monitor.ps1 -Node weather
```

**Custom COM port:**
```powershell
.\monitor.ps1 -Node gateway -Port COM5
```

## 🔧 Individual Node Commands

### Gateway Node
```powershell
cd gateway_node
pio run                        # Build
pio run --target upload        # Upload
pio device monitor             # Monitor
```

### Soil Node
```powershell
cd soil_node
pio run                        # Build
pio run --target upload        # Upload
pio device monitor             # Monitor
```

### Weather Node
```powershell
cd weather_node
pio run                        # Build
pio run --target upload        # Upload
pio device monitor             # Monitor
```

## 🧹 Clean Build Artifacts
```powershell
.\clean.ps1
```

## 📁 Project Structure

```
esp32_nodes/
├── gateway_node/          # Central gateway with Firebase
├── soil_node/            # Soil & plant monitoring
├── weather_node/         # Weather & air quality
├── build_all.ps1         # Build all nodes
├── upload_all.ps1        # Upload all nodes
├── monitor.ps1           # Serial monitor
├── clean.ps1             # Clean builds
└── FIRMWARE_STRUCTURE.md # Detailed documentation
```

## 🔍 Troubleshooting

### Build Errors
1. Check `platformio.ini` for correct library dependencies
2. Clean and rebuild: `.\clean.ps1` then `.\build_all.ps1`
3. Update platform: `pio platform update espressif32`

### Upload Errors
1. Verify correct COM port
2. Check ESP32 USB connection
3. Press BOOT button during upload if needed
4. Update USB drivers

### Serial Monitor Issues
1. Close other serial connections to the port
2. Check baud rate (115200)
3. Verify COM port assignment

## 📖 Configuration

### WiFi Settings (Gateway Node)
Edit `gateway_node/include/config.h`:
```cpp
#define WIFI_SSID "YourNetwork"
#define WIFI_PASSWORD "YourPassword"
```

### Firebase Settings (Gateway Node)
Edit `gateway_node/include/config.h`:
```cpp
#define FIREBASE_HOST "your-project.firebaseio.com"
#define FIREBASE_AUTH "your-auth-token"
```

### ESP-NOW MAC Addresses
After first upload, get MAC addresses from serial monitor, then update:
- `gateway_node/include/config.h` - Set soil & weather node MACs
- `soil_node/include/config.h` - Set gateway MAC
- `weather_node/include/config.h` - Set gateway MAC

## 🎯 Next Steps

1. **Build**: `.\build_all.ps1`
2. **Upload**: Connect each ESP32 and run `.\upload_all.ps1`
3. **Monitor**: Use `.\monitor.ps1` to verify operation
4. **Dashboard**: Open `dashboard/index.html` to view data

## 📚 Documentation

- [FIRMWARE_STRUCTURE.md](FIRMWARE_STRUCTURE.md) - Detailed architecture
- [platformio.ini](gateway_node/platformio.ini) - Build configuration
- [config.h](gateway_node/include/config.h) - System configuration

## 🆘 Support

For issues:
1. Check serial output for error messages
2. Verify all connections and configurations
3. Review FIRMWARE_STRUCTURE.md for details
