/**
 * @file motor.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 电机的主要控制方法
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

