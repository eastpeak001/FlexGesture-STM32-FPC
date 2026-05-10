\# FlexGesture FPC Hardware Plan



\## Project Positioning



FlexGesture is a flexible FPC gesture sensing patch designed to be attached to the back of the right hand. The FPC patch uses an LSM6DSO 6-axis IMU to capture hand posture and motion. An external STM32F103 controller reads the IMU through I2C, recognizes gestures, and sends gesture commands to a PC through UART.



\## System Architecture



FlexGesture FPC Patch

→ 5Pin 0.5mm FPC Gold Finger

→ FPC-to-2.54mm Adapter Board

→ STM32F103C8T6 Controller

→ USB-TTL

→ PC Python Controller

→ PPT / Web Control



\## FPC Patch Components



\- LSM6DSO 6-axis IMU

\- 0.1uF decoupling capacitor

\- 1uF power filter capacitor

\- 4.7uF input capacitor

\- 4.7kΩ I2C pull-up resistor for SCL

\- 4.7kΩ I2C pull-up resistor for SDA

\- Test points: 3V3, GND, SCL, SDA, INT1

\- 5Pin 0.5mm FPC gold finger



\## FPC Connector Pin Definition



| Pin | Signal | Description |

|---|---|---|

| Pin1 | 3V3 | Power input |

| Pin2 | GND | Ground |

| Pin3 | SCL | I2C clock |

| Pin4 | SDA | I2C data |

| Pin5 | INT1 | LSM6DSO interrupt output, reserved |



\## STM32 Connection



| FPC Signal | STM32 Pin |

|---|---|

| 3V3 | 3V3 |

| GND | GND |

| SCL | PB6 / I2C1\_SCL |

| SDA | PB7 / I2C1\_SDA |

| INT1 | Reserved GPIO |



\## Design Notes



\- The FPC patch only contains the sensor front-end.

\- The STM32 controller, USB-TTL, OLED, battery, and PC interface are kept outside the FPC.

\- The FPC tail points toward the wrist.

\- The sensor side faces outward.

\- The final FPC should include clear silkscreen marks: FlexGesture, WRIST, SENSOR, 3V3, GND, SCL, SDA, INT.

