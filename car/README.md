# STM32F103C8T6 项目模板

基于标准库 V3.5.0 的 STM32F103C8T6 开发模板。

## 硬件配置

| 外设 | 引脚 | 说明 |
|------|------|------|
| LED | PC13 | 板载LED |
| OLED | PB13(SCL), PB12(SDA) | 0.96寸 I2C OLED |
| 蓝牙 | PB10(TX), PB11(RX) | JDY-31 (9600波特率) |
| 电机 | PA0-PA5 | L298N 双电机驱动 |

## 项目结构

```
Template_Lite/
├── 1_Platform/     # CMSIS 核心文件
├── 2_HAL/         # 标准库驱动
├── 3_BSP/         # 板级驱动
├── 4_App/         # 应用层
│   └── test/      # 测试代码
├── build/         # 构建输出
└── .trae/        # AI开发辅助
```

## 构建与烧录

```bash
# 构建
build.bat

# 烧录
flash.bat
```

## 驱动列表

- LED (bsp_led)
- 系统时钟 (bsp_clk)
- 延时 (bsp_delay)
- OLED (OLED)
- 蓝牙 (BT)
- 电机驱动 (L298N)
- PID控制算法 (PID) ⭐ 新增

## 工具链

- GCC: arm-none-eabi-gcc 15.2.1
- 构建: CMake + Ninja
- 烧录: OpenOCD + DAPLink

## VSCode 调试

按 F5 开始调试 (需配置 launch.json)

---

**芯片**: STM32F103C8T6
**主频**: 72MHz
**Flash**: 64KB
**RAM**: 20KB
