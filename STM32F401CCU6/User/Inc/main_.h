/**
 * @file main_.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 主进程主要文件。
 * @version 0.1
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MAIN__H
#define __MAIN__H
#include "tim.h"
#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

int Main_Init(void);

int Main_Debug(void);

int Main_Process(void);

void Main_TIM_ElapsedHandler(TIM_HandleTypeDef *htim);

void Main_TIM_PeriodElapsedCallback(void);

#ifdef __cplusplus
}
#endif

#endif // !__MAIN_H

