/**
 * @file ctrl.cpp
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 通过算法使机器人达到平衡
 *
 * 算法的框架来自稚辉君，具体细节在调试成功后会进行补充。
 * @version 0.1
 * @date 2021-11-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "ctrl.h"

#include "PID.h"
#include "motor.h"
#include "mpu6050.h"
#include "stm32f4xx.h"
#include "sys_timer.h"

PID_TypeDef AngleX, SpeedX, AngleY, SpeedY;

// X -- roll, Y -- pitch
float DefaultAngle_X, DefaultAngle_Y;

int16_t DeadZone;

void Ctrl_Init(void) {
  float pitch, roll, yaw;
  while (MPU_dmp_get_data(&pitch, &roll, &yaw))
    ;  // retry when reading failed.

  DefaultAngle_X = roll, DefaultAngle_Y = pitch;

  AngleX.SetPIDParam(0, 0, 0, 1000, 1000);
  SpeedX.SetPIDParam(0, 0, 0, 1000, 1000);
  AngleY.SetPIDParam(0, 0, 0, 1000, 1000);
  SpeedY.SetPIDParam(0, 0, 0, 1000, 1000);
  AngleX.getMicroTick_regist(SysTimer_GetTick_Microseconds);
  SpeedX.getMicroTick_regist(SysTimer_GetTick_Microseconds);
  AngleY.getMicroTick_regist(SysTimer_GetTick_Microseconds);
  SpeedY.getMicroTick_regist(SysTimer_GetTick_Microseconds);
}

void Ctrl_SetTargetForwardSpeed(int32_t speed) { SpeedY.Target = speed; }

/**
 * @brief Call at frequency of 200Hz
 *
 */
void Ctrl_PeriodicCompute(void) {
  static uint8_t cnt = 0;

  // 100Hz
  if (!(cnt % 2)) {
    Ctrl_PID_Angle_X();
    Motor_SetOutput(MotorUpper, AngleX.Out);
		// Motor_SetOutput(MotorUpper, 500);
    Ctrl_PID_Angle_Y();
		Motor_SetOutput(MotorLower, 500);
    //Motor_SetOutput(MotorLower, AngleY.Out);
  }
  // 40Hz
  if (!(cnt % 5)) {
    Ctrl_PID_Speed_X();
  }

  // 20Hz
  if (!cnt % 10) {
    Ctrl_PID_Speed_Y();
  }
}

void Ctrl_PID_Angle_X(void) {
    return;
  float pitch, roll, yaw;
  while (MPU_dmp_get_data(&pitch, &roll, &yaw))
    ;
  AngleX.Current = roll;
}

void Ctrl_PID_Angle_Y(void) {
    return;
  float pitch, roll, yaw;
  while (MPU_dmp_get_data(&pitch, &roll, &yaw))
    ;
  AngleY.Current = pitch;
  AngleY.Adjust();
}

void Ctrl_PID_Speed_X(void) {
  SpeedX.Current = MotorUpper->CurrentSpeed;
  SpeedX.Adjust();
  AngleY.Target = DefaultAngle_X + SpeedX.Out;
}

void Ctrl_PID_Speed_Y(void) {
  MotorLower->CurrentSpeed = MotorLower->CaptureCnt;
  MotorLower->CaptureCnt = 0;

  SpeedY.Current = MotorLower->CurrentSpeed;
  SpeedY.Adjust();
  AngleY.Target = DefaultAngle_Y - SpeedY.Out;
}
