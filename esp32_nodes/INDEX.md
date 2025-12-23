# 📚 ESP32 Smart Farm - Documentation Index

Welcome to the ESP32 Smart Farm firmware documentation! This index will help you find the information you need.

## 🎯 Quick Navigation

### For Getting Started
- 📄 **[README.md](README.md)** - Start here! Quick setup and basic commands
- 📄 **[STRUCTURE_SUMMARY.md](STRUCTURE_SUMMARY.md)** - Overview of what was created

### For Understanding the System
- 📄 **[ARCHITECTURE.md](ARCHITECTURE.md)** - System design and components
- 📄 **[DIAGRAMS.md](DIAGRAMS.md)** - Visual diagrams and flowcharts
- 📄 **[FIRMWARE_STRUCTURE.md](FIRMWARE_STRUCTURE.md)** - Detailed firmware documentation

### For Development
- 📁 **[gateway_node/](gateway_node/)** - Central gateway code
- 📁 **[soil_node/](soil_node/)** - Soil monitoring code
- 📁 **[weather_node/](weather_node/)** - Weather monitoring code

### For Building & Deploying
- 🔧 **[build_all.ps1](build_all.ps1)** - Build all nodes
- 🔧 **[upload_all.ps1](upload_all.ps1)** - Upload firmware
- 🔧 **[monitor.ps1](monitor.ps1)** - Serial monitoring
- 🔧 **[clean.ps1](clean.ps1)** - Clean builds

---

## 📖 Documentation Guide by Task

### "I want to build the project"
1. Read [README.md](README.md) - Prerequisites section
2. Run `.\build_all.ps1`
3. If errors occur, check troubleshooting in [README.md](README.md)

### "I want to understand the architecture"
1. Start with [ARCHITECTURE.md](ARCHITECTURE.md) - System Overview
2. View diagrams in [DIAGRAMS.md](DIAGRAMS.md)
3. Deep dive in [FIRMWARE_STRUCTURE.md](FIRMWARE_STRUCTURE.md)

### "I want to configure the system"
1. Gateway Node: `gateway_node/include/config.h`
2. Soil Node: `soil_node/include/config.h`
3. Weather Node: `weather_node/include/config.h`
4. See configuration guide in [README.md](README.md)

### "I want to add new sensors"
1. Read [FIRMWARE_STRUCTURE.md](FIRMWARE_STRUCTURE.md) - "Adding New Sensors"
2. Update appropriate `config.h`
3. Modify data structures in `data_structures.h`
4. Update main .cpp file

### "I want to understand data flow"
1. View [DIAGRAMS.md](DIAGRAMS.md) - Data Flow Diagram
2. Read [ARCHITECTURE.md](ARCHITECTURE.md) - Data Flow section
3. Check packet structures in `gateway_node/include/data_structures.h`

### "I'm getting errors"
1. Check [README.md](README.md) - Troubleshooting section
2. Review error codes in node's `config.h`
3. Use `.\monitor.ps1` to see serial output
4. Check [FIRMWARE_STRUCTURE.md](FIRMWARE_STRUCTURE.md) - Error Handling

---

## 📂 File Structure Reference

```
esp32_nodes/
├── 📚 DOCUMENTATION (You are here)
│   ├── INDEX.md                    ← This file
│   ├── README.md                   ← Quick start guide
│   ├── STRUCTURE_SUMMARY.md        ← What was created
│   ├── ARCHITECTURE.md             ← System architecture
│   ├── DIAGRAMS.md                 ← Visual diagrams
│   └── FIRMWARE_STRUCTURE.md       ← Detailed firmware docs
│
├── 🔧 BUILD SCRIPTS
│   ├── build_all.ps1               ← Build all nodes
│   ├── upload_all.ps1              ← Upload to ESP32
│   ├── monitor.ps1                 ← Serial monitor
│   └── clean.ps1                   ← Clean builds
│
├── 📁 gateway_node/
│   ├── src/
│   │   └── gateway_node.cpp        ← Main code
│   ├── include/
│   │   ├── config.h                ← Configuration
│   │   └── data_structures.h       ← Data types
│   ├── platformio.ini              ← Build config
│   └── wokwi.toml                  ← Simulation config
│
├── 📁 soil_node/
│   ├── src/
│   │   └── soil_node.cpp           ← Main code
│   ├── include/
│   │   └── config.h                ← Configuration
│   ├── platformio.ini              ← Build config
│   └── wokwi.toml                  ← Simulation config
│
└── 📁 weather_node/
    ├── src/
    │   └── weather_node.cpp        ← Main code
    ├── include/
    │   └── config.h                ← Configuration
    ├── platformio.ini              ← Build config
    └── wokwi.toml                  ← Simulation config
```

---

## 🎓 Documentation Levels

### Level 1: Quick Start (5 minutes)
**File:** [README.md](README.md)
- Installation
- Build commands
- Upload commands
- Basic troubleshooting

### Level 2: Overview (15 minutes)
**Files:** [STRUCTURE_SUMMARY.md](STRUCTURE_SUMMARY.md), [DIAGRAMS.md](DIAGRAMS.md)
- What was created
- High-level architecture
- Visual diagrams
- Key features

### Level 3: Architecture (30 minutes)
**File:** [ARCHITECTURE.md](ARCHITECTURE.md)
- System design
- Communication protocols
- Hardware components
- Software layers
- Data flow
- Security

