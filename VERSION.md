# FlexGesture Version Marker

## V0.4-PC-Control-Demo

Date: 2026-05-10

This project state is marked as `V0.4-PC-Control-Demo`.

Status:

- STM32F103C8T6 reads MPU6050 data over I2C.
- The pose-based gesture state machine is working.
- `RIGHT`, `LEFT`, and `UP` can be recognized and emitted over USART1.
- `DOWN` is temporarily disabled because `HAND_POSE_FLAT` is used as the neutral reset pose.
- The PC Python controller reads `Gesture: ...` serial lines and simulates keyboard input.
- PowerPoint slide control has been tested successfully.
- The serial monitor and Python controller cannot use the same COM port at the same time.
- Debug build has passed with `0 errors, 0 warnings`.

## V0.3-stable-before-fpc

Date: 2026-05-10

This project state is marked as `V0.3-stable-before-fpc`.

Status:

- STM32F103C8T6 and MPU6050 communication is working.
- The pose-based gesture state machine is working.
- `RIGHT`, `LEFT`, and `UP` can be recognized.
- `DOWN` is temporarily disabled because `HAND_POSE_FLAT` is used as the neutral reset pose.
- The later FPC version should retune pose mapping and thresholds for LSM6DSO.
- Debug build has passed with `0 errors, 0 warnings`.
