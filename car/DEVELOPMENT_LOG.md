# 开发日志

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