### Level 4: Deep Dive (1+ hours)
**File:** [FIRMWARE_STRUCTURE.md](FIRMWARE_STRUCTURE.md)
- Complete firmware details
- Data structures
- State machines
- API reference
- Coding standards
- Testing strategy
- Performance optimization

---

## 🔍 Quick Reference Tables

### Configuration Files
| Node | Config File | Purpose |
|------|-------------|---------|
| Gateway | `gateway_node/include/config.h` | WiFi, Firebase, pins, thresholds |
| Gateway | `gateway_node/include/data_structures.h` | Packet formats, data types |
| Soil | `soil_node/include/config.h` | Pins, calibration, ESP-NOW |
| Weather | `weather_node/include/config.h` | Pins, calibration, ESP-NOW |

### Build Scripts
| Script | Command | Purpose |
|--------|---------|---------|
| Build All | `.\build_all.ps1` | Compile all nodes |
| Upload All | `.\upload_all.ps1` | Flash all ESP32s |
| Monitor | `.\monitor.ps1 -Node gateway` | View serial output |
| Clean | `.\clean.ps1` | Remove build artifacts |

### Documentation Files
| File | Best For | Time to Read |
|------|----------|--------------|
| README.md | Getting started | 5 min |
| STRUCTURE_SUMMARY.md | Understanding what's included | 10 min |
| DIAGRAMS.md | Visual learners | 15 min |
| ARCHITECTURE.md | System design | 30 min |
| FIRMWARE_STRUCTURE.md | Developers | 60+ min |

---

## 💡 Common Questions

### Q: Where do I start?
**A:** Read [README.md](README.md) first, then run `.\build_all.ps1`

### Q: How do I change WiFi settings?
**A:** Edit `gateway_node/include/config.h`, lines with `WIFI_SSID` and `WIFI_PASSWORD`

### Q: How do I add a new sensor?
**A:** See [FIRMWARE_STRUCTURE.md](FIRMWARE_STRUCTURE.md) - "Adding New Sensors" section

### Q: What's the communication protocol?
**A:** Nodes use ESP-NOW, Gateway uses WiFi/HTTPS. See [ARCHITECTURE.md](ARCHITECTURE.md)

### Q: How do I debug issues?
**A:** Use `.\monitor.ps1 -Node <nodename>` to see serial output

### Q: Where are sensor thresholds defined?
**A:** In each node's `include/config.h` file

### Q: How do I update firmware?
**A:** Build with `.\build_all.ps1`, upload with `.\upload_all.ps1`

### Q: What are the memory requirements?
**A:** See [DIAGRAMS.md](DIAGRAMS.md) - "Memory Layout" section

---

## 🎯 Learning Path

### For Beginners
1. ✅ [README.md](README.md) - Get it running
2. ✅ [STRUCTURE_SUMMARY.md](STRUCTURE_SUMMARY.md) - Understand what you have
3. ✅ [DIAGRAMS.md](DIAGRAMS.md) - See how it works visually
4. ✅ Experiment with `config.h` files
5. ✅ Try modifying threshold values

### For Intermediate Users
1. ✅ [ARCHITECTURE.md](ARCHITECTURE.md) - System design
2. ✅ Study `data_structures.h`
3. ✅ Review main .cpp files
4. ✅ Add a new sensor
5. ✅ Customize LCD display

### For Advanced Users
1. ✅ [FIRMWARE_STRUCTURE.md](FIRMWARE_STRUCTURE.md) - Complete reference
2. ✅ Implement new communication protocols
3. ✅ Add OTA updates
4. ✅ Optimize power consumption
5. ✅ Implement mesh networking

---

## 🔧 Development Workflow

```
1. READ → Documentation (this guide)
           ↓
2. CONFIGURE → Edit config.h files
           ↓
3. BUILD → .\build_all.ps1
           ↓
4. UPLOAD → .\upload_all.ps1
           ↓
5. MONITOR → .\monitor.ps1
           ↓
6. DEBUG → Check serial output
           ↓
7. ITERATE → Back to step 2
```

---

## 📞 Getting Help

### Before Asking for Help
1. ✅ Check relevant documentation
2. ✅ Read troubleshooting in [README.md](README.md)
3. ✅ Check serial monitor output
4. ✅ Verify all configurations
5. ✅ Try clean build: `.\clean.ps1` then `.\build_all.ps1`

### When Reporting Issues
Include:
- Which node (gateway, soil, weather)
- Error message from serial monitor
- Build output (if build fails)
- Configuration changes made
- Steps to reproduce

---

## 📊 Feature Checklist

### System Features
- ✅ 16 sensor monitoring
- ✅ ESP-NOW communication
- ✅ Firebase cloud storage
- ✅ Web dashboard
- ✅ LCD display
- ✅ Alert generation
- ✅ Real-time updates
- ✅ Modular architecture

### Documentation Features
- ✅ Quick start guide
- ✅ Architecture documentation
- ✅ Visual diagrams
- ✅ Configuration reference
- ✅ Build automation
- ✅ Troubleshooting guide
- ✅ Code examples
- ✅ This index!

---

## 🎉 You're Ready!

Start with [README.md](README.md) and build your Smart Farm IoT system!

**Quick Commands:**
```powershell
# Build everything
.\build_all.ps1

# Upload to ESP32s
.\upload_all.ps1

# Monitor gateway node
.\monitor.ps1 -Node gateway

# Need help?
# Read README.md or check this INDEX.md
```

---

## 📅 Last Updated
December 11, 2025

## 👨‍💻 Created By
GitHub Copilot - Your AI Programming Assistant

---

**Happy Coding! 🚀**
