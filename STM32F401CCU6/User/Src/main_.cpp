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


int Main_Init(void) {
	HAL_StatusTypeDef ret;
    Debug_Init();
    // ret = MPU_Init();
	while (MPU_Init() != HAL_OK) {
		osDelay(200);
	}
    Motor_Init();
    SysTimer_Init();
    return 0;
}

int Main_Debug(void) {
    return 0;
}

int Main_Process(void) {
		HAL_Delay(1500);
    Main_Init();
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

void Main_TIM_ElapsedHandler(TIM_HandleTypeDef *htim) {
  if (htim->Instance == SYS_MICRO_TIMER_HANDLE.Instance) {
    SysTimer_PeriodElapsedCallback();
  }
  if (htim->Instance == SYS_PERIODIC_TIMER_HANDLE.Instance) {
      Main_TIM_PeriodElapsedCallback();
  }
}

// I Guess frequency == 1kHz.
void Main_TIM_PeriodElapsedCallback(void) {
    /****************TODO****************/
}
