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
#include "cmsis_os.h"
#include "tim.h"
#include "motor.h"

uint8_t Initialized_all;


int Main_Init(void) {
		HAL_StatusTypeDef ret;
    SysTimer_Init();
    Debug_Init();
    // ret = MPU_Init();
	while (MPU_Init() != HAL_OK) {
		osDelay(200);
	}
    Motor_Init();
		Initialized_all = 1;
    return 0;
}

int Main_Debug(void) {
		float pitch, roll, yaw;
		uint8_t ret = MPU_dmp_get_data(&pitch, &roll, &yaw);
		while (1) {
    ret = MPU_dmp_get_data(&pitch, &roll, &yaw);
      if (ret) {
        // usb_printf("Failed ret %d\r\n",ret );
        continue;
      }
			usb_printf("%f, %f, %f", pitch, roll, yaw);
			vTaskDelay(50);
		}
    return 0;
}

int Main_Process(void) {
		HAL_Delay(1500);
    Main_Init();

    // Motor_STBY_GPIO_Port->ODR |= Motor_STBY_Pin;
    Motor_AIN2_GPIO_Port->ODR &= ~Motor_AIN2_Pin;
    SysTimer_Delay_us(10);
    Motor_AIN1_GPIO_Port->ODR |= Motor_AIN1_Pin;

    // __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, 500);



    while (1) {
      // usb_printf("Main Process running well\r\n");
      // vTaskDelay(800);
      Motor_PrintSpeed();
      vTaskDelay(200);
    }
    return 0;
}

void Main_TIM_ElapsedHandler(TIM_HandleTypeDef *htim) {
  if (htim->Instance == SYS_MICRO_TIMER_HANDLE.Instance) {
    SysTimer_PeriodElapsedCallback();
  }
  if (htim->Instance == SYS_PERIODIC_TIMER_HANDLE.Instance) {
      Main_TIM_PeriodElapsedCallback();
  }
  if (htim->Instance == MOTOR_INPUT_CAPTURE_HANDLE.Instance) {
    Motor_PeriodElapsedCallback(MotorUpper);
    Motor_PeriodElapsedCallback(MotorLower);
  }
}

// I Guess frequency == 1kHz.
void Main_TIM_PeriodElapsedCallback(void) {
	if (!Initialized_all)
		return;
  static uint32_t cnt = 0;
  if (!(cnt % 10)) {
    Motor_PeriodicUpdate(MotorUpper);
    Motor_PeriodicUpdate(MotorLower);
  }

  ++cnt;
    /****************TODO****************/
}
