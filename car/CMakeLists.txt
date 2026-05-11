cmake_minimum_required(VERSION 3.16)
project(car C ASM)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi.cmake)
set(CMAKE_BUILD_TYPE Debug)

include_directories(
    1_Platform/CMSIS/CM3/CoreSupport
    1_Platform/CMSIS/CM3/DeviceSupport/ST/STM32F10x
    2_HAL/STM32F10x_StdPeriph_Driver/inc
    3_BSP
    4_App
)

file(GLOB_RECURSE SOURCES
    1_Platform/CMSIS/CM3/CoreSupport/*.c
    1_Platform/CMSIS/CM3/DeviceSupport/ST/STM32F10x/*.c
    2_HAL/STM32F10x_StdPeriph_Driver/src/*.c
    3_BSP/*.c
    4_App/*.c
)

file(GLOB_RECURSE ASM_SOURCES
    1_Platform/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/*.s
)

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${ASM_SOURCES})

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    -DSTM32F10X_MD
    -DUSE_STDPERIPH_DRIVER
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103C8T6_FLASH.ld)
target_link_libraries(${PROJECT_NAME}.elf -T${LINKER_SCRIPT})

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set(OPENOCD_EXECUTABLE "D:/STM32_Dev/openocd-v0.12.0-i686-w64-mingw32/bin/openocd.exe")

add_custom_target(flash
    COMMAND ${OPENOCD_EXECUTABLE} -f ${CMAKE_SOURCE_DIR}/openocd-daplink.cfg -c "program ${PROJECT_NAME}.elf verify reset exit"
    DEPENDS ${PROJECT_NAME}.elf
)