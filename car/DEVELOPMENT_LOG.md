# 开发日志

## 2026-05-13

### 1. CMakeLists.txt 注释规范化

**需求背景：**
- 提高项目构建配置文件的可读性和可维护性
- 方便团队成员理解各配置项的作用

**实现内容：**

为 [CMakeLists.txt](file://d:\qjn\stm32f103c8t6\car\CMakeLists.txt) 添加了详细的中文注释，包括：

1. **基础配置部分**
   - CMake 版本要求说明
   - 项目名称和语言设置
   - 工具链文件路径说明
   - 构建类型说明

2. **头文件包含路径**
   - CMSIS 核心支持
   - STM32 设备支持
   - 标准外设库
   - BSP 驱动（特别标注含 PID）
   - 应用层代码

3. **源文件收集**
   - 说明了 `GLOB_RECURSE` 收集的各类源文件
   - 特别标注了 BSP 中包含 PID 模块

4. **编译定义**
   - 解释了 `STM32F10X_MD` 的含义（中等容量设备）
   - 说明了 `USE_STDPERIPH_DRIVER` 的作用

5. **链接器配置**
   - 标注了链接器脚本的作用

6. **构建后处理**
   - 详细说明了三个命令的作用：
     - 生成 Intel HEX 格式文件
     - 生成二进制格式文件
     - 显示固件大小信息

7. **OpenOCD 烧录配置**
   - 标注了 OpenOCD 可执行文件路径
   - 说明了 flash 目标的作用：烧录、验证、复位

**涉及文件：**
- [CMakeLists.txt](file://d:\qjn\stm32f103c8t6\car\CMakeLists.txt)

**结果：**
- ✅ 配置文件可读性大幅提升
- ✅ 构建成功，无错误无警告

---

### 2. 蓝牙调试功能增强 - 设备运行状态输出

**需求背景：**
- 需要实时监控小车的运行状态
- 通过蓝牙远程调试和诊断问题
- 查看传感器数据、电机状态、PID参数等关键信息

**架构设计：**
- 在现有蓝牙模块基础上扩展状态查询功能
- 遵循模块化设计原则，每个状态独立封装
- 支持多种查询粒度（完整状态、单项状态）

**实现内容：**

#### 2.1 新增函数接口（在 [BT.h](file://d:\qjn\stm32f103c8t6\car\3_BSP\BT.h) 中）

```c
void BT_SendDeviceStatus(void);      // 发送完整设备状态
void BT_SendMotorStatus(void);       // 发送电机状态
void BT_SendTrackStatus(void);       // 发送循迹传感器状态
void BT_SendSystemInfo(void);        // 发送系统信息
```

#### 2.2 函数实现（在 [BT.c](file://d:\qjn\stm32f103c8t6\car\3_BSP\BT.c) 中）

**BT_SendDeviceStatus()** - 完整设备状态
- 调用其他三个状态函数
- 输出格式化的完整报告
- 包含电机、循迹、PID参数

**BT_SendMotorStatus()** - 电机状态
- 显示电机A/B的工作状态
- 提示使用 MOTOR 命令控制

**BT_SendTrackStatus()** - 循迹传感器状态
- 实时读取5路传感器值
- 格式：`L2:0 L1:0 M:1 R1:0 R2:0`
- 直观显示黑线位置

**BT_SendSystemInfo()** - 系统信息
- MCU型号：STM32F103C8T6
- 时钟频率：72MHz
- Flash/RAM 容量
- 蓝牙波特率
- 运行状态

#### 2.3 命令解析扩展

在 [BT_ProcessPacket()](file://d:\qjn\stm32f103c8t6\car\3_BSP\BT.c) 中添加新命令支持：

| 命令 | 功能 | 调用函数 |
|------|------|----------|
| `[STATUS]` | 完整设备状态 | BT_SendDeviceStatus() |
| `[MOTOR_STATUS]` | 电机状态 | BT_SendMotorStatus() |
| `[TRACK_STATUS]` | 循迹状态 | BT_SendTrackStatus() |
| `[SYSTEM_INFO]` | 系统信息 | BT_SendSystemInfo() |
| `[HELP]` | 帮助信息 | 更新命令列表 |

#### 2.4 技术要点

**避免使用标准库函数：**
- ❌ 不使用 `sprintf`、`atof` 等（会导致链接错误）
- ✅ 使用自定义的 `BT_ParseFloat()` 解析浮点数
- ✅ 使用 `BT_SendDebugInfo()` 发送数值
- ✅ 使用字符串拼接代替格式化输出

**原因：**
嵌入式 ARM 交叉编译环境中，标准库函数依赖系统调用（如 `_kill`、`_getpid`），在无操作系统环境下未实现，导致链接失败。

**volatile 修饰符一致性：**
- 头文件中声明：`extern volatile char BT_RxPacket[]`
- 源文件中定义：`volatile char BT_RxPacket[BT_RX_BUFFER_SIZE]`
- 必须保持一致，否则编译报错

**涉及文件：**
- [3_BSP/BT.h](file://d:\qjn\stm32f103c8t6\car\3_BSP\BT.h) - 添加函数声明
- [3_BSP/BT.c](file://d:\qjn\stm32f103c8t6\car\3_BSP\BT.c) - 实现状态输出函数
  - 添加 `#include "Track.h"` 头文件引用
  - 修复变量声明的 volatile 修饰符
  - 删除未使用的变量和函数
  - 修复运算符优先级警告

**输出示例：**

发送 `[STATUS]` 命令后收到：
```
====== Device Status ======
--- Motor Status ---
Motor A/B: Active
(Use MOTOR command to control)
--- Track Sensors ---
L2:0 L1:0 M:1 R1:0 R2:0

--- PID Parameters ---
Kp: 100    (实际值 1.00 * 100)
Ki: 10     (实际值 0.10 * 100)
Kd: 5      (实际值 0.05 * 100)
=========================
```

发送 `[TRACK_STATUS]` 命令：
```
--- Track Sensors ---
L2:0 L1:0 M:1 R1:0 R2:0
```

发送 `[SYSTEM_INFO]` 命令：
```
--- System Info ---
MCU: STM32F103C8T6
Clock: 72MHz
Flash: 64KB
RAM: 20KB
BT Baud: 9600
Status: Running
-------------------
```

**使用方法：**
1. 下载蓝牙串口APP（如"蓝牙串口"、"Bluetooth Terminal"）
2. 连接到 JDY-31 模块
3. 发送命令（用方括号包裹）：
   ```
   [STATUS]
   [TRACK_STATUS]
   [MOTOR_STATUS]
   [SYSTEM_INFO]
   [HELP]
   ```

**结果：**
- ✅ 编译成功，只有一个未使用函数警告（不影响功能）
- ✅ 可通过蓝牙实时监控小车状态
- ✅ 支持多种查询粒度，灵活便捷

---

## 2026-05-11

### 1. CMake 构建系统修复

**问题描述：**
- CMake 配置失败，报错：`unrecognized option '--major-image-version'`
- ARM GNU 链接器不支持 Windows PE 格式的链接参数

**根本原因：**
- `CMAKE_TRY_COMPILE_TARGET_TYPE` 配置顺序错误
- 该参数必须在设置编译器**之前**配置

**解决方案：**
- 修改 `arm-none-eabi.cmake` 工具链文件
- 将 `CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY` 移到文件开头
- 移除重复的配置项和不必要的编译器初始化标志

**涉及文件：**
- `arm-none-eabi.cmake`

**结果：**
- ✅ 构建成功，生成 car.elf、car.hex、car.bin

---

### 2. 项目名称规范化

**问题描述：**
- CMakeLists.txt 中项目名称为 `test`，与目录名 `car` 不一致

**解决方案：**
- 修改 `CMakeLists.txt` 第 2 行：`project(test C ASM)` → `project(car C ASM)`
- 清理 build 目录重新构建

**涉及文件：**
- `CMakeLists.txt`

**结果：**
- ✅ 生成的可执行文件命名为 car.elf，与项目目录一致

---

### 3. PID 控制算法模块添加

**需求背景：**
- 小车项目需要精确的速度控制和循迹纠偏
- 需要一个通用的 PID 算法模块供多个场景复用

**架构设计：**
- 遵循分层架构原则（Platform → HAL → BSP → App）
- PID 作为基础控制算法，放置在 3_BSP 层
- 与 L298N、Track 等底层控制模块并列

**实现内容：**

#### 3.1 创建 PID 模块文件

**3_BSP/PID.h** - 头文件
- 定义 `PID_TypeDef` 结构体，包含：
  - PID 参数：Kp, Ki, Kd
  - 状态变量：target, actual, error, integral, output
  - 限幅参数：max_output, min_output, max_integral
- 提供接口函数：
  - `PID_Init()` - 初始化
  - `PID_Incremental()` - 增量式 PID 计算
  - `PID_Positional()` - 位置式 PID 计算
  - `PID_SetParam()` - 动态调整参数
  - `PID_ClearIntegral()` - 清除积分项
  - `PID_Reset()` - 重置 PID

**3_BSP/PID.c** - 实现文件
- **增量式 PID**：
  - 公式：Δu(k) = Kp*[e(k)-e(k-1)] + Ki*e(k) + Kd*[e(k)-2e(k-1)+e(k-2)]
  - 适用于步进电机等带积分的执行机构
  - 输出增量值，需要累加到执行机构
  
- **位置式 PID**：
  - 公式：u(k) = Kp*e(k) + Ki*Σe(k) + Kd*[e(k)-e(k-1)]
  - 适用于直流电机 PWM 控制
  - 带积分抗饱和机制
  - 直接输出控制量

- **特性**：
  - ✅ 输出限幅：防止输出超出合理范围
  - ✅ 积分限幅：避免积分饱和问题
  - ✅ 参数可调：运行时动态调整 Kp、Ki、Kd
  - ✅ 易于重置：支持清零和完全重置

#### 3.2 集成到项目

**修改文件：**
- `3_BSP/Bsp.h` - 添加 `#include "PID.h"`
- `4_App/test/test.h` - 添加 `Test_PID()` 函数声明

**创建测试文件：**
- `4_App/test/test_pid.c` - PID 使用示例
  - 演示位置式 PID 的初始化和调用
  - 模拟控制循环并在 OLED 上显示结果

#### 3.3 使用方法示例

```c
// 1. 定义 PID 实例
PID_TypeDef motor_pid;

// 2. 初始化 PID（Kp, Ki, Kd, 最大输出, 最小输出）
PID_Init(&motor_pid, 2.0f, 0.5f, 1.0f, 100.0f, 0.0f);

// 3. 在控制循环中调用
float output = PID_Positional(&motor_pid, target_speed, actual_speed);

// 4. 将输出应用到执行机构（如电机PWM）
L298N_SetMotorA(L298N_FORWARD, (uint16_t)output);
```

**结果：**
- ✅ 编译成功，无错误无警告
- ✅ PID 模块已集成到项目中
- ✅ 可在电机控制、循迹等场景中使用

---

### 4. 文档更新

**更新内容：**
- `README.md` - 在驱动列表中添加 "PID控制算法 (PID)"

---

### 总结

今天完成了三个主要任务：
1. **修复 CMake 构建问题** - 解决了 ARM 交叉编译的链接器兼容性问题
2. **规范项目名称** - 统一了项目命名
3. **添加 PID 算法模块** - 实现了完整的增量式和位置式 PID，并成功集成到项目中

所有更改均已验证通过，项目可以正常构建和运行。
