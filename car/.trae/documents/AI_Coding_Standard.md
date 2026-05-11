# AI 代码编写规范 - STM32F103C8T6 项目

## 1. 项目目录结构

```
Template_Lite/
├── 1_Platform/              # 平台层 - CMSIS 核心文件
│   └── CMSIS/CM3/
│       ├── CoreSupport/     # cortex-m3 核心支持
│       └── DeviceSupport/ST/STM32F10x/
│           ├── startup/arm/ # 启动文件
│           ├── stm32f10x.h  # 芯片定义头文件
│           ├── system_stm32f10x.c/.h  # 系统时钟配置
├── 2_HAL/                   # 硬件抽象层 - 标准库
│   └── STM32F10x_StdPeriph_Driver/
│       ├── inc/             # 头文件
│       └── src/             # 源文件
├── 3_BSP/                  # 板级支持包 - 驱动层
│   ├── bsp_led.c/.h        # LED驱动
│   ├── bsp_clk.c/.h        # 系统时钟驱动
│   ├── bsp_delay.c/.h     # 延时驱动
│   ├── OLED.c/.h           # OLED驱动
│   ├── BT.c/.h             # 蓝牙驱动
│   ├── Board_Config.h     # 引脚配置
│   └── Bsp.h              # BSP总头文件
├── 4_App/                  # 应用层
│   ├── main.c             # 主程序
│   ├── stm32f10x_conf.h   # 标准库配置
│   └── test/              # 测试代码
│       ├── test_led.c     # LED测试
│       ├── test_oled.c    # OLED测试
│       ├── test_bt.c      # 蓝牙测试
│       └── test_l298n.c   # 电机测试
├── build/                  # 构建输出目录
├── arm-none-eabi.cmake    # 交叉编译工具链
├── CMakeLists.txt         # CMake构建配置
├── build.bat              # 构建脚本
├── flash.bat              # 烧录脚本
└── openocd-daplink.cfg   # OpenOCD配置
```

## 2. 命名规范

### 2.1 文件命名
- **驱动文件**: `模块名.c` / `模块名.h`
- 示例: `bsp_led.c`, `BT.c`, `OLED.h`

### 2.2 函数命名
- **驱动函数**: `模块名_功能()`
- 示例: `LED_Init()`, `BT_Init()`, `OLED_ShowString()`

### 2.3 变量命名
- **全局变量**: `模块名_变量名`
- 示例: `BT_RxFlag`, `BT_RxPacket`
- **局部变量**: 小驼峰命名
- 示例: `uint8_t rxData`, `char buffer[32]`

### 2.4 宏/常量命名
- **引脚配置**: `模块名_功能_PORT/PIN/CLK`
- 示例: `OLED_SCL_PORT`, `BT_TX_PIN`
- **配置参数**: 全大写下划线分隔
- 示例: `BT_RX_BUFFER_SIZE`, `USART_WordLength_8b`

## 3. 头文件规范

### 3.1 头文件保护
```c
#ifndef __MODULE_NAME_H
#define __MODULE_NAME_H

// 内容

#endif
```

### 3.2 头文件包含顺序
1. 模块自身头文件
2. 标准库头文件 (stm32f10x.h)
3. 标准外设头文件 (stm32f10x_xxx.h)
4. 项目内部头文件

```c
#include "BT.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
```

### 3.3 统一头文件 (Bsp.h)
- 所有模块的公共头文件
- 包含标准库头文件和所有驱动头文件
- 应用层只需包含 `Bsp.h`

## 4. 引脚配置规范

### 4.1 Board_Config.h
所有引脚配置集中管理:
```c
#define LED_GPIO_PORT     GPIOC
#define LED_GPIO_PIN      GPIO_Pin_13
#define LED_GPIO_CLK      RCC_APB2Periph_GPIOC

#define OLED_SCL_PORT     GPIOB
#define OLED_SCL_PIN      GPIO_Pin_13

#define BT_TX_PORT        GPIOB
#define BT_TX_PIN         GPIO_Pin_10
```

### 4.2 引脚分配原则
- LED: PC13 (板载LED)
- OLED: PB13 (SCL), PB12 (SDA)
- 蓝牙: PB10 (TX), PB11 (RX) - USART3

## 5. 驱动编写规范

### 5.1 驱动结构
```c
// xxx.h
#ifndef __XXX_H
#define __XXX_H

#include <stdint.h>

// 功能函数声明
void XXX_Init(void);
void XXX_SendByte(uint8_t Byte);

#endif

// xxx.c
#include "xxx.h"
#include "stm32f10x.h"

void XXX_Init(void)
{
    // 1. 开启时钟
    // 2. 配置GPIO
    // 3. 配置外设
    // 4. 如果需要中断，配置NVIC
}
```

### 5.2 中断处理规范
- 中断函数命名: `外设_IRQHandler()`
- 中断中使用全局变量需声明 `volatile`
- 示例:
```c
volatile char BT_RxPacket[BT_RX_BUFFER_SIZE];
volatile uint8_t BT_RxFlag = 0;

void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        // 处理接收
    }
}
```

### 5.3 外设配置顺序
1. 开启时钟 (RCC)
2. 配置GPIO
3. 配置外设参数
4. 使能外设
5. (可选) 配置NVIC中断

## 6. 应用层编写规范

### 6.1 主程序结构
```c
#include "Bsp.h"

int main(void)
{
    // 1. 系统初始化
    SystemClock_Config();

    // 2. 外设初始化
    LED_Init();
    OLED_Init();
    BT_Init();

    // 3. 主循环
    while(1)
    {
        // 业务逻辑
        // 延时
    }
}
```

### 6.2 初始化顺序
1. 系统时钟配置
2. 各外设初始化
3. OLED显示初始状态

## 7. 构建规范

