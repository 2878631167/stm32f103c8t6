@echo off
echo Flashing STM32F103C8T6...

D:\STM32_Dev\cmake-4.3.0-windows-x86_64\bin\cmake.exe --build build --target flash

if %errorlevel% equ 0 (
    echo Flash successful!
) else (
    echo Flash failed!
    exit /b 1
)