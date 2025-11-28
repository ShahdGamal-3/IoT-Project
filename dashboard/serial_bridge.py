#!/usr/bin/env python3
"""
Serial Bridge for Dashboard-to-ESP32 Communication
Connects web dashboard to ESP32 via Serial port using WebSocket
"""

import asyncio
import json
import serial
import serial.tools.list_ports
from websockets.server import serve
import sys

# Configuration
SERIAL_PORT = "COM5"  # Updated to COM5
SERIAL_BAUD = 115200
WEBSOCKET_HOST = "localhost"
WEBSOCKET_PORT = 8765

class SerialBridge:
    def __init__(self):
        self.serial_conn = None
        self.websocket_clients = set()
        
    def find_serial_port(self):
        """Auto-detect ESP32 serial port"""
        ports = serial.tools.list_ports.comports()
        print("\n📡 Available Serial Ports:")
        for i, port in enumerate(ports):
            print(f"  {i+1}. {port.device} - {port.description}")
        
        if not ports:
            print("❌ No serial ports found!")
            return None
            
        # Try to find ESP32
        for port in ports:
            if "CH340" in port.description or "CP210" in port.description or "USB-SERIAL" in port.description:
                print(f"\n✅ Auto-detected ESP32 on: {port.device}")
                return port.device
        
        # Use first port if no ESP32 found
        print(f"\n⚠️  Using first available port: {ports[0].device}")
        return ports[0].device
    
    def connect_serial(self, port=None):
        """Connect to ESP32 via Serial"""
        try:
            if port is None:
                port = self.find_serial_port()
                if port is None:
                    return False
            
            self.serial_conn = serial.Serial(
                port=port,
                baudrate=SERIAL_BAUD,
                timeout=0.1
            )
            print(f"✅ Serial connected: {port} @ {SERIAL_BAUD} baud")
            return True
            
        except serial.SerialException as e:
            print(f"❌ Serial connection failed: {e}")
            print(f"\n💡 Tips:")
            print(f"   - Make sure Wokwi simulation is running")
            print(f"   - Check if COM port is correct")
            print(f"   - Try running: python find_com_port.py")
            return False
    
    async def handle_websocket(self, websocket):
        """Handle WebSocket connection from dashboard"""
        self.websocket_clients.add(websocket)
        client_addr = websocket.remote_address
        print(f"🌐 Dashboard connected: {client_addr}")
        
        try:
            async for message in websocket:
                try:
                    # Parse JSON from dashboard
                    data = json.loads(message)
                    sensor = data.get('command', data.get('sensor', ''))
                    value = data.get('value', 0)
                    
                    # Send to ESP32 via Serial
                    json_cmd = json.dumps({"sensor": sensor, "value": value})
                    if self.serial_conn and self.serial_conn.is_open:
                        self.serial_conn.write((json_cmd + "\n").encode())
                        print(f"📤 Sent to ESP32: {sensor} = {value}")
                    else:
                        print("⚠️  Serial not connected - command not sent")
                        
                except json.JSONDecodeError:
                    print(f"⚠️  Invalid JSON: {message}")
                except Exception as e:
                    print(f"❌ Error processing message: {e}")
                    
        except Exception as e:
            print(f"❌ WebSocket error: {e}")
        finally:
            self.websocket_clients.remove(websocket)
            print(f"🔌 Dashboard disconnected: {client_addr}")
    
    async def read_serial(self):
        """Read from ESP32 and send to dashboard"""
        while True:
            try:
                if self.serial_conn and self.serial_conn.is_open and self.serial_conn.in_waiting:
                    line = self.serial_conn.readline().decode('utf-8', errors='ignore').strip()
                    if line:
                        print(f"📥 ESP32: {line}")
                        
                        # Forward to all connected dashboards
                        if self.websocket_clients:
                            message = json.dumps({"type": "serial", "data": line})
                            await asyncio.gather(
                                *[client.send(message) for client in self.websocket_clients],
                                return_exceptions=True
                            )
                
                await asyncio.sleep(0.01)  # Small delay to prevent CPU overload
                
            except Exception as e:
                print(f"❌ Serial read error: {e}")
                await asyncio.sleep(1)
    
    async def start_server(self):
        """Start WebSocket server"""
        print(f"\n🚀 Starting Serial Bridge...")
        print(f"   WebSocket Server: ws://{WEBSOCKET_HOST}:{WEBSOCKET_PORT}")
        print(f"   Serial Port: {SERIAL_PORT}")
        
        if not self.connect_serial(SERIAL_PORT):
            print("\n⚠️  Serial connection failed. WebSocket server will still run.")
            print("   You can test the dashboard, but ESP32 won't receive commands.")
        
        async with serve(self.handle_websocket, WEBSOCKET_HOST, WEBSOCKET_PORT):
            print(f"✅ WebSocket server running!")
            print(f"\n📋 System Status:")
            print(f"   • HTTP Server: http://localhost:8080")
            print(f"   • WebSocket: ws://{WEBSOCKET_HOST}:{WEBSOCKET_PORT}")
            print(f"   • Serial: {'✅ Connected' if self.serial_conn and self.serial_conn.is_open else '❌ Not connected'}")
            print(f"\n🎮 Usage:")
            print(f"   1. Open dashboard: http://localhost:8080")
            print(f"   2. Move sliders to control sensors")
            print(f"   3. Watch LCD update in Wokwi!")
            print(f"\n✋ Press Ctrl+C to stop\n")
            
            # Run serial reader in parallel
            await self.read_serial()

def main():
    """Main entry point"""
    print("=" * 60)
    print("  🌾 Smart Farm IoT - Serial Bridge")
    print("  Dashboard ↔ WebSocket ↔ Serial ↔ ESP32")
    print("=" * 60)
    
    bridge = SerialBridge()
    
    try:
        asyncio.run(bridge.start_server())
    except KeyboardInterrupt:
        print("\n\n👋 Serial Bridge stopped")
    except Exception as e:
        print(f"\n❌ Fatal error: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)

if __name__ == "__main__":
    main()
