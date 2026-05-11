cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE arm-none-eabi.cmake)

project(STM32F103C8T6_Template C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CPU_PARAMETERS
    -mcpu=cortex-m3
    -mthumb
    -mfpu=vfp
    -mfloat-abi=soft
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/1_Platform/Linker/STM32F103C8T6_FLASH.ld)

set(EXECUTABLE ${PROJECT_NAME}.elf)

include_directories(
    1_Platform/CMSIS
    1_Platform/Device
    2_HAL/STM32F10x_StdPeriph_Driver/inc
    3_BSP/Inc
    4_App/Inc
)

file(GLOB_RECURSE SOURCES
    1_Platform/CMSIS/*.c
    1_Platform/Device/*.c
    1_Platform/Startup/*.s
    2_HAL/STM32F10x_StdPeriph_Driver/src/*.c
    3_BSP/Src/*.c
    4_App/Src/*.c
)

add_executable(${EXECUTABLE} ${SOURCES})

target_compile_definitions(${EXECUTABLE} PRIVATE
    STM32F10X_MD
    USE_STDPERIPH_DRIVER
)

target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<CONFIG:Debug>:-Og -g3>
    $<$<CONFIG:Release>:-O2>
)

target_link_options(${EXECUTABLE} PRIVATE
    -T${LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--gc-sections
    -Wl,--print-memory-usage
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} ${EXECUTABLE}
)

set_target_properties(${EXECUTABLE} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)