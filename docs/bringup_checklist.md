# FlexGesture V2 Bring-up Checklist

## 1. Visual Inspection

- [ ] Check PCB orientation and component placement.
- [ ] Check ESP32 module soldering.
- [ ] Check Type-C connector soldering.
- [ ] Check CH340C soldering.
- [ ] Check LSM6DSOTR soldering.
- [ ] Check TPS63031, TP4055, AO3401A and B5819W orientation.
- [ ] Confirm C12 is DNP if not required.
- [ ] Confirm BAT+ / BAT- silkscreen before connecting a battery.

## 2. Short Check

- [ ] Check VIN_5V to GND resistance.
- [ ] Check SYS_IN to GND resistance.
- [ ] Check 3V3 to GND resistance.
- [ ] Check BAT to GND resistance before battery connection.

## 3. USB 5V Check

- [ ] Connect USB without battery.
- [ ] Measure VIN_5V.
- [ ] Measure SYS_IN.
- [ ] Confirm no abnormal heating.

## 4. TPS63031 3V3 Check

- [ ] Measure 3V3 output.
- [ ] Confirm 3V3 is stable.
- [ ] Confirm ESP32, CH340C and LSM6DSOTR receive 3V3.

## 5. CH340 Enumeration Check

- [ ] Connect USB to PC.
- [ ] Confirm CH340 serial port appears.
- [ ] Confirm no serial port disconnect/reconnect loop.

## 6. ESP32 Download Check

- [ ] Confirm EN level.
- [ ] Confirm IO0 / BOOT button behavior.
- [ ] Try automatic download.
- [ ] Try manual BOOT + RESET download if needed.
- [ ] Flash a minimal blink or UART test firmware.

## 7. I2C Scan Check

- [ ] Run I2C scanner on IO21/IO22.
- [ ] Confirm LSM6DSOTR responds at `0x6A`.
- [ ] If no device is found, check SDA/SCL pull-ups and soldering.

## 8. LSM Data Output Check

- [ ] Read LSM6DSOTR `WHO_AM_I`.
- [ ] Confirm expected value `0x6C`.
- [ ] Print raw accelerometer and gyroscope data.
- [ ] Move the board and confirm data changes.

## 9. Bluetooth Broadcast Check

- [ ] Load BLE test firmware.
- [ ] Confirm ESP32 BLE advertisement appears on PC/phone.
- [ ] Confirm reconnect behavior.

## 10. Battery Charging Check

- [ ] Connect battery after USB power tests pass.
- [ ] Confirm battery polarity.
- [ ] Measure charge current.
- [ ] Confirm TP4055 status behavior.
- [ ] Confirm battery voltage does not exceed 4.2V.

## 11. Battery Power Check

- [ ] Disconnect USB.
- [ ] Confirm SYS_IN is supplied from BAT through AO3401A path.
- [ ] Confirm 3V3 remains stable.
- [ ] Confirm ESP32 can boot on battery.

## 12. Gesture Recognition Check

- [ ] Run raw IMU serial output.
- [ ] Confirm static orientation mapping.
- [ ] Test LEFT / RIGHT / UP / DOWN gestures.
- [ ] Test TAP / SHAKE if implemented.
- [ ] Test BLE HID keyboard mapping when firmware is ready.
