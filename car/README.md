# STM32F103C8T6 智能小车项目

基于 STM32F103C8T6（ARM Cortex-M3）的智能小车控制系统，采用分层架构设计，支持循迹、手动控制、蓝牙遥控等功能。

![STM32](https://img.shields.io/badge/MCU-STM32F103C8T6-blue)
![Clock](https://img.shields.io/badge/Clock-72MHz-green)
![Flash](https://img.shields.io/badge/Flash-64KB-orange)
![RAM](https://img.shields.io/badge/RAM-20KB-orange)

---

## 📋 目录

- [硬件配置](#硬件配置)
- [项目架构](#项目架构)
- [快速开始](#快速开始)
- [功能特性](#功能特性)
- [蓝牙命令手册](#蓝牙命令手册)
- [开发指南](#开发指南)
- [常见问题](#常见问题)

---

## 🔧 硬件配置

### 核心芯片
- **MCU**: STM32F103C8T6 (ARM Cortex-M3, 72MHz)
- **Flash**: 64KB
- **RAM**: 20KB

### 外设引脚映射

| 外设 | 引脚 | 说明 | 备注 |
|------|------|------|------|
| **LED** | PC13 | 板载 LED | 低电平点亮 |
| **OLED** | PB13 (SCL)<br>PB12 (SDA) | 0.96寸 I2C OLED | SSD1306 驱动 |
| **蓝牙** | PB10 (TX)<br>PB11 (RX) | JDY-31 蓝牙模块 | 9600 波特率 |
| **电机 A** | PA0 (IN1)<br>PA1 (IN2)<br>PA2 (ENA) | L298N 左电机 | PWM 调速 |
| **电机 B** | PA3 (IN3)<br>PA4 (IN4)<br>PA5 (ENB) | L298N 右电机 | PWM 调速 |
| **循迹传感器** | PB0-PB4 | 5路红外循迹模块 | 数字信号输入 |

### 电源要求
- **输入电压**: 7-12V DC（推荐 9V）
- **稳压输出**: 5V（供 MCU 和传感器）
- **最大电流**: 2A

---

## 🏗️ 项目架构

### 分层设计

```
┌─────────────────────────────────────┐
│         4_App (应用层)              │
│  ┌──────────┐  ┌────────────────┐  │
│  │ app_car  │  │  app_test      │  │
│  │ (业务逻辑)│  │  (测试框架)    │  │
│  └──────────┘  └────────────────┘  │
└──────────────┬──────────────────────┘
               │ 调用接口
┌──────────────▼──────────────────────┐
│       3_BSP (板级支持包)            │
│  LED | OLED | BT | L298N | Track   │
│  PID | SysTick | Delay | Clock     │
└──────────────┬──────────────────────┘
               │ 寄存器操作
┌──────────────▼──────────────────────┐
│       2_HAL (硬件抽象层)            │
│     STM32F10x 标准外设库 V3.5.0      │
└──────────────┬──────────────────────┘
               │ CMSIS
┌──────────────▼──────────────────────┐
│   1_Platform (平台层)               │
│   CMSIS Core + Device Support       │
└─────────────────────────────────────┘
```

### 目录结构

```
car/
├── 1_Platform/          # 平台层：CMSIS 核心文件
│   └── CMSIS/
│       └── CM3/
│           ├── CoreSupport/
│           └── DeviceSupport/ST/STM32F10x/
│
├── 2_HAL/               # 硬件抽象层：STM32 标准外设库
│   └── STM32F10x_StdPeriph_Driver/
│       ├── inc/         # 头文件
│       └── src/         # 源文件
│
├── 3_BSP/               # 板级支持包：硬件驱动
│   ├── bsp_led.h/c      # LED 驱动
│   ├── bsp_clk.h/c      # 系统时钟配置
│   ├── bsp_delay.h/c    # 延时函数（基于 SysTick）
│   ├── bsp_SysTick.h/c  # SysTick 定时器驱动
│   ├── OLED.h/c         # OLED 显示驱动（SSD1306）
│   ├── OLED_Data.h/c    # OLED 字符集和中文字库
│   ├── BT.h/c           # 蓝牙串口通信
│   ├── L298N.h/c        # L298N 电机驱动
│   ├── Track.h/c        # 循迹传感器驱动
│   ├── PID.h/c          # PID 控制算法
│   ├── Board_Config.h   # 硬件引脚配置
│   └── Bsp.h            # BSP 统一头文件
│
├── 4_App/               # 应用层：业务逻辑
│   ├── main.c           # 主入口（52行，精简调度）
│   ├── app_car.h/c      # 小车业务逻辑
│   ├── app_test.h/c     # 统一测试框架
│   └── test/            # 旧测试文件（参考用）
│
├── build/               # 构建输出目录（自动生成）
├── .vscode/             # VSCode 配置
│   ├── c_cpp_properties.json
│   ├── launch.json
│   ├── tasks.json
│   └── settings.json
│
├── CMakeLists.txt       # CMake 构建配置
├── CMakePresets.json    # CMake 预设配置
├── arm-none-eabi.cmake  # ARM 工具链配置
├── STM32F103C8T6_FLASH.ld  # 链接脚本
├── openocd-daplink.cfg  # OpenOCD 调试配置
├── build.bat            # 一键构建脚本
├── flash.bat            # 一键烧录脚本
├── README.md            # 项目说明文档
└── DEVELOPMENT_LOG.md   # 开发日志
```

---

## 🚀 快速开始

### 环境准备

#### 必需工具
1. **编译器**: [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm) (v15.2.1+)
2. **构建工具**: [CMake](https://cmake.org/download/) (v3.16+)
3. **烧录工具**: [OpenOCD](http://openocd.org/) (v0.12.0+)
4. **调试器**: DAPLink 下载器
5. **IDE**: [VSCode](https://code.visualstudio.com/) + CMake Tools 插件

#### 可选工具
- 串口调试助手（如 PuTTY、SecureCRT）
- 蓝牙终端 APP（手机）

### 构建步骤

#### 方法 1：使用批处理脚本（推荐）

```bash
# 1. 构建项目
build.bat

# 2. 烧录固件
flash.bat
```

#### 方法 2：使用 CMake 命令

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 配置项目
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake

# 3. 编译
cmake --build .

# 4. 烧录
cmake --build . --target flash
```

#### 方法 3：使用 VSCode

1. 安装 **CMake Tools** 插件
2. 按 `Ctrl+Shift+P` → 选择 "CMake: Configure"
3. 按 `F7` 或点击底部状态栏的 "Build" 按钮
4. 按 `F5` 开始调试（需预先配置 DAPLink）

### 验证烧录

烧录成功后，观察以下现象：
1. ✅ OLED 屏幕显示 "Car System Ready"
2. ✅ LED 闪烁一次
3. ✅ 蓝牙发送启动信息："=== Car System Started ==="

---

## ⚙️ 功能特性

### 1. 工作模式

系统支持 5 种工作模式，可通过蓝牙命令切换：

| 模式 | 说明 | 用途 |
|------|------|------|
| **STOP** | 停止模式 | 电机停转，安全状态 |
| **MANUAL** | 手动控制 | 通过蓝牙命令控制电机速度 |
| **TRACK** | 循迹模式 | 自动沿黑线行驶 |
| **AVOIDANCE** | 避障模式 | 预留（可扩展超声波传感器） |
| **TEST** | 测试模式 | 硬件功能测试 |

### 2. 循迹算法

采用 **加权平均法 + PID 控制**：

```
传感器布局：  [L2] [L1] [M] [R1] [R2]
权重分配：    -4   -2    0   +2   +4

误差计算： error = Σ(传感器值 × 权重)
PID 输出： output = Kp×e + Ki×Σe + Kd×Δe
差速转向： left_speed  = base + output
           right_speed = base - output
```

### 3. PID 控制

- **算法类型**: 位置式 PID
- **可调参数**: Kp, Ki, Kd（通过蓝牙动态调整）
- **应用场景**: 循迹纠偏、速度控制

### 4. 测试框架

提供统一的硬件测试管理，支持：
- LED 闪烁测试
- OLED 显示测试
- 电机正反转测试
- 循迹传感器读取
- 蓝牙通信测试
- PID 算法测试
- 自动化测试序列

---

## 📱 蓝牙命令手册

### 连接方式

1. 手机下载蓝牙终端 APP（如 "蓝牙串口"）
2. 搜索并连接设备 "JDY-31"
3. 默认密码：**1234** 或 **0000**
4. 波特率：**9600**

### 命令格式

所有命令以换行符（`\r\n`）结尾，不区分大小写。

#### 基础命令

| 命令 | 示例 | 说明 | 响应 |
|------|------|------|------|
| `HELP` | `HELP` | 显示帮助信息 | 命令列表 |
| `STATUS` | `STATUS` | 查看设备完整状态 | 电机、传感器、PID 信息 |
| `SYSTEM_INFO` | `SYSTEM_INFO` | 查看系统信息 | 芯片、时钟、内存信息 |

#### 电机控制

| 命令 | 示例 | 说明 |
|------|------|------|
| `MOTOR,<A/B>,<F/B>,<speed>` | `MOTOR,A,F,50` | 电机 A 正转，速度 50% |
| `MOTOR,<A/B>,<F/B>,<speed>` | `MOTOR,B,B,80` | 电机 B 反转，速度 80% |

**参数说明：**
- `A/B`: 电机 A（左）或电机 B（右）
- `F/B`: 方向 F（前进）或 B（后退）
- `speed`: 速度 0-100（百分比）

#### LED 控制

| 命令 | 示例 | 说明 |
|------|------|------|
| `LED,ON` | `LED,ON` | 点亮 LED |
| `LED,OFF` | `LED,OFF` | 熄灭 LED |

#### PID 参数调整

| 命令 | 示例 | 说明 |
|------|------|------|
| `PID,<Kp>,<Ki>,<Kd>` | `PID,1.5,0.1,0.5` | 设置 PID 参数 |

**调参建议：**
- **Kp**: 比例系数，影响响应速度（典型值 0.5-3.0）
- **Ki**: 积分系数，消除稳态误差（典型值 0-0.5）
- **Kd**: 微分系数，抑制超调（典型值 0-1.0）

#### 状态查询

| 命令 | 示例 | 返回信息 |
|------|------|----------|
| `MOTOR_STATUS` | `MOTOR_STATUS` | 左右电机速度和方向 |
| `TRACK_STATUS` | `TRACK_STATUS` | 5路传感器状态 |
| `STATUS` | `STATUS` | 完整设备状态 |

#### 测试命令

| 命令 | 示例 | 说明 |
|------|------|------|
| `[TEST,LED]` | `[TEST,LED]` | 启动 LED 测试 |
| `[TEST,OLED]` | `[TEST,OLED]` | 启动 OLED 测试 |
| `[TEST,L298N]` | `[TEST,L298N]` | 启动电机测试 |
| `[TEST,TRACK]` | `[TEST,TRACK]` | 启动循迹测试 |
| `[TEST,BT]` | `[TEST,BT]` | 启动蓝牙测试 |
| `[TEST,PID]` | `[TEST,PID]` | 启动 PID 测试 |
| `[TEST,ALL]` | `[TEST,ALL]` | 执行所有测试（自动序列） |
| `[TEST,STOP]` | `[TEST,STOP]` | 停止当前测试 |

**注意：** 测试命令使用方括号格式，超时时间为 30 秒。

### 使用示例

```bash
# 1. 查看帮助
HELP

# 2. 控制电机
MOTOR,A,F,50
MOTOR,B,F,50

# 3. 调整 PID
PID,2.0,0.2,0.8

# 4. 查看状态
STATUS

# 5. 启动循迹测试
[TEST,TRACK]

# 6. 停止测试
[TEST,STOP]
```

---

## 💻 开发指南

### 添加新功能

#### 示例：添加避障模式

**步骤 1：在 `app_car.h` 中添加模式枚举**

```c
typedef enum {
    CAR_MODE_STOP = 0,
    CAR_MODE_MANUAL,
    CAR_MODE_TRACK,
    CAR_MODE_AVOIDANCE,  // 新增
    CAR_MODE_TEST
} CarMode_TypeDef;
```

**步骤 2：在 `app_car.c` 中实现控制逻辑**

```c
void Car_Run(void)
{
    switch (CarState.mode)
    {
        case CAR_MODE_AVOIDANCE:
            Car_AvoidanceControl();  // 新增函数
            break;
        // ... 其他模式
    }
}

void Car_AvoidanceControl(void)
{
    // 读取超声波传感器
    // 判断障碍物距离
    // 控制电机转向或停止
}
```

**步骤 3：在 `main.c` 中调用**

无需修改，`Car_Run()` 会自动处理新模式。

### 修改硬件引脚

编辑 [`Board_Config.h`](file://d:\qjn\stm32f103c8t6\car\3_BSP\Board_Config.h)：

```c
// 修改 LED 引脚
#define LED_PORT    GPIOC
#define LED_PIN     GPIO_Pin_13

// 修改 OLED I2C 引脚
#define OLED_I2C_PORT   GPIOB
#define OLED_SCL_PIN    GPIO_Pin_13
#define OLED_SDA_PIN    GPIO_Pin_12
```

### 调试技巧

#### 1. 使用 OLED 显示调试信息

```c
#include "OLED.h"

OLED_Clear();
OLED_ShowString(0, 0, "Debug:", OLED_6X8);
OLED_ShowNum(0, 8, variable, 4, OLED_6X8);
OLED_Update();
```

#### 2. 使用蓝牙发送调试数据

```c
#include "BT.h"

BT_SendString("Variable value: ");
char buf[20];
sprintf(buf, "%d\r\n", variable);
BT_SendString(buf);
```

#### 3. 使用 SysTick 测量代码执行时间

```c
#include "bsp_SysTick.h"

uint32_t start = SysTick_GetMicros();
// ... 待测代码 ...
uint32_t elapsed = SysTick_GetMicros() - start;
```

#### 4. VSCode 断点调试

1. 在代码行号左侧点击设置断点
2. 按 `F5` 启动调试
3. 查看变量、寄存器、调用栈

---

## ❓ 常见问题

### Q1: 编译报错 "undefined reference to xxx"

**原因：** 缺少源文件或链接错误

**解决：**
1. 检查 `CMakeLists.txt` 是否包含新添加的 `.c` 文件
2. 清理构建目录：删除 `build/` 文件夹后重新构建
3. 检查函数是否在头文件中正确声明

### Q2: OLED 屏幕无显示

**可能原因：**
1. I2C 接线错误（SCL/SBA 接反）
2. OLED 地址不正确（默认 0x78）
3. 忘记调用 `OLED_Update()`

**排查步骤：**
```c
// 1. 检查初始化
OLED_Init();

// 2. 测试显示
OLED_ShowString(0, 0, "Test", OLED_6X8);
OLED_Update();  // 必须调用！

// 3. 检查 I2C 波形（如有示波器）
```

### Q3: 蓝牙无法连接

**检查清单：**
- ✅ 蓝牙模块供电正常（3.3V 或 5V）
- ✅ TX/RX 交叉连接（MCU_TX → BT_RX, MCU_RX → BT_TX）
- ✅ 波特率匹配（9600）
- ✅ 蓝牙模块未进入 AT 命令模式

### Q4: 电机不转或转动异常

**排查步骤：**
1. 检查 L298N 供电（7-12V）
2. 检查使能端（ENA/ENB）是否有 PWM 信号
3. 检查 IN1-IN4 电平是否正确
4. 使用测试命令验证：`[TEST,L298N]`

### Q5: 循迹效果不佳

**调参建议：**
1. **增大 Kp**：提高响应速度（但过大会震荡）
2. **增加 Kd**：抑制超调和震荡
3. **减小 Ki**：避免积分饱和（通常设为 0）
4. **检查传感器**：确保 5 路传感器都能检测到黑线

**示例调参过程：**
```bash
# 初始参数
PID,1.0,0.0,0.5

# 如果反应慢，增大 Kp
PID,2.0,0.0,0.5

# 如果有震荡，增大 Kd
PID,2.0,0.0,1.0

# 如果有稳态误差，增加 Ki
PID,2.0,0.1,1.0
```

### Q6: 烧录失败

**常见错误及解决：**

| 错误信息 | 原因 | 解决方法 |
|---------|------|---------|
| "Can't find DAPLink" | 驱动未安装或 USB 线问题 | 安装 CMSIS-DAP 驱动，更换 USB 线 |
| "Target not found" | 接线错误或目标板断电 | 检查 SWDIO/SWCLK/GND 连接 |
| "Flash write failed" | Flash 被保护 | 解锁 Flash 或擦除全片 |

---

## 📊 性能指标

| 指标 | 数值 | 说明 |
|------|------|------|
| **主频** | 72 MHz | PLL 倍频后 |
| **SysTick 周期** | 1 ms | 系统心跳中断 |
| **主循环频率** | ~100 Hz | 10ms 周期 |
| **OLED 刷新率** | 10 Hz | 每 100ms 更新 |
| **蓝牙波特率** | 9600 bps | USART2 |
| **PWM 频率** | ~20 kHz | TIM2/TIM3 |
| **PID 计算周期** | 10 ms | 可配置 |

---

## 🛠️ 工具链版本

| 工具 | 版本 | 下载地址 |
|------|------|---------|
| GCC ARM | 15.2.1 | [Arm Developer](https://developer.arm.com/downloads/-/gnu-rm) |
| CMake | 3.16+ | [CMake官网](https://cmake.org/download/) |
| OpenOCD | 0.12.0 | [OpenOCD官网](http://openocd.org/) |
| Ninja | 1.10+ | [Ninja-build](https://ninja-build.org/) |

---

## 📝 开发日志

详细的项目演进记录请查看：[DEVELOPMENT_LOG.md](DEVELOPMENT_LOG.md)

**最近更新时间：** 2026-05-13  
**主要更新：**
- ✅ 完成架构重构（方案 B）
- ✅ 创建应用层框架（app_car/app_test）
- ✅ 添加 SysTick 驱动模块
- ✅ main.c 从 176 行精简至 52 行

---

## 📄 许可证

本项目仅供学习和研究使用。

---

## 👥 贡献

欢迎提交 Issue 和 Pull Request！

---

**芯片**: STM32F103C8T6  
**主频**: 72MHz  
**Flash**: 64KB  
**RAM**: 20KB  
**架构**: Platform → HAL → BSP → App  

---

*最后更新: 2026-05-13*
