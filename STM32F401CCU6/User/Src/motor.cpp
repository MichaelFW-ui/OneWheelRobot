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

myPID MotorUpperPID, MotorLowerPID_Speed, MotorLowerPID_Position;

uint16_t MotorLowerPosition_Enable;

// Target
int16_t MotorUpper_Speed, MotorLower_Speed, MotorLower_Position;


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

void Motor_PeriodElapsedCallback(MotorTypeDef *Motor) {
    if (Motor == MotorUpper) {
        ++Motor->CaptureCnt;
        if (Motor->CaptureCnt > 1) {
            MotorUpperPID.Current = 0;
        }
    }
    if (Motor == MotorLower) {
        // Never called here
    }
}

#define BIG_INT ((1 << 30) - 1)

void Motor_InputCaptureCallback(MotorTypeDef *Motor) {
    if (Motor == MotorUpper) {
        static uint32_t last = 0;
        MotorUpperPID.Current =
            (BIG_INT / ((Motor->TIM_Input_Handle->Instance->CNT - last +
                         65535 * Motor->CaptureCnt)));
        last = Motor->TIM_Input_Handle->Instance->CNT;
        Motor->CaptureCnt = 0;

        if (Motor->Input_GPIO_Port->IDR & Motor->Input_GPIO_Pin) {
          MotorUpperPID.Current *= 1;
        } else {
            MotorUpperPID.Current *= -1;
        }
    }

    if (Motor == MotorLower) {
        if (Motor->Input_GPIO_Port->IDR & Motor->Input_GPIO_Pin) {
            ++Motor->CaptureCnt;
        } else {
            --Motor->CaptureCnt;
        }
    }
}

/**
 * @brief Runs at frequency of 100Hz.
 * 
 * @param Motor 
 */
void Motor_PeriodicUpdate(MotorTypeDef *Motor) {
  /****************** Motor Upper Periodic Update *****************************/
  if (Motor == MotorUpper) {
      MotorUpperPID.Target = MotorUpper_Speed;
    /* TODO */
  }

  /****************** Motor Lower Periodic Update *****************************/

  if (Motor == MotorLower) {
    MotorLowerPID_Speed.Target = MotorLower_Speed;

    Motor->Current = Motor->CaptureCnt;
    Motor->CaptureCnt = 0;
    static uint32_t MotorLowerCount = 0;
    if (!(MotorLowerCount % 2)) {
      MotorLowerPID_Speed.Current = Motor->Current;
			// usb_printf("%d\r\n", Motor->Current);
      MotorLowerPID_Speed.Adjust();
      if (Motor->Output > 0) {
        Motor->DirB_GPIO_Port->ODR &= ~Motor->DirB_GPIO_Pin;
        SysTimer_Delay_us(10);
        Motor->DirA_GPIO_Port->ODR |= Motor->DirA_GPIO_Pin;
        __HAL_TIM_SetCompare(Motor->TIM_Output_Handle, Motor->OutputChannel,
                             (uint32_t)Motor->Output);
      } else {
        Motor->DirA_GPIO_Port->ODR &= ~Motor->DirA_GPIO_Pin;
        SysTimer_Delay_us(10);
        Motor->DirB_GPIO_Port->ODR |= Motor->DirB_GPIO_Pin;
        __HAL_TIM_SetCompare(Motor->TIM_Output_Handle, Motor->OutputChannel,
                             (uint32_t)(-Motor->Output));
      }
    }
    if (!(MotorLowerCount % 4) && MotorLowerPosition_Enable) {
      MotorLowerPID_Position.Target = MotorLower_Position;
      /*TODO Position*/
    }
    ++MotorLowerCount;
  }
}

void Motor_PrintSpeed(void) {
    usb_printf("Upper: %f\r\n", MotorUpperPID.Current);
    usb_printf("Lower: %f\r\n", MotorLowerPID_Speed.Current);
}




void Motor_Init(void) {
  MotorUpper = new MotorTypeDef(
      Motor_BIN1_GPIO_Port, Motor_BIN1_Pin, Motor_BIN2_GPIO_Port,
      Motor_BIN1_Pin, &htim2, HAL_TIM_ACTIVE_CHANNEL_1,
      Encoder_UpperSub_GPIO_Port, Encoder_UpperSub_Pin, &htim1, TIM_CHANNEL_2);

  MotorUpperPID.SetPIDParam(0, 0, 0, 1000, 1000);
  MotorUpperPID.getMicroTick_regist(SysTimer_GetTick_Microseconds);

  MotorLower = new MotorTypeDef(
      Motor_AIN1_GPIO_Port, Motor_AIN1_Pin, Motor_AIN2_GPIO_Port,
      Motor_AIN1_Pin, &htim2, HAL_TIM_ACTIVE_CHANNEL_2,
      Encoder_LowerSub_GPIO_Port, Encoder_LowerSub_Pin, &htim1, TIM_CHANNEL_3);

  MotorLowerPID_Speed.getMicroTick_regist(SysTimer_GetTick_Microseconds);
  MotorLowerPID_Speed.SetPIDParam(0, 0, 0, 1000, 1000);

  MotorLowerPID_Position.getMicroTick_regist(SysTimer_GetTick_Microseconds);
  MotorLowerPID_Position.SetPIDParam(0, 0, 0, 1000, 1000);

  MotorLowerPosition_Enable = 0;

  MotorUpper_Speed = MotorLower_Speed = MotorLower_Position = 0;

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


