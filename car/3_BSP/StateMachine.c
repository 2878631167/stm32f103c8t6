/**
 * @file    StateMachine.c
 * @brief   通用状态机管理模块实现
 * @details 提供统一的状态机管理功能，支持状态切换、事件处理等
 * @author  Auto-generated
 * @date    2026-05-13
 */

#include "StateMachine.h"
#include <string.h>

/* 全局状态机实例 */
static StateMachine_TypeDef g_state_machine;

/**
 * @brief 初始化状态机
 * @param sm 状态机实例指针
 */
void SM_Init(StateMachine_TypeDef* sm)
{
    if (sm == NULL) return;
    
    memset(sm, 0, sizeof(StateMachine_TypeDef));
    sm->current_state = STATE_ID_INIT;
    sm->state_count = 0;
    sm->is_running = 0;
}

/**
 * @brief 注册状态到状态机
 * @param sm 状态机实例指针
 * @param state 状态定义
 * @return 0表示成功，-1表示失败
 */
int8_t SM_RegisterState(StateMachine_TypeDef* sm, StateDef_TypeDef* state)
{
    if (sm == NULL || state == NULL) return -1;
    if (sm->state_count >= STATE_ID_MAX) return -1;
    
    sm->states[state->id] = state;
    sm->state_count++;
    
    return 0;
}

/**
 * @brief 触发状态机事件
 * @param sm 状态机实例指针
 * @param event 事件类型
 * @return 0表示成功，非0表示失败
 */
int8_t SM_TriggerEvent(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    if (sm == NULL || !sm->is_running) return -1;
    
    StateID_TypeDef current_state_id = sm->current_state;
    StateDef_TypeDef* current_state = sm->states[current_state_id];
    
    if (current_state == NULL) return -1;
    
    /* 调用当前状态的事件处理函数 */
    if (current_state->handler_func != NULL)
    {
        int8_t result = current_state->handler_func(sm, event);
        if (result != 0) return result;
    }
    
    return 0;
}

/**
 * @brief 获取当前状态
 * @param sm 状态机实例指针
 * @return 当前状态ID
 */
StateID_TypeDef SM_GetCurrentState(StateMachine_TypeDef* sm)
{
    if (sm == NULL) return STATE_ID_INIT;
    return sm->current_state;
}

/**
 * @brief 运行状态机
 * @param sm 状态机实例指针
 */
void SM_Run(StateMachine_TypeDef* sm)
{
    if (sm == NULL || !sm->is_running) return;
    
    StateID_TypeDef current_state_id = sm->current_state;
    StateDef_TypeDef* current_state = sm->states[current_state_id];
    
    if (current_state == NULL) return;
    
    /* 执行当前状态的处理逻辑 */
    if (current_state->handler_func != NULL)
    {
        current_state->handler_func(sm, EVENT_NONE);
    }
}

/**
 * @brief 内部函数：切换到新状态
 * @param sm 状态机实例指针
 * @param new_state_id 新状态ID
 * @return 0表示成功，非0表示失败
 */
static int8_t SM_SwitchToState(StateMachine_TypeDef* sm, StateID_TypeDef new_state_id)
{
    if (sm == NULL) return -1;
    if (new_state_id >= STATE_ID_MAX) return -1;
    
    StateDef_TypeDef* old_state = sm->states[sm->current_state];
    StateDef_TypeDef* new_state = sm->states[new_state_id];
    
    if (new_state == NULL) return -1;
    
    /* 退出旧状态 */
    if (old_state != NULL && old_state->exit_func != NULL)
    {
        old_state->exit_func(sm, EVENT_RETURN_IDLE);
    }
    
    /* 进入新状态 */
    sm->current_state = new_state_id;
    if (new_state->entry_func != NULL)
    {
        new_state->entry_func(sm, EVENT_START);
    }
    
    return 0;
}

/**
 * @brief 获取状态名称
 * @param state_id 状态ID
 * @return 状态名称字符串
 */
