# CMake 最低版本要求
cmake_minimum_required(VERSION 3.16)

# 项目名称和语言（C语言和汇编）
project(car C ASM)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 指定工具链文件路径（ARM交叉编译工具链配置）
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi.cmake)

# 设置构建类型为Debug（包含调试信息）
set(CMAKE_BUILD_TYPE Debug)

# ============================================================================
# 查找源文件
# ============================================================================
file(GLOB_RECURSE SOURCES 
    "4_App/*.c"
    "3_BSP/*.c"
    "2_HAL/STM32F10x_StdPeriph_Driver/src/*.c"
    "1_Platform/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c"
    "1_Platform/CMSIS/CM3/CoreSupport/core_cm3.c"
)

# 收集汇编启动文件
file(GLOB_RECURSE ASM_SOURCES
    "1_Platform/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/*.s"
)

# 排除不需要的文件 (如果有测试文件或其他需要排除的)
list(FILTER SOURCES EXCLUDE REGEX ".*/test_.*\\.c$")

# ============================================================================
# 创建可执行目标
# ============================================================================
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${ASM_SOURCES})

# ============================================================================
# 包含目录
# ============================================================================
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    4_App
    3_BSP
    2_HAL/STM32F10x_StdPeriph_Driver/inc
    1_Platform/CMSIS/CM3/CoreSupport
    1_Platform/CMSIS/CM3/DeviceSupport/ST/STM32F10x
)

# ============================================================================
# 编译定义（预处理器宏）
# ============================================================================
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    STM32F10X_MD          # 定义中等容量设备（STM32F103C8T6属于MD系列）
    USE_STDPERIPH_DRIVER  # 启用标准外设库
)

# ============================================================================
# 链接器脚本配置与链接选项
# ============================================================================
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103C8T6_FLASH.ld)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${PROJECT_NAME}.map
    --specs=nosys.specs
    --specs=nano.specs
    -u _printf_float
    -u _scanf_float
    -Wl,--gc-sections
)

# ============================================================================
# 构建后处理命令（生成hex和bin文件）
# ============================================================================
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}.elf>
    COMMENT "Generating hex and bin files"
)

# ============================================================================
# 设置输出目录属性
# ============================================================================
set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ============================================================================
# OpenOCD烧录配置
# ============================================================================
set(OPENOCD_EXECUTABLE "D:/STM32_Dev/openocd-v0.12.0-i686-w64-mingw32/bin/openocd.exe")

# 创建flash目标：使用OpenOCD烧录固件到开发板
add_custom_target(flash
    COMMAND ${OPENOCD_EXECUTABLE} -f ${CMAKE_SOURCE_DIR}/openocd-daplink.cfg -c "program ${PROJECT_NAME}.elf verify reset exit"
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Flashing to target"
)
