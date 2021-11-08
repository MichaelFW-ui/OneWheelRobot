/**
 * @file sys_timer.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 微秒级的系统时钟。使用传统C语言编写。
 * @version 0.1
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __SYS_TIMER_H
#define __SYS_TIMER_H

#include "stm32f4xx.h"
#include "tim.h"

extern uint32_t sys_timer_cnt;

#define SYS_MICRO_TIMER_HANDLE htim9

__STATIC_INLINE void SysTimer_Init(void) {
    sys_timer_cnt = 0;
    HAL_TIM_Base_Start_IT(&SYS_MICRO_TIMER_HANDLE);
}

__STATIC_INLINE uint32_t SysTimer_GetTick_Microseconds(void) {
    return SYS_MICRO_TIMER_HANDLE.Instance->CNT + sys_timer_cnt * 0xffff;
}

void SysTimer_PeriodElapsedCallback(void);


#endif // !__SYS_TIMER_H
