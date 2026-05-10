import argparse
import sys
import time

import pyautogui
import serial
from serial.tools import list_ports


GESTURE_KEY_MAP = {
    "RIGHT": "right",
    "LEFT": "left",
    "UP": "enter",
    "DOWN": "esc",
}

COOLDOWN_SECONDS = 0.4


def list_available_ports():
    return list(list_ports.comports())


def print_available_ports(ports):
    if not ports:
        print("No serial ports found.")
        return

    print("Available serial ports:")
    for port in ports:
        description = port.description or "Unknown device"
        print(f"  {port.device} - {description}")


def choose_port(port_arg):
    ports = list_available_ports()
    port_names = {port.device.upper(): port.device for port in ports}

    if port_arg:
        normalized = port_arg.upper()
        if normalized in port_names:
            return port_names[normalized]

        print(f"Serial port '{port_arg}' was not found.")
        print_available_ports(ports)
        return None

    print_available_ports(ports)
    if not ports:
        return None

    selected = input("Enter COM port, for example COM5: ").strip()
    if not selected:
        print("No COM port entered.")
        return None

    return selected


def parse_gesture(line):
    prefix = "Gesture:"
    if not line.startswith(prefix):
        return None

    gesture = line[len(prefix):].strip().upper()
    if gesture in GESTURE_KEY_MAP:
        return gesture

    return None


def print_startup(port, baud):
    print("FlexGesture PC Controller started")
    print("Please close any serial monitor first. A COM port can only be used by one program at a time.")
    print("Please click the PowerPoint slide show window so it becomes the active window.")
    print(f"Serial: {port} @ {baud}")
    print("RIGHT -> keyboard right")
    print("LEFT  -> keyboard left")
    print("UP    -> keyboard enter")
    print("DOWN  -> keyboard esc")
    print("Press Ctrl+C to exit.")


def run_controller(port, baud):
    last_action_time = 0.0

    with serial.Serial(port=port, baudrate=baud, timeout=1) as ser:
        print_startup(port, baud)

        while True:
            raw = ser.readline()
            if not raw:
                continue

            line = raw.decode("utf-8", errors="replace").strip()
            if not line:
                continue

            print(line)

            gesture = parse_gesture(line)
            if gesture is None:
                continue

            now = time.monotonic()
            if (now - last_action_time) < COOLDOWN_SECONDS:
                print(f"Action skipped: cooldown {COOLDOWN_SECONDS:.1f}s")
                continue

            key = GESTURE_KEY_MAP[gesture]
            pyautogui.press(key)
            last_action_time = now
            print(f"Action: press {key}")


def build_parser():
    parser = argparse.ArgumentParser(
        description="Read FlexGesture STM32 serial output and control keyboard keys."
    )
    parser.add_argument("--port", help="Serial port, for example COM5")
    parser.add_argument("--baud", type=int, default=115200, help="Serial baud rate")
    return parser


def main():
    args = build_parser().parse_args()
    port = choose_port(args.port)
    if port is None:
        return 1

    try:
        run_controller(port, args.baud)
    except serial.SerialException as exc:
        print(f"Serial error: {exc}")
        print("The COM port may be used by a serial monitor or another program. Please close it and try again.")
        print_available_ports(list_available_ports())
        return 1
    except KeyboardInterrupt:
        print("\nFlexGesture PC Controller stopped.")
        return 0

    return 0


if __name__ == "__main__":
    sys.exit(main())
