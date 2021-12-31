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
#include "ctrl.h"

float pitch, roll, yaw;

uint8_t Initialized_all;

int Main_Init(void) {
  HAL_StatusTypeDef ret;
  SysTimer_Init();
  Debug_Init();

  while (MPU_Init() != HAL_OK) {
    osDelay(200);
  }

  Motor_Init();
	Ctrl_Init();
  Initialized_all = 1;
  return 0;
}

int Main_Debug(void) {
	static int cnt = 0;
	#define NUMBER 2
	float pitchs[NUMBER], rolls[NUMBER], yaws[NUMBER];
	float pitch__, roll__, yaw__;

  // MPU6050 Test!!!
  uint8_t ret = MPU_dmp_get_data(&pitch, &roll, &yaw);
  while (1) {
    ret = MPU_dmp_get_data(&(pitchs[cnt % NUMBER]), &(rolls[cnt % NUMBER]), &(yaws[cnt % NUMBER]));
    if (ret) {
      continue;
    }
		pitch__ = 0;
		roll__ = 0;
		yaw__ = 0;
		for(int i = 0; i < NUMBER; ++i) {
			pitch__ += pitchs[i] / NUMBER;
			roll__ += rolls[i] / NUMBER;
			yaw__ += yaws[i] / NUMBER;
		}
		pitch = pitch__;
		roll = roll__;
		yaw = yaw__;
		
		if (!(cnt % 6))
			usb_printf("%f,%f,%f,%d,%d\r\n", pitch, roll, yaw, MotorUpper->CurrentSpeed, MotorLower->CurrentSpeed);
    vTaskDelay(3);
		++cnt;
  }
  return 0;
}

int Main_Process(void) {
  HAL_Delay(1500);
  Main_Init();

  Motor_STBY_GPIO_Port->ODR |= Motor_STBY_Pin;
  // Motor_AIN2_GPIO_Port->ODR &= ~Motor_AIN2_Pin;
  // SysTimer_Delay_us(10);
  // Motor_AIN1_GPIO_Port->ODR |= Motor_AIN1_Pin;

  // __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, 500);

  Main_Debug();

  while (1) {
    Motor_PrintSpeed();
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
  if (!(cnt % 5)) {
    Ctrl_PeriodicCompute();
  }

  ++cnt;
    /****************TODO****************/
}
