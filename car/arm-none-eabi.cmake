set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 关键：告诉 CMake 在测试编译器时只生成静态库，避免链接器问题
# 必须在设置编译器之前指定
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TOOLCHAIN_BIN_DIR "D:/STM32_Dev/arm-gnu-toolchain-15.2.rel1-mingw-w64-i686-arm-none-eabi/bin")
set(CMAKE_C_COMPILER "${TOOLCHAIN_BIN_DIR}/arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN_DIR}/arm-none-eabi-g++.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_BIN_DIR}/arm-none-eabi-gcc.exe")

set(FLAGS "-mcpu=cortex-m3 -mthumb -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS "${FLAGS} -g -O0 -Wall -Wextra -std=c99" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS "${FLAGS}" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections" CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)