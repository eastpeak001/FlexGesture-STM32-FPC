# Firmware

This directory is reserved for the ESP32 firmware that will replace the legacy STM32/FPC prototype firmware.

Current firmware status:

- Legacy STM32CubeIDE prototype project remains in `FlexGesture/`.
- ESP32 V2 firmware is planned.
- Initial ESP32 firmware should start with UART bring-up, I2C scanner, LSM6DSOTR `WHO_AM_I`, raw IMU output and then gesture recognition.
