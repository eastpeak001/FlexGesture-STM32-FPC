# STM32 Firmware Notes

## MCU

当前固件工程基于 STM32F103C8T6 和 STM32CubeIDE。

## I2C

- `I2C1_SCL` -> `PB6`
- `I2C1_SDA` -> `PB7`

当前面包板验证使用 MPU6050。FPC 版本目标 IMU 为 LSM6DSO，后续需要根据实际安装方向重新调整姿态映射和阈值。

## USART

- `USART1_TX` -> `PA9`
- `USART1_RX` -> `PA10`
- Baud rate: `115200`

固件启动后输出启动信息，识别到有效手势时输出：

```text
Gesture: RIGHT
Gesture: LEFT
Gesture: UP
Gesture: DOWN
```

## IMU Interrupt Pins

- `LSM6DSO INT1` -> `PB0`
- `LSM6DSO INT2` -> `PB1`，可选

当前固件主要使用轮询读取 IMU 数据，INT 引脚可在后续低功耗或中断触发版本中启用。

## SWD

- `SWDIO` -> `PA13`
- `SWCLK` -> `PA14`

## BOOT0 / NRST

- `BOOT0` 用于选择启动模式，正常运行时保持默认启动到用户 Flash。
- `NRST` 用于复位 MCU，建议保留测试点或按键。
