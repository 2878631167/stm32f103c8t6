# 状态机管理模块使用指南

## 概述

本项目引入了一个通用的状态机管理模块（StateMachine），用于统一管理整个系统的状态流转。该模块提供了清晰的状态转换机制，使系统行为更加可预测和易于维护。

## 架构设计

### 核心组件

1. **状态机管理器** (`StateMachine.c/h`) - 提供统一的状态机管理接口
2. **状态定义** - 定义了系统中所有可能的状态
3. **事件处理** - 通过事件驱动的方式触发状态转换
4. **回调函数** - 每个状态可以定义进入、处理和退出时的回调函数

### 状态图

```
[INIT] → [IDLE] ↔ [MANUAL_CTRL] ↔ [AUTO_TRACK] ↔ [AUTO_AVOID] ↔ [TEST]
   ↓        ↓           ↓              ↓             ↓          ↓
[SHUTDOWN] [ERROR] ←---+--------------+-------------+----------+
```

## 状态定义

| 状态ID | 名称 | 描述 |
|--------|------|------|
| `STATE_ID_INIT` | INIT | 初始化状态 |
| `STATE_ID_IDLE` | IDLE | 空闲状态 |
| `STATE_ID_MANUAL_CTRL` | MANUAL_CTRL | 手动控制状态 |
| `STATE_ID_AUTO_TRACK` | AUTO_TRACK | 自动循迹状态 |
| `STATE_ID_AUTO_AVOID` | AUTO_AVOID | 自动避障状态 |
| `STATE_ID_TEST` | TEST | 测试状态 |
| `STATE_ID_ERROR` | ERROR | 错误状态 |
| `STATE_ID_SHUTDOWN` | SHUTDOWN | 关机状态 |

## 事件定义

| 事件ID | 名称 | 描述 |
|--------|------|------|
| `EVENT_NONE` | 无事件 | 无特殊事件 |
| `EVENT_START` | 启动事件 | 启动状态机 |
| `EVENT_STOP` | 停止事件 | 停止当前操作 |
| `EVENT_MANUAL_CTRL` | 手动控制事件 | 切换到手动控制 |
| `EVENT_AUTO_TRACK` | 自动循迹事件 | 切换到自动循迹 |
| `EVENT_AUTO_AVOID` | 自动避障事件 | 切换到自动避障 |
| `EVENT_TEST_MODE` | 测试模式事件 | 切换到测试模式 |
| `EVENT_ERROR` | 错误事件 | 发生错误 |
| `EVENT_RETURN_IDLE` | 返回空闲事件 | 返回空闲状态 |
| `EVENT_SHUTDOWN` | 关闭系统事件 | 关闭系统 |

## API 接口

### 初始化

```c
// 初始化状态机
void SM_Init(StateMachine_TypeDef* sm);

// 注册状态到状态机
int8_t SM_RegisterState(StateMachine_TypeDef* sm, StateDef_TypeDef* state);

// 系统级状态机初始化
void SystemStateMachine_Init(void);
```

### 状态控制

```c
// 触发状态机事件
int8_t SM_TriggerEvent(StateMachine_TypeDef* sm, StateEvent_TypeDef event);

// 获取当前状态
StateID_TypeDef SM_GetCurrentState(StateMachine_TypeDef* sm);

// 运行状态机
void SM_Run(StateMachine_TypeDef* sm);

// 获取状态名称
const char* SM_GetStateName(StateID_TypeDef state_id);
```

### 使用示例

```c
// 在主循环中运行状态机
while(1)
{
    // 处理蓝牙命令
    if (BT_RxFlag == 1)
    {
        BT_ProcessPacket();
        Test_HandleCommand();
        
        // 根据蓝牙命令触发状态机事件
        // 例如: SM_TriggerEvent(System_GetStateMachine(), EVENT_MANUAL_CTRL);
    }
    
    // 运行状态机
    SM_Run(System_GetStateMachine());
    
    // 运行应用逻辑
    Car_Run();
    
    // 更新显示
    Car_UpdateDisplay();
    
    // 延时
    Delay_Ms(10);
}
```

## 状态处理函数

每个状态都可以定义三个回调函数：

1. **entry_func** - 进入状态时调用
2. **handler_func** - 状态处理函数，每帧调用
3. **exit_func** - 退出状态时调用

```c
typedef struct {
    StateID_TypeDef id;                           // 状态ID
    const char* name;                             // 状态名称
    StateHandlerFunc entry_func;                  // 状态进入函数
    StateHandlerFunc handler_func;                // 状态处理函数
    StateHandlerFunc exit_func;                   // 状态退出函数
} StateDef_TypeDef;
```

## 集成说明

### 与现有代码的集成

1. **main.c** - 已集成状态机初始化和运行
2. **app_car.c** - 已修改为基于状态机状态执行相应逻辑
3. **app_test.c** - 已集成状态机事件处理

### 扩展新状态

要添加新的状态，只需：

1. 在 `StateID_TypeDef` 枚举中添加新的状态ID
2. 创建对应的状态处理函数
3. 在 `SystemStateMachine_Init()` 中注册新状态

## 优势

1. **清晰的架构** - 状态转换逻辑清晰可见
2. **易于维护** - 状态和行为分离，便于调试和修改
3. **可扩展性强** - 可以轻松添加新的状态和事件
4. **解耦合** - 各模块通过状态机进行协调，降低耦合度
5. **错误处理** - 统一的错误状态处理机制

## 注意事项

1. 确保在状态处理函数中不要阻塞主循环
2. 状态转换时要考虑资源清理和初始化
3. 避免状态间的循环依赖
4. 在嵌入式环境中注意内存使用

## 故障排除

如果状态机工作异常，请检查：

1. 状态是否正确注册
2. 事件是否正确触发
3. 状态处理函数是否有返回值错误
4. 内存是否充足