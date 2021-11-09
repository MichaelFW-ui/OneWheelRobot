/**
 * @file main_.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 主进程主要文件。
 * @version 0.1
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "main_.h"
#include "debug.h"
#include "sys_timer.h"

int Main_Init(void) {
    Debug_Init();
    return 0;
}

int Main_Debug(void) {
    return 0;
}

int Main_Process(void) {
    return 0;
}

void Main_TIM_ElapsedHandler(TIM_HandleTypeDef *htim) {
  if (htim->Instance == SYS_MICRO_TIMER_HANDLE.Instance) {
    SysTimer_PeriodElapsedCallback();
  }
}
