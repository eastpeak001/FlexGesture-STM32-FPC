# FlexGesture

**FlexGesture：基于 ESP32 + LSM6DSOTR 的柔性贴片手势控制器**

FlexGesture 是一个面向 FPC / 柔性贴片场景的可穿戴手势控制器。新版硬件使用 ESP32-WROOM-32E-N8 和 LSM6DSOTR 六轴姿态传感器，通过 I2C 读取姿态数据；当前目标是完成 USB 串口调试链路，后续扩展为 BLE HID 蓝牙手势控制器，用于 PPT 翻页、网页控制等场景。

## Current Status

- **V1 STM32/FPC 版本**：机械结构失败。第一块 FPC 最初完成 STM32 + LSM6DSO bring-up，但插针直接焊在 FPC 软板上，弯折后焊点和焊盘长期受力，导致信号传输极不稳定，后续焊点/焊盘脱落；第二块备用 FPC 板无信号输出。结论是第一版主要失败在机械连接和受力设计，不代表 LSM6DSO 方案本身不可行。
- **V2 ESP32 版本**：重新设计主控、电源、充电、蓝牙和姿态传感器连接。PCB design completed / ready for bring-up，硬件 bring-up 待进行。

## Highlights

- ESP32-WROOM-32E-N8 主控，后续可直接使用 ESP32 BLE。
- LSM6DSOTR 六轴 IMU，I2C 接口。
- Type-C 供电和串口烧录。
- CH340C USB-UART。
- 自动下载电路，保留 BOOT / RESET 手动按键。
- TP4055 单节 3.7V 锂电池充电，PROG 电阻为 10kΩ。
- TPS63031DSKR 3.3V 升降压供电，搭配 1.5uH 电感。
- USB / BAT 自动电源切换。
- 完善测试点，方便 bring-up 和故障定位。
- 根据 V1 FPC 失败经验，强调连接器、补强和机械受力设计。

## System Overview

```text
Hand motion
  -> LSM6DSOTR IMU
  -> ESP32-WROOM-32E-N8
  -> USB serial debug / future BLE HID
  -> PC / PPT / browser control
```

## Hardware Connections

| Function | Connection |
| --- | --- |
| LSM6DSOTR SDA | ESP32 IO21 |
| LSM6DSOTR SCL | ESP32 IO22 |
| LSM6DSOTR INT1 | ESP32 IO27 |
| LSM6DSOTR INT2 | ESP32 IO26 |
| CH340C TXD | ESP32 RXD0 |
| CH340C RXD | ESP32 TXD0 |
| BOOT button | ESP32 IO0 |
| RESET button | ESP32 EN |
| TPS63031 EN | PWR_EN |
| Battery | BAT / GND |

LSM6DSOTR uses I2C mode. SDA/SCL each have 4.7kΩ pull-ups to 3V3. CS is tied to 3V3. SDO/SA0 is tied to GND, so the expected I2C address is `0x6A`. SDX/SCX, SDO_AUX and OCS_AUX are not used and remain NC.

## Power Architecture

```text
Type-C VIN_5V -> TP4055 -> BAT
Type-C VIN_5V -> B5819W -> SYS_IN
BAT           -> AO3401A -> SYS_IN
SYS_IN        -> TPS63031 -> 3V3
3V3           -> ESP32 / CH340C / LSM6DSOTR
```

Power design notes:

- Type-C provides `VIN_5V`.
- TP4055 charges a single-cell 3.7V Li-ion pouch battery, charge limit 4.2V.
- Battery target: 3.7V, about 1.11Wh / 300mAh, around 20mm x 30mm.
- TP4055 PROG resistor is 10kΩ for a small pouch cell.
- TPS63031DSKR generates the 3V3 rail from `SYS_IN`.
- USB/BAT power-path switching uses VIN_5V through B5819W to `SYS_IN`, and BAT through AO3401A P-MOS to `SYS_IN`.

## Debug Test Points

The V2 board includes test points for:

- VIN_5V
- SYS_IN
- 3V3
- BAT
- GND
- TXD0
- RXD0
- I2C_SDA
- I2C_SCL
- EN
- IO0
- PWR_EN

## Firmware Plan

- I2C scanner.
- LSM6DSOTR `WHO_AM_I` read.
- Serial output for raw IMU data.
- Gesture recognition: LEFT / RIGHT / UP / DOWN / TAP / SHAKE.
- BLE HID Keyboard for PPT page control.
- Low-power optimization.

## Manufacturing Notes

- ESP32 antenna area must keep out copper, traces and vias.
- USB D+ / D- should be short, parallel and use as few vias as possible.
- TPS63031 inductor and input/output capacitors must be placed close to the IC.
- Q3 AO3401A G/S/D package mapping must be manually checked before ordering.
- SW3 MSK12C02 common terminal must be manually checked.
- C12 is DNP and should not be assembled by default.
- BAT+ / BAT- silkscreen must be checked before battery connection.
- FPC versions need PI/FR4 stiffeners around ESP32, LSM6DSOTR, Type-C, battery pads, switches and power area.

## Repository Notes

- `FlexGesture/` contains the legacy STM32CubeIDE firmware project and bring-up history.
- `docs/` contains V2 ESP32 hardware notes and bring-up checklist.
- `hardware/`, `production/` and `firmware/` are prepared for hardware design files, production outputs and future ESP32 firmware.

## License

MIT License. See `LICENSE` if present.
