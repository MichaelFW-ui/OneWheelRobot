/**
 * @file debug.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 使用USB虚拟串口模拟的调试窗口。
 * @version 0.1
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __DEBUG_H

#include "stm32f4xx.h"

#define BUFFER_SIZE 256

extern uint8_t RxDataBuffer[BUFFER_SIZE];

HAL_StatusTypeDef Debug_ProcessCommand(uint8_t *cmd);

void Debug_Main(void);

void Debug_Init(void);

void Debug_PrintAllArguments(void);


#endif // !__DEBUG_H
