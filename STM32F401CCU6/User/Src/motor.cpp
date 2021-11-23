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
}

void MotorTypeDef::PID_Configure(float _Kp, float _Ki, float _Kd, float _I_Term_Max, float _Out_Max) {
    this->pid.SetPIDParam(_Kp, _Ki, _Kd, _I_Term_Max, _Out_Max);
}


void MotorTypeDef::PID_TimeConfigure(uint32_t (*getTick_fun)(void)) {
    this->pid.getMicroTick_regist(getTick_fun);
}

void MotorTypeDef::OnCapture(void) {
    if (this->Input_GPIO_Port->IDR | this->Input_GPIO_Pin) {
        ++this->CaptureCnt;
    } else {
        --this->CaptureCnt;
    }
}

void MotorTypeDef::SetTargetSpeed(int16_t speed) {
    this->pid.Target = speed;
}

void MotorTypeDef::OnPeriodicActivation(void) {
    this->pid.Current = this->CaptureCnt;
    this->CaptureCnt = 0;
    this->Output = this->pid.Adjust();
    if (this->Output > 0) {
        this->DirB_GPIO_Port->ODR &= ~this->DirB_GPIO_Pin;
        SysTimer_Delay_us(10);
        this->DirA_GPIO_Port->ODR |= this->DirA_GPIO_Pin;
    __HAL_TIM_SetCompare(this->TIM_Output_Handle, this->OutputChannel, (uint32_t)this->Output);
    } else {
        this->DirA_GPIO_Port->ODR &= ~this->DirA_GPIO_Pin;
        SysTimer_Delay_us(10);
        this->DirB_GPIO_Port->ODR |= this->DirB_GPIO_Pin;
    __HAL_TIM_SetCompare(this->TIM_Output_Handle, this->OutputChannel, (uint32_t)(-this->Output));
    }

}

void Motor_Init(void) {
  MotorUpper = new MotorTypeDef(
      Motor_AIN1_GPIO_Port, Motor_AIN1_Pin, Motor_AIN2_GPIO_Port,
      Motor_AIN1_Pin, &htim2, HAL_TIM_ACTIVE_CHANNEL_1,
      Encoder_UpperSub_GPIO_Port, Encoder_UpperSub_Pin, &htim1, TIM_CHANNEL_2);

  MotorUpper->PID_Configure(0, 0, 0, 1000, 1000);
  MotorUpper->PID_TimeConfigure(SysTimer_GetTick_Microseconds);

  MotorLower = new MotorTypeDef(
      Motor_BIN1_GPIO_Port, Motor_BIN1_Pin, Motor_BIN2_GPIO_Port,
      Motor_BIN1_Pin, &htim2, HAL_TIM_ACTIVE_CHANNEL_2,
      Encoder_LowerSub_GPIO_Port, Encoder_LowerSub_Pin, &htim1, TIM_CHANNEL_3);
  MotorLower->PID_Configure(0, 0, 0, 1000, 1000);
  MotorLower->PID_TimeConfigure(SysTimer_GetTick_Microseconds);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
}
