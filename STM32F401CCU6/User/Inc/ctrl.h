/**
 * @file ctrl.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 通过算法使机器人达到平衡
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CTRL_H
#define __CTRL_H
#include "stm32f4xx.h"

void Ctrl_Init(void);

void Ctrl_SetTargetForwardSpeed(int32_t speed);

void Ctrl_PID_Angle_X(void);

void Ctrl_PID_Speed_X(void);

void Ctrl_PID_Angle_Y(void);

void Ctrl_PID_Speed_Y(void);

void Ctrl_PeriodicCompute(void);

#endif // !__CTRL_H
