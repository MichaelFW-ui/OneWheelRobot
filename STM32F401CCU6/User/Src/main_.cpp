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
#include "mpu6050.h"
#include "motor.h"
#include "tim.h"

MotorTypeDef *MotorUpper, *MotorLower;

int Main_Init(void) {
    Debug_Init();
    MPU_Init();
    MotorUpper = new MotorTypeDef(
        Motor_AIN1_GPIO_Port, Motor_AIN1_Pin, Motor_AIN2_GPIO_Port, Motor_AIN1_Pin,
        &htim2, HAL_TIM_ACTIVE_CHANNEL_1, Encoder_UpperSub_GPIO_Port,
        Encoder_UpperSub_Pin, &htim1, TIM_CHANNEL_2);

    MotorUpper =
        new MotorTypeDef(Motor_BIN1_GPIO_Port, Motor_BIN1_Pin,
                         Motor_BIN2_GPIO_Port, Motor_BIN1_Pin, &htim2,
                         HAL_TIM_ACTIVE_CHANNEL_2, Encoder_LowerSub_GPIO_Port,
                         Encoder_LowerSub_Pin, &htim1, TIM_CHANNEL_3);
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
