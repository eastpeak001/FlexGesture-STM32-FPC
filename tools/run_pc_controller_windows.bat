@echo off
setlocal

cd /d "%~dp0\.."

echo Installing FlexGesture PC controller dependencies...
py -m pip install -r tools\requirements.txt
if errorlevel 1 (
    echo Failed to install dependencies.
    pause
    exit /b 1
)

echo.
echo Starting FlexGesture PC Controller...
py tools\pc_gesture_controller.py

echo.
pause
