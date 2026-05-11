@echo off
echo Building STM32F103C8T6 Project...

D:\STM32_Dev\cmake-4.3.0-windows-x86_64\bin\cmake.exe -G Ninja -B build -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi.cmake -DCMAKE_MAKE_PROGRAM=D:\STM32_Dev\ninja\ninja.exe
D:\STM32_Dev\ninja\ninja.exe -C build

if %errorlevel% equ 0 (
    echo Build successful!
) else (
    echo Build failed!
    exit /b 1
)