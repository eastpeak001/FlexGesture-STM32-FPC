# FlexGesture V2 Hardware Update

## Why Switch From STM32/FPC V1 to ESP32

The STM32/FPC V1 board proved that the IMU bring-up path could work, but it also exposed a mechanical weakness in the first flexible board concept. The redesign moves the project toward an ESP32-based wearable controller with integrated Bluetooth, better power architecture and clearer bring-up test points.

The switch to ESP32-WROOM-32E-N8 is mainly for integration:

- Built-in BLE for future BLE HID gesture control.
- Type-C USB-UART programming through CH340C.
- Larger software ecosystem for rapid bring-up and HID experiments.
- Fewer external modules than a separate MCU + Bluetooth design.

## V1 Failure Summary

- Pin headers were soldered directly onto the FPC soft board.
- Bending transferred force into solder joints and pads.
- Signal transmission became unstable after repeated flexing.
- Pads and solder joints later detached.
- A second backup FPC board produced no output.
- The failure is judged to be mechanical connection and stress design, not proof that the LSM6DSO/LSM6DSOTR sensing approach is invalid.

## ESP32 Version Improvements

- ESP32-WROOM-32E-N8 main controller with integrated Bluetooth.
- Type-C power and serial flashing.
- CH340C USB-UART and auto-download circuit.
- BOOT / RESET manual buttons retained.
- TP4055 single-cell Li-ion battery charging.
- TPS63031DSKR 3.3V buck-boost rail.
- USB/BAT automatic power-path switching.
- More debug test points for power, UART, I2C and control pins.
- LSM6DSOTR moved away from the area below the ESP32 module to reduce mechanical and assembly interference.
- Mechanical notes now emphasize connectors, stiffeners and strain relief.

## Power Design

```text
Type-C VIN_5V -> TP4055 -> BAT
Type-C VIN_5V -> B5819W -> SYS_IN
BAT           -> AO3401A -> SYS_IN
SYS_IN        -> TPS63031 -> 3V3
3V3           -> ESP32 / CH340C / LSM6DSOTR
```

Key points:

- TP4055 charges one 3.7V Li-ion cell with a 4.2V charge limit.
- PROG resistor is 10kΩ for the small 300mAh class pouch battery.
- TPS63031DSKR uses a 1.5uH inductor and should have tight input/output capacitor placement.
- VIN_5V and BAT feed `SYS_IN` through a simple automatic power-path arrangement.

## LSM6DSOTR Connection

- Interface: I2C.
- SDA -> ESP32 IO21.
- SCL -> ESP32 IO22.
- SDA/SCL pull-up: 4.7kΩ to 3V3.
- CS -> 3V3.
- SDO/SA0 -> GND, expected address `0x6A`.
- INT1 -> ESP32 IO27.
- INT2 -> ESP32 IO26.
- SDX/SCX, SDO_AUX and OCS_AUX remain NC.

## PCB Layout Notes

- ESP32 antenna faces outward from the board edge.
- ESP32 antenna area needs keep-out for copper, traces and vias.
- Type-C connector is placed on the board edge.
- CH340C stays close to Type-C.
- TPS63031 power section is kept compact.
- GND copper pour is used.
- USB D+ / D- should remain short, parallel and use minimal vias.
- Before ordering, manually verify Q3 AO3401A G/S/D mapping.
- Before ordering, manually verify SW3 MSK12C02 common terminal.
- C12 is DNP optional and should not be mounted by default.

## Bring-up Steps

1. Do not solder the battery first; power the board from USB.
2. Measure `VIN_5V`.
3. Measure `SYS_IN`.
4. Measure `3V3`.
5. Measure ESP32 `EN`.
6. Check CH340C USB enumeration.
7. Flash a simple ESP32 blink or serial test firmware.
8. Run I2C scan and check for LSM6DSOTR at `0x6A`.
9. Read LSM6DSOTR `WHO_AM_I`; expected value is `0x6C`.
10. Output raw accelerometer and gyroscope data over serial.
11. After USB bring-up passes, test battery charging.
12. Test battery-powered operation.
