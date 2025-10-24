# DebugThing

## Setup
Aquire a Raspberry Pi Pico (2W/W(H))

Attach wires to GPIO26_A0 and GPIO27_AGND or attach
more probes attach wires to GPIO27_A1 and GPIO28_A2

then prob voltages 3.3v or lower as specified by the Pico family.

If you want to, for example, use 5v, use a voltage regulator
or an appropriate resistor (1kΩ resistor for example, make SURE to calculate based on ohms law).
Any voltage above has the potential to damage the Pico's hardware.

## Roadmap
- [x] Pico Support
- [ ] ESP32 Support
- [ ] Voltage Drop detection
- [ ] Reference voltage verification
- [ ] Clock signal verification
- [ ] Jitter measurement
- [ ] Arduino support is not planned nor is STM support