const char* SM_GetStateName(StateID_TypeDef state_id)
{
    static const char* state_names[] = {
        "INIT",
        "IDLE", 
        "MANUAL_CTRL",
        "AUTO_TRACK",
        "AUTO_AVOID",
        "TEST",
        "ERROR",
        "SHUTDOWN"
    };
    
    if (state_id < STATE_ID_MAX)
    {
        return state_names[state_id];
    }
    
    return "UNKNOWN";
}

/**
 * @brief 默认状态处理函数（可被具体应用覆盖）
 */
static int8_t DefaultStateHandler(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    switch(event)
    {
        case EVENT_STOP:
            SM_SwitchToState(sm, STATE_ID_IDLE);
            break;
            
        case EVENT_SHUTDOWN:
            SM_SwitchToState(sm, STATE_ID_SHUTDOWN);
            break;
            
        default:
            break;
    }
    
    return 0;
}

/**
 * @brief 初始化状态处理函数
 */
static int8_t InitStateHandler(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    switch(event)
    {
        case EVENT_START:
            /* 初始化完成后自动切换到空闲状态 */
            SM_SwitchToState(sm, STATE_ID_IDLE);
            break;
            
        default:
            break;
    }
    
    return 0;
}

/**
 * @brief 空闲状态处理函数
 */
static int8_t IdleStateHandler(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    switch(event)
    {
        case EVENT_MANUAL_CTRL:
            SM_SwitchToState(sm, STATE_ID_MANUAL_CTRL);
            break;
            
        case EVENT_AUTO_TRACK:
            SM_SwitchToState(sm, STATE_ID_AUTO_TRACK);
            break;
            
        case EVENT_AUTO_AVOID:
            SM_SwitchToState(sm, STATE_ID_AUTO_AVOID);
            break;
            
        case EVENT_TEST_MODE:
            SM_SwitchToState(sm, STATE_ID_TEST);
            break;
            
        case EVENT_ERROR:
            SM_SwitchToState(sm, STATE_ID_ERROR);
            break;
            
        case EVENT_SHUTDOWN:
            SM_SwitchToState(sm, STATE_ID_SHUTDOWN);
            break;
            
        default:
            break;
    }
    
    return 0;
}

/**
 * @brief 手动控制状态处理函数
 */
static int8_t ManualCtrlStateHandler(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    switch(event)
    {
        case EVENT_STOP:
            SM_SwitchToState(sm, STATE_ID_IDLE);
            break;
            
        case EVENT_ERROR:
            SM_SwitchToState(sm, STATE_ID_ERROR);
            break;
            
        default:
            /* 在手动控制状态下持续处理蓝牙命令 */
            break;
    }
    
    return 0;
}

/**
 * @brief 自动循迹状态处理函数
 */
static int8_t AutoTrackStateHandler(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    switch(event)
    {
        case EVENT_STOP:
            SM_SwitchToState(sm, STATE_ID_IDLE);
            break;
            
        case EVENT_ERROR:
            SM_SwitchToState(sm, STATE_ID_ERROR);
            break;
            
        default:
            /* 在自动循迹状态下持续执行循迹算法 */
            break;
    }
    
    return 0;
}

/**
 * @brief 自动避障状态处理函数
 */
static int8_t AutoAvoidStateHandler(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    switch(event)
    {
        case EVENT_STOP:
            SM_SwitchToState(sm, STATE_ID_IDLE);
            break;
            
        case EVENT_ERROR:
            SM_SwitchToState(sm, STATE_ID_ERROR);
            break;
            
        default:
            /* 在自动避障状态下持续执行避障算法 */
            break;
    }
    
    return 0;
}

/**
 * @brief 测试状态处理函数
 */
static int8_t TestStateHandler(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    switch(event)
    {
        case EVENT_STOP:
            SM_SwitchToState(sm, STATE_ID_IDLE);
            break;
            
        case EVENT_ERROR:
            SM_SwitchToState(sm, STATE_ID_ERROR);
            break;
            
        default:
            /* 在测试状态下持续执行测试逻辑 */
            break;
    }
    
    return 0;
}

/**
 * @brief 错误状态处理函数
 */
static int8_t ErrorStateHandler(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    switch(event)
    {
        case EVENT_RETURN_IDLE:
            SM_SwitchToState(sm, STATE_ID_IDLE);
            break;
            
        case EVENT_SHUTDOWN:
            SM_SwitchToState(sm, STATE_ID_SHUTDOWN);
            break;
            
        default:
            /* 在错误状态下停止所有动作 */
            break;
    }
    
    return 0;
}

/**
 * @brief 关机状态处理函数
 */
static int8_t ShutdownStateHandler(StateMachine_TypeDef* sm, StateEvent_TypeDef event)
{
    /* 关机状态下不响应任何事件 */
    return 0;
}

/**
 * @brief 系统级状态机初始化
 */
void SystemStateMachine_Init(void)
{
    /* 初始化状态机实例 */
    SM_Init(&g_state_machine);
    
    /* 定义各个状态 */
    static StateDef_TypeDef init_state = {
        .id = STATE_ID_INIT,
        .name = "INIT",
        .entry_func = NULL,
        .handler_func = InitStateHandler,
        .exit_func = NULL
    };
    
    static StateDef_TypeDef idle_state = {
        .id = STATE_ID_IDLE,
        .name = "IDLE",
        .entry_func = NULL,
        .handler_func = IdleStateHandler,
        .exit_func = NULL
    };
    
    static StateDef_TypeDef manual_ctrl_state = {
        .id = STATE_ID_MANUAL_CTRL,
        .name = "MANUAL_CTRL",
        .entry_func = NULL,
        .handler_func = ManualCtrlStateHandler,
        .exit_func = NULL
    };
    
    static StateDef_TypeDef auto_track_state = {
        .id = STATE_ID_AUTO_TRACK,
        .name = "AUTO_TRACK",
        .entry_func = NULL,
        .handler_func = AutoTrackStateHandler,
        .exit_func = NULL
    };
    
    static StateDef_TypeDef auto_avoid_state = {
        .id = STATE_ID_AUTO_AVOID,
        .name = "AUTO_AVOID",
        .entry_func = NULL,
        .handler_func = AutoAvoidStateHandler,
        .exit_func = NULL
    };
    
    static StateDef_TypeDef test_state = {
        .id = STATE_ID_TEST,
        .name = "TEST",
        .entry_func = NULL,
        .handler_func = TestStateHandler,
        .exit_func = NULL
    };
    
    static StateDef_TypeDef error_state = {
        .id = STATE_ID_ERROR,
        .name = "ERROR",
        .entry_func = NULL,
        .handler_func = ErrorStateHandler,
        .exit_func = NULL
    };
    
    static StateDef_TypeDef shutdown_state = {
        .id = STATE_ID_SHUTDOWN,
        .name = "SHUTDOWN",
        .entry_func = NULL,
        .handler_func = ShutdownStateHandler,
        .exit_func = NULL
    };
    
    /* 注册所有状态 */
    SM_RegisterState(&g_state_machine, &init_state);
    SM_RegisterState(&g_state_machine, &idle_state);
    SM_RegisterState(&g_state_machine, &manual_ctrl_state);
    SM_RegisterState(&g_state_machine, &auto_track_state);
    SM_RegisterState(&g_state_machine, &auto_avoid_state);
    SM_RegisterState(&g_state_machine, &test_state);
    SM_RegisterState(&g_state_machine, &error_state);
    SM_RegisterState(&g_state_machine, &shutdown_state);
    
    /* 启动状态机 */
    g_state_machine.is_running = 1;
}

/**
 * @brief 获取系统状态机实例
 * @return 系统状态机指针
 */
StateMachine_TypeDef* System_GetStateMachine(void)
{
    return &g_state_machine;
}