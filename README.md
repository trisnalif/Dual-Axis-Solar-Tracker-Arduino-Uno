<img width="741" height="486" alt="image" src="https://github.com/user-attachments/assets/956d3162-7cee-4440-b6ad-cd051c1c6eec" />

# Dual-Axis-Solar-Tracker-Arduino-Uno
================================================================================
DUAL-AXIS SOLAR TRACKER LIBRARY FOR PROTEUS  
================================================================================

1. OVERVIEW
This library provides a mixed-signal simulation model (`SOLARTRACKERTEP`) for a compact dual-axis solar tracker in Proteus. It simulates a 6V 1W solar panel, pan/tilt servos, four LDR sensors behind a shade cross, and real-time sun positioning based on solar time, latitude, and day of the year. Clicking the component during simulation opens an interactive pop-up with live telemetry, sun-path maps, graphs, event logs, and diagnostics.

2. KEY FEATURES & SPECIFICATIONS
- Solar Panel (PV+, PV-): 6V, 1W (7.2V open-circuit, 0.18A short-circuit). Calculates output based on direct sunlight angle, sky conditions (Clear/Cloudy), and compares efficiency against a fixed south-facing panel.
- Servos (PAN, TILT): Controlled via standard 544–2400 us PWM pulses (0–180°). Pan: 0° East, 90° South, 180° West. Tilt: 0° upright, 90° flat.
- Light Sensors (TL, TR, BL, BR): Four LDRs with a 1% grey filter cap (to prevent saturation) and a shade cross. Higher light intensity outputs higher voltage (0–5V).
- Power Supply (VCC, GND): Requires 5V DC for internal servos and LDR voltage dividers.
- Interactive Pop-Up: Displays live sun/panel angles, LDR voltages/lux, real-time graphs, event logs, and system health checks.

3. INSTALLATION (QUICK STEPS)
1. Close Proteus and extract the downloaded archive.
2. Copy `TEPTRACKER.LIB` and `ArduinoV4TEP.LIB` into Proteus's `LIBRARY` folder.
3. Copy `TEPTRACKER.DLL` and `TEPARDUINO.DLL` into Proteus's `MODELS` folder.
4. Delete `TEPTRACKER.IDX` from the `LIBRARY` folder if it exists.
5. Open Proteus and search for "TRACKER" in Pick Devices.

4. WIRING & DEMO SETUP
- LDRs (TL, TR, BL, BR) -> Arduino Analog Pins A0–A3
- Servos (PAN, TILT) -> Arduino PWM Pins D9, D10
- Power (VCC, GND) -> 5V Power Rail and Ground
- Panel Load & Sensing -> Connect PV- to Ground. Connect a 33-ohm dummy load resistor across PV+ and Ground. Use two equal 10k-ohm resistors as a voltage divider from PV+ to Ground, feeding the midpoint into Analog Pin A4 (scales 7.2V max down to <3.6V for safe 5V ADC reading).
- Demo Project: Open `Dual-Axis-Solar-Tracker-ArduinoUnoV4.pdsprj` (uses `SolarTracker_Demo.hex` at 9600 baud). Click the Arduino's USB port to view the built-in Serial Monitor (`park`, `track`, `rate <ms>`, `help`).

5. CONFIGURABLE PROPERTIES
Double-click the tracker component in Proteus to adjust:
- HOUR (0–24): Starting solar time (default: 6)
- SPEED (1–14400): Time multiplier (default: 3600 = 1 hour per simulated second)
- LATITUDE (0–66° N): Geographic latitude (default: 30)
- DAY (1–365): Day of the year (81 = Mar 21, 172 = Jun 21, 355 = Dec 21)
- SKY: `CLEAR` or `CLOUDY`
- PAN0 / TILT0: Initial panel angles

6. TROUBLESHOOTING & LIMITATIONS
- Panel won't move: Check 5V supply on VCC and PWM signals on PAN/TILT.
- Panel turns away from sun: Swap the LDR pairs in your code (brighter = higher voltage).
- Low power at noon: Ensure load resistance is near 36 ohms for maximum power transfer.
- Model Scope: Does not simulate temperature effects, wind load, servo gear torque, or southern hemisphere latitudes.
#Arduino #SolarTracker #DualAxis #EmbeddedSystems #Proteus #ElectronicsEngineering #RenewableEnergy #ControlSystems
