@echo off
setlocal

set PATH=D:\STM32_Dev\openocd-v0.12.0-i686-w64-mingw32\bin;%PATH%

echo Flashing STM32F103C8T6...
openocd -f interface/cmsis-dap.cfg -f target/stm32f1x.cfg -c "program build/STM32F103C8T6_Template.hex reset exit"

if %errorlevel% equ 0 (
    echo Flash successful!
) else (
    echo Flash failed!
    pause
)

endlocal