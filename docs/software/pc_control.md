# PC Control Notes

电脑端脚本读取 STM32 串口输出，并用键盘事件控制 PPT、网页或其他桌面应用。

## 依赖

```bat
py -m pip install -r tools\requirements.txt
```

依赖包：

- `pyserial`
- `pyautogui`

## 运行命令

指定串口：

```bat
py tools\pc_gesture_controller.py --port COM5 --baud 115200
```

不指定串口时，脚本会列出当前可用串口并提示输入：

```bat
py tools\pc_gesture_controller.py
```

Windows 批处理入口：

```bat
tools\run_pc_controller_windows.bat
```

## COM 口占用注意事项

同一个 COM 口同一时间只能被一个程序占用。

- 运行 Python 控制脚本前，请关闭串口助手。
- 如果提示串口打开失败，通常是 COM 口被串口助手或其他程序占用。
- 关闭占用串口的程序后再重新运行脚本。

## PPT 控制映射

| STM32 串口输出 | 电脑按键 |
| --- | --- |
| `Gesture: RIGHT` | `Right` |
| `Gesture: LEFT` | `Left` |
| `Gesture: UP` | `Enter` |
| `Gesture: DOWN` | `Esc` |

运行脚本后，请点击 PPT 放映窗口，使其成为当前活动窗口。