### 7.1 构建工具
- 构建系统: CMake + Ninja
- 工具链: arm-none-eabi-gcc
- 烧录工具: OpenOCD + DAPLink

### 7.2 构建命令
```bash
# 构建
build.bat

# 烧录
flash.bat
```

### 7.3 CMakeLists.txt 结构
- 工具链配置
- 头文件路径
- 源文件 glob
- 编译选项
- 链接配置
- 后处理命令 (生成hex/bin)

## 8. VSCode 配置

### 8.1 c_cpp_properties.json
- 包含路径
- 宏定义
- 编译器路径

### 8.2 settings.json
- CMake路径
- 生成器配置
- 工具链配置

## 9. 注意事项

### 9.1 PC13 引脚限制
- 只能工作在2MHz模式下
- 配置为低速推挽输出

### 9.2 USART 引脚映射
- TX引脚配置为复用推挽输出
- RX引脚配置为输入上拉

### 9.3 中断优先级
- 合理分配抢占优先级和子优先级
- 避免优先级冲突

### 9.4 波特率匹配
- 确保模块波特率与代码配置一致
- JDY-31默认9600

## 10. 开发日志规范

### 10.1 日志文件位置
- 路径: `.trae/documents/开发日志.md`
- 使用 Markdown 格式

### 10.2 记录内容
每次添加新功能或修复问题时记录:
- **日期**: 格式 YYYY-MM-DD
- **功能实现**: 新增的驱动或功能
- **文件变更**: 修改的文件列表
- **问题修复**: 遇到的问题及解决方案

### 10.3 日志格式
```markdown
## 2026-05-07

### 功能实现
- 新增 LED 驱动 (PC13)
  - 文件: 3_BSP/bsp_led.c, 3_BSP/bsp_led.h
  - 功能: LED初始化和闪烁

### 问题修复
- 修复蓝牙引脚配置错误
  - 原因: TX/RX引脚反了
  - 解决: 调整PB10/11对应关系
```

### 10.4 更新时机
- 添加新驱动模块时
- 修复重大问题时
- 项目配置变更时

## 11. 测试目录规范

### 11.1 目录位置
- 路径: `4_App/test/`
- 每个驱动模块对应一个测试文件
- 需包含 `test.h` 统一头文件

### 11.2 测试文件清单
```
4_App/test/
├── test.h          # 测试函数统一声明头文件
├── test_led.c      # LED驱动测试
├── test_oled.c    # OLED显示测试
├── test_bt.c       # 蓝牙通信测试
├── test_l298n.c   # 电机驱动测试
└── test_track.c   # 循迹传感器测试
```

### 11.3 测试文件命名
- 格式: `test_模块名.c`
- 示例: `test_led.c`, `test_oled.c`, `test_bt.c`, `test_l298n.c`, `test_track.c`

### 11.4 测试函数命名
- 格式: `Test_模块名_功能()`
- 示例: `Test_LED_On()`, `Test_OLED_ShowString()`, `Test_L298N_Forward()`

### 11.5 test.h 统一头文件
```c
/**
 * @file    test.h
 * @brief   测试函数头文件
 * @author  Author
 * @date    2026-05-08
 */

#ifndef __TEST_H
#define __TEST_H

// LED测试
void Test_LED_On(void);
void Test_LED_Off(void);
void Test_LED_Blink(void);

// OLED测试
void Test_OLED_Clear(void);
void Test_OLED_ShowString(void);
void Test_OLED_ShowNum(void);

// 蓝牙测试
void Test_BT_Init(void);
void Test_BT_SendString(void);

// 电机测试
void Test_L298N_Forward(void);
void Test_L298N_Backward(void);

// 循迹测试
void Test_Track_ReadOnce(void);
void Test_Track_Loop(void);

#endif
```

### 11.6 测试文件结构
```c
/**
 * @file    test_xxx.c
 * @brief   XXX驱动测试
 * @author  Author
 * @date    2026-05-08
 */

#include "Bsp.h"

void Test_XXX_Function1(void)
{
    XXX_Init();
    // 测试代码
}

void Test_XXX_Function2(void)
{
    XXX_Init();
    while(1)
    {
        // 循环测试
    }
}
```

### 11.7 测试运行方式
- 在 main.c 中注释掉主程序，临时调用测试函数
- 或在 main.c 中添加测试模式选择

## 12. 代码注释规范

### 12.1 注释原则
- 注释应解释"为什么"，而不是"是什么"
- 保持注释与代码同步更新
- 避免过度注释，明显的代码不需要注释

### 12.2 文件头部注释
每个 `.c` 文件开头应有功能说明:
```c
/**
 * @file    模块名.c
 * @brief   简要功能描述
 * @details 详细说明（可选）
 * @author  作者
 * @date    2026-01-01
 */
```

### 12.3 函数注释
公共函数需注释功能和参数:
```c
/**
 * @brief   功能描述
 * @param   参数名 参数说明
 * @return  返回值说明
 */
void Module_Function(uint8_t param);
```

### 12.4 行注释
- 使用 `//` 进行单行注释
- 注释放在代码上方或行尾
- 行尾注释需与代码保持至少一个空格

```c
// 开启时钟
RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

// 配置GPIO为推挽输出
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
```

### 12.5 模块头部注释（可选）
驱动模块可在头文件开头添加模块说明:
```c
/* ============================================
 * 模块名称: LED驱动
 * 功能: LED初始化和控制
 * 硬件: PC13
 * ============================================ */
```

### 12.6 TODO注释
未完成或待优化代码使用:
```c
// TODO: 优化延时函数精度
// FIXME: 修复某问题
```

---

**最后更新**: 2026-05-08
**适用芯片**: STM32F103C8T6
**标准库版本**: STM32F10x StdPeriph Lib V3.5.0