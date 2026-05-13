/**
 * @file    StateMachine.h
 * @brief   通用状态机管理模块接口定义
 * @details 提供统一的状态机管理接口，支持状态切换、事件处理等功能
 * @author  Auto-generated
 * @date    2026-05-13
 */

#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include "Bsp.h"

/**
 * @brief 状态ID枚举
 */
typedef enum {
    STATE_ID_INIT = 0,        /**< 初始化状态 */
    STATE_ID_IDLE,            /**< 空闲状态 */
    STATE_ID_MANUAL_CTRL,     /**< 手动控制状态 */
    STATE_ID_AUTO_TRACK,      /**< 自动循迹状态 */
    STATE_ID_AUTO_AVOID,      /**< 自动避障状态 */
    STATE_ID_TEST,            /**< 测试状态 */
    STATE_ID_ERROR,           /**< 错误状态 */
    STATE_ID_SHUTDOWN,        /**< 关机状态 */
    STATE_ID_MAX              /**< 状态总数 */
} StateID_TypeDef;

/**
 * @brief 状态机事件枚举
 */
typedef enum {
    EVENT_NONE = 0,           /**< 无事件 */
    EVENT_START,              /**< 启动事件 */
    EVENT_STOP,               /**< 停止事件 */
    EVENT_MANUAL_CTRL,        /**< 手动控制事件 */
    EVENT_AUTO_TRACK,         /**< 自动循迹事件 */
    EVENT_AUTO_AVOID,         /**< 自动避障事件 */
    EVENT_TEST_MODE,          /**< 测试模式事件 */
    EVENT_ERROR,              /**< 错误事件 */
    EVENT_RETURN_IDLE,        /**< 返回空闲事件 */
    EVENT_SHUTDOWN,           /**< 关闭系统事件 */
    EVENT_MAX                 /**< 事件总数 */
} StateEvent_TypeDef;

/**
 * @brief 状态机结构体定义
 */
typedef struct StateMachine StateMachine_TypeDef;

/**
 * @brief 状态处理函数指针类型
 * @param state_machine 状态机实例
 * @param event 触发的事件
 * @return 0表示成功，非0表示失败
 */
typedef int8_t (*StateHandlerFunc)(StateMachine_TypeDef* state_machine, StateEvent_TypeDef event);

/**
 * @brief 状态定义结构体
 */
typedef struct {
    StateID_TypeDef id;                           /**< 状态ID */
    const char* name;                             /**< 状态名称 */
    StateHandlerFunc entry_func;                  /**< 状态进入函数 */
    StateHandlerFunc handler_func;                /**< 状态处理函数 */
    StateHandlerFunc exit_func;                   /**< 状态退出函数 */
} StateDef_TypeDef;

/**
 * @brief 状态机实例结构体
 */
struct StateMachine {
    StateID_TypeDef current_state;                /**< 当前状态 */
    StateDef_TypeDef* states[STATE_ID_MAX];       /**< 状态定义数组 */
    uint8_t state_count;                          /**< 状态数量 */
    volatile uint8_t is_running;                  /**< 是否正在运行 */
};

/**
 * @brief 初始化状态机
 * @param sm 状态机实例指针
 */
void SM_Init(StateMachine_TypeDef* sm);

/**
 * @brief 注册状态到状态机
 * @param sm 状态机实例指针
 * @param state 状态定义
 * @return 0表示成功，-1表示失败
 */
int8_t SM_RegisterState(StateMachine_TypeDef* sm, StateDef_TypeDef* state);

/**
 * @brief 触发状态机事件
 * @param sm 状态机实例指针
 * @param event 事件类型
 * @return 0表示成功，非0表示失败
 */
int8_t SM_TriggerEvent(StateMachine_TypeDef* sm, StateEvent_TypeDef event);

/**
 * @brief 获取当前状态
 * @param sm 状态机实例指针
 * @return 当前状态ID
 */
StateID_TypeDef SM_GetCurrentState(StateMachine_TypeDef* sm);

/**
 * @brief 运行状态机
 * @param sm 状态机实例指针
 */
void SM_Run(StateMachine_TypeDef* sm);

/**
 * @brief 获取状态名称
 * @param state_id 状态ID
 * @return 状态名称字符串
 */
const char* SM_GetStateName(StateID_TypeDef state_id);

#endif /* __STATE_MACHINE_H */