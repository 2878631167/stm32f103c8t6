#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H

#ifdef USE_STDPERIPH_DRIVER
    #include "stm32f10x_gpio.h"
    #include "stm32f10x_rcc.h"
    #include "stm32f10x_flash.h"
#endif

#define assert_param(expr) ((void)0)

#endif