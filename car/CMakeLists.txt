# CMake 最低版本要求
cmake_minimum_required(VERSION 3.16)

# 项目名称和语言（C语言和汇编）
project(car C ASM)

# 指定工具链文件路径（ARM交叉编译工具链配置）
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi.cmake)

# 设置构建类型为Debug（包含调试信息）
set(CMAKE_BUILD_TYPE Debug)

# ============================================================================
# 头文件包含路径
# ============================================================================
include_directories(
    1_Platform/CMSIS/CM3/CoreSupport                    # CMSIS核心支持文件
    1_Platform/CMSIS/CM3/DeviceSupport/ST/STM32F10x     # STM32F10x设备支持文件
    2_HAL/STM32F10x_StdPeriph_Driver/inc                # 标准外设库头文件
    3_BSP                                               # 板级支持包（BSP驱动）
    4_App                                               # 应用层代码
)

# ============================================================================
# 收集C语言源文件
# ============================================================================
file(GLOB_RECURSE SOURCES
    1_Platform/CMSIS/CM3/CoreSupport/*.c                # CMSIS核心源文件
    1_Platform/CMSIS/CM3/DeviceSupport/ST/STM32F10x/*.c # 设备初始化源文件
    2_HAL/STM32F10x_StdPeriph_Driver/src/*.c            # 标准外设库源文件
    3_BSP/*.c                                           # BSP驱动源文件（含PID）
    4_App/*.c                                           # 应用层源文件
)

# ============================================================================
# 收集汇编启动文件
# ============================================================================
file(GLOB_RECURSE ASM_SOURCES
    1_Platform/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/*.s  # STM32启动代码
)

# ============================================================================
# 创建可执行目标
# ============================================================================
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${ASM_SOURCES})

# ============================================================================
# 编译定义（预处理器宏）
# ============================================================================
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    -DSTM32F10X_MD          # 定义中等容量设备（STM32F103C8T6属于MD系列）
    -DUSE_STDPERIPH_DRIVER  # 启用标准外设库
)

# ============================================================================
# 链接器脚本配置
# ============================================================================
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103C8T6_FLASH.ld)
target_link_libraries(${PROJECT_NAME}.elf -T${LINKER_SCRIPT})

# ============================================================================
# 构建后处理命令（生成hex和bin文件）
# ============================================================================
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex      # 生成Intel HEX格式文件
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin    # 生成二进制格式文件
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf                                     # 显示固件大小信息
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ============================================================================
# 设置输出目录属性
# ============================================================================
set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}  # 输出到build目录
)

# ============================================================================
# OpenOCD烧录配置
# ============================================================================
set(OPENOCD_EXECUTABLE "D:/STM32_Dev/openocd-v0.12.0-i686-w64-mingw32/bin/openocd.exe")

# 创建flash目标：使用OpenOCD烧录固件到开发板
add_custom_target(flash
    COMMAND ${OPENOCD_EXECUTABLE} -f ${CMAKE_SOURCE_DIR}/openocd-daplink.cfg -c "program ${PROJECT_NAME}.elf verify reset exit"
    DEPENDS ${PROJECT_NAME}.elf
)
