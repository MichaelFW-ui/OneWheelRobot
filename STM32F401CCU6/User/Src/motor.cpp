/**
 * @file motor.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 电机的主要控制方法
 *  PID库的使用方法已经加载在代码文件夹中，可供参阅。
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "motor.h"
#include "debug.h"
#include "tim.h"
#include "sys_timer.h"

MotorTypeDef *MotorUpper, *MotorLower;

MotorTypeDef::MotorTypeDef() {
    usb_printf("Failed to initialize the motor\r\n");
}

MotorTypeDef::MotorTypeDef(GPIO_TypeDef *DirA_GPIO_Port, uint16_t DirA_GPIO_Pin,
                           GPIO_TypeDef *DirB_GPIO_Port, uint16_t DirB_GPIO_Pin,
                           TIM_HandleTypeDef *TIM_Input_Handle,
                           uint8_t InputChannel, GPIO_TypeDef *Input_GPIO_Port,
                           uint16_t Input_GPIO_Pin,
                           TIM_HandleTypeDef *TIM_Output_Handle,
                           uint8_t OutputChannel) {
  this->DirA_GPIO_Port = DirA_GPIO_Port;
  this->DirA_GPIO_Pin = DirA_GPIO_Pin;
  this->DirB_GPIO_Port = DirB_GPIO_Port;
  this->DirB_GPIO_Pin = DirB_GPIO_Pin;
  this->TIM_Input_Handle = TIM_Input_Handle;
  this->InputChannel = InputChannel;
  this->Input_GPIO_Port = Input_GPIO_Port;
  this->Input_GPIO_Pin = Input_GPIO_Pin;
  this->TIM_Output_Handle = TIM_Output_Handle;
  this->OutputChannel = OutputChannel;
  this->CaptureCnt = 0;
  this->CurrentSpeed = 0;
}

void Motor_PeriodElapsedCallback(MotorTypeDef *Motor) {
  if (Motor == MotorUpper) {
    ++Motor->CaptureCnt;
    if (Motor->CaptureCnt > 1) {
      Motor->CurrentSpeed = 0;
    }
  }
  if (Motor == MotorLower) {
    // Never called here
  }
}

#define BIG_INT ((1 << 30) - 1)

void Motor_InputCaptureCallback(MotorTypeDef *Motor) {
  if (Motor == MotorUpper) {
    if (Motor->Input_GPIO_Port->IDR & Motor->Input_GPIO_Pin) {
      Motor->CurrentSpeed = 1;
    } else {
      Motor->CurrentSpeed = -1;
    }
    static uint32_t last = 0;
    Motor->CurrentSpeed *=
        (BIG_INT / (((Motor->TIM_Input_Handle->Instance->CNT) - last +
                     65535 * Motor->CaptureCnt)));
    last = Motor->TIM_Input_Handle->Instance->CNT;
    Motor->CaptureCnt = 0;
  }

  if (Motor == MotorLower) {
    if (Motor->Input_GPIO_Port->IDR & Motor->Input_GPIO_Pin) {
      ++Motor->CaptureCnt;
    } else {
      --Motor->CaptureCnt;
    }
  }
}


void Motor_PrintSpeed(void) {
    usb_printf("Upper: %f\r\n", MotorUpper->CurrentSpeed);
    usb_printf("Lower: %f\r\n", MotorLower->CurrentSpeed);
}


void Motor_Init(void) {
  MotorUpper = new MotorTypeDef(
      Motor_BIN1_GPIO_Port, Motor_BIN1_Pin, Motor_BIN2_GPIO_Port,
      Motor_BIN2_Pin, &htim2, HAL_TIM_ACTIVE_CHANNEL_1,
      Encoder_UpperSub_GPIO_Port, Encoder_UpperSub_Pin, &htim1, TIM_CHANNEL_2);


  MotorLower = new MotorTypeDef(
      Motor_AIN1_GPIO_Port, Motor_AIN1_Pin, Motor_AIN2_GPIO_Port,
      Motor_AIN2_Pin, &htim2, HAL_TIM_ACTIVE_CHANNEL_2,
      Encoder_LowerSub_GPIO_Port, Encoder_LowerSub_Pin, &htim1, TIM_CHANNEL_3);

  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_1) {
      Motor_InputCaptureCallback(MotorUpper);
    }
    if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_2) {
      Motor_InputCaptureCallback(MotorLower);
    }
  }
}

void Motor_SetOutput(MotorTypeDef *Motor, float output) {
  if (output >= 0) {
    if (output > 1000) output = 1000;
    Motor->DirB_GPIO_Port->ODR &= ~Motor->DirB_GPIO_Pin;
    SysTimer_Delay_us(10);
    Motor->DirA_GPIO_Port->ODR |= Motor->DirA_GPIO_Pin;
    __HAL_TIM_SetCompare(Motor->TIM_Output_Handle, Motor->OutputChannel,
                         (uint32_t)output);
  } else {
    if (output < -1000) output = -1000;
    Motor->DirA_GPIO_Port->ODR &= ~Motor->DirA_GPIO_Pin;
    SysTimer_Delay_us(10);
    Motor->DirB_GPIO_Port->ODR |= Motor->DirB_GPIO_Pin;
    __HAL_TIM_SetCompare(Motor->TIM_Output_Handle, Motor->OutputChannel,
                         ((uint32_t)-output));
  }
}
