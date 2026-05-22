# Hardware

This directory is prepared for FlexGesture V2 ESP32 hardware design files, schematic exports, PCB files, Gerber files and assembly notes.

Current hardware status:

- ESP32-WROOM-32E-N8 controller selected.
- LSM6DSOTR I2C connection prepared.
- TP4055 charging and TPS63031 3.3V buck-boost power architecture prepared.
- PCB design completed / ready for bring-up.

Before manufacturing or assembly, manually verify:

- Q3 AO3401A G/S/D footprint mapping.
- SW3 MSK12C02 common terminal.
- C12 DNP setting.
- ESP32 antenna keep-out.
- BAT+ / BAT- silkscreen.
