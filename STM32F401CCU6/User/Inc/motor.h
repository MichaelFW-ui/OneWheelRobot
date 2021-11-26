/**
 * @file motor.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 电机的主要控制方法
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx.h"

#define MOTOR_INPUT_CAPTURE_HANDLE htim2
#define MOTOR_PWM_HANDLE htim1

struct MotorTypeDef{
    GPIO_TypeDef *DirA_GPIO_Port;
    uint16_t DirA_GPIO_Pin;
    GPIO_TypeDef *DirB_GPIO_Port;
    uint16_t DirB_GPIO_Pin;
    uint8_t InputChannel;
    GPIO_TypeDef *Input_GPIO_Port;
    uint16_t Input_GPIO_Pin;
    TIM_HandleTypeDef *TIM_Output_Handle;
    uint8_t OutputChannel;
    TIM_HandleTypeDef *TIM_Input_Handle;
    int32_t CaptureCnt;
    int32_t CurrentSpeed;
public:
    MotorTypeDef();
    MotorTypeDef(GPIO_TypeDef *DirA_GPIO_Port, uint16_t DirA_GPIO_Pin,
                 GPIO_TypeDef *DirB_GPIO_Port, uint16_t DirB_GPIO_Pin,
                 TIM_HandleTypeDef *TIM_Input_Handle, uint8_t InputChannel,
                 GPIO_TypeDef *Input_GPIO_Port, uint16_t Input_GPIO_Pin,
                 TIM_HandleTypeDef *TIM_Output_Handle, uint8_t OutputChannel);
};
    void PID_Configure(float _Kp, float _Ki, float _Kd, float _I_Term_Max, float _Out_Max);
    void PID_TimeConfigure(uint32_t (*getTick_fun)(void));
    void SetTargetSpeed(int16_t target);

extern MotorTypeDef *MotorUpper, *MotorLower;


void Motor_Init(void);

void Motor_PeriodElapsedCallback(MotorTypeDef *Motor);

void Motor_InputCaptureCallback(MotorTypeDef *Motor);

void Motor_PrintSpeed(void);

void Motor_SetOutput(MotorTypeDef *Motor, float output);

#endif // !__MOTOR_H

