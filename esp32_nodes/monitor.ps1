# Monitor ESP32 Serial Output
# PowerShell script to monitor serial output from specified node

param(
    [Parameter(Mandatory=$false)]
    [ValidateSet("gateway", "soil", "weather")]
    [string]$Node = "gateway",
    
    [Parameter(Mandatory=$false)]
    [string]$Port = ""
)

Write-Host "================================" -ForegroundColor Cyan
Write-Host "ESP32 Serial Monitor" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host ""

# Default COM ports
$ports = @{
    "gateway" = "COM3"
    "soil" = "COM4"
    "weather" = "COM5"
}

# Determine which node to monitor
$nodeFolder = $Node + "_node"
$comPort = if ($Port) { $Port } else { $ports[$Node] }

Write-Host "Monitoring: $nodeFolder" -ForegroundColor Yellow
Write-Host "COM Port: $comPort" -ForegroundColor Yellow
Write-Host "Baud Rate: 115200" -ForegroundColor Yellow
Write-Host ""
Write-Host "Press Ctrl+C to exit" -ForegroundColor Gray
Write-Host "--------------------------------" -ForegroundColor Gray
Write-Host ""

Push-Location $nodeFolder

# Update port in platformio.ini
$iniContent = Get-Content platformio.ini
$iniContent = $iniContent -replace "monitor_port = COM\d+", "monitor_port = $comPort"
$iniContent | Set-Content platformio.ini

# Start monitoring
pio device monitor

Pop-Location
