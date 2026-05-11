@echo off
setlocal

if exist build (
    rmdir /s /q build
    echo Clean successful!
) else (
    echo No build directory to clean.
)

endlocal