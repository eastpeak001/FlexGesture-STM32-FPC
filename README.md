# FlexGesture

FlexGesture：基于 FPC 柔性贴片的可穿戴手势控制器。

## 项目简介

FlexGesture 是一个面向可穿戴交互的手势控制器项目。目标硬件形态是基于
STM32F103C8T6、LSM6DSO 和 FPC 柔性电路板的一体化手背贴片，通过本地姿态
识别输出 `Gesture` 指令，再由电脑端 Python 程序转换为键盘控制，用于 PPT、
网页或其他桌面应用控制。

当前 V0.4-PC-Control-Demo 版本已经完成 MPU6050 面包板验证和 PPT 控制链路：
STM32 通过 I2C 读取 IMU 数据，识别 `RIGHT`、`LEFT`、`UP`，通过 USART1 输出
`Gesture: ...`，电脑端 Python 脚本读取串口并模拟键盘按键。

## 项目特点

- FPC 柔性结构，适合手背贴片式佩戴。
- 主控刚性岛 + BEND ZONE + 传感器岛的硬件结构。
- LSM6DSO 六轴姿态检测目标方案。
- STM32 本地手势识别，电脑端无需做复杂算法。
- 串口输出 `Gesture` 指令，易于调试和扩展。
- Python 上位机可控制 PPT / 网页 / 桌面应用。

## 系统结构

```text
手背传感器岛 -> BEND ZONE -> STM32 主控岛 -> UART -> PC Python -> PPT/网页控制
```

## 硬件组成

- STM32F103C8T6
- LSM6DSO
- Type-C 5V 输入
- AMS1117-3.3
- SWD 下载接口
- BOOT0 / NRST
- USART1 串口接口
- FPC 柔性电路板

## 引脚说明

| 功能 | STM32 引脚 |
| --- | --- |
| LSM6DSO SCL | PB6 |
| LSM6DSO SDA | PB7 |
| LSM6DSO INT1 | PB0 |
| LSM6DSO INT2 | PB1，可选 |
| USART1_TX | PA9 |
| USART1_RX | PA10 |
| SWDIO | PA13 |
| SWCLK | PA14 |

当前面包板验证版本使用 MPU6050：

| MPU6050 | STM32F103C8T6 |
| --- | --- |
| VCC | 3V3 |
| GND | GND |
| SCL | PB6 / I2C1_SCL |
| SDA | PB7 / I2C1_SDA |
| INT | 暂不连接 |

USB-TTL 连接：

| STM32 | USB-TTL |
| --- | --- |
| PA9 / USART1_TX | RX |
| PA10 / USART1_RX | TX |
| GND | GND |

串口波特率：`115200`。

## 软件说明

### STM32 固件

固件位于 `firmware/stm32/`。当前固件基于 STM32CubeIDE，主要功能：

- I2C1 读取 IMU 数据。
- 姿态状态机识别手势。
- USART1 输出启动信息和有效手势。
- 当前稳定识别 `RIGHT`、`LEFT`、`UP`。
- `DOWN` 暂时关闭，因为 `FLAT` 姿态用于中立复位。

### Python 上位机

电脑端脚本位于 `tools/pc_gesture_controller.py`，依赖：

- `pyserial`
- `pyautogui`

手势按键映射：

| 串口输出 | 键盘动作 |
| --- | --- |
| `Gesture: RIGHT` | `Right`，PPT 下一页 |
| `Gesture: LEFT` | `Left`，PPT 上一页 |
| `Gesture: UP` | `Enter` |
| `Gesture: DOWN` | `Esc` |

## 当前进度

- MPU6050 面包板验证完成。
- PPT 控制链路完成。
- LSM6DSO + STM32 一体化 FPC 设计完成。
- DRC 通过。
- FPC 打样和焊接测试待进行。

## 使用方法

1. 使用 ST-Link 下载 STM32 程序。
2. 使用 Type-C 或 3.3V/5V 方案给硬件供电，按实际板卡电源设计连接。
3. 连接 USB-TTL 或板载串口到电脑。
4. 确认串口波特率为 `115200`。
5. 关闭串口助手，避免 COM 口被占用。
6. 打开 PPT 放映或网页，并点击目标窗口让它成为当前活动窗口。
7. 安装并运行电脑端控制脚本：

```bat
py -m pip install -r tools\requirements.txt
py tools\pc_gesture_controller.py --port COM5
```

也可以运行：

```bat
tools\run_pc_controller_windows.bat
```

如果未指定 `--port`，脚本会列出当前可用串口并提示输入。

## 注意事项

- BEND ZONE 不建议大面积铺铜，避免影响弯折寿命。
- 主控岛和传感器岛建议做补强。
- Type-C 区域建议补强，降低插拔应力。
- LSM6DSO 是 LGA 封装，建议 SMT 或加热台低温锡膏焊接。
- 上电前检查 `3V3` 和 `GND` 是否短路。
- 串口助手和 Python 脚本不能同时占用同一个 COM 口。
- 如果 Python 提示 COM 口被占用，请关闭串口助手或其他串口程序后重试。

## 目录结构

```text
FlexGesture/
├── docs/          项目说明文档
├── firmware/      STM32 固件工程
├── hardware/      原理图、PCB、Gerber、BOM
├── media/         演示图片或视频占位
└── tools/         Python 上位机控制脚本
```

## License

This project is released under the MIT License. See `LICENSE` for details.
