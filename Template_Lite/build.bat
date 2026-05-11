@echo off
setlocal

set PATH=D:\STM32_Dev\cmake-4.3.0-windows-x86_64\bin;D:\STM32_Dev\arm-gnu-toolchain-15.2.rel1-mingw-w64-i686-arm-none-eabi\bin;%PATH%

if not exist build (
    mkdir build
)

cd build
cmake .. -G "MinGW Makefiles"
if %errorlevel% neq 0 (
    echo CMake failed!
    pause
    exit /b %errorlevel%
)

make -j4
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)

echo Build successful!
cd ..
endlocal