/**
 * @file sys_timer.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 微秒级的系统时钟。使用传统C语言编写。
 * @version 0.1
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "sys_timer.h"

uint32_t sys_timer_cnt;

void SysTimer_PeriodElapsedCallback(void) {
    ++sys_timer_cnt;
}

