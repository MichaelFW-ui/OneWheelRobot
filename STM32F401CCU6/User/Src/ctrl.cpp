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

extern float pitch, roll, yaw;

PID_TypeDef AngleX, SpeedX, AngleY, SpeedY;

PID_TypeDef MotorSpeedX, MotorSpeedY;





float SpeedTargetDeadZone_X, SpeedTargetDeadZone_Y;

int32_t DeadZone_Upper = 0x12;
int32_t DeadZone_Lower = 50;

// X -- roll, Y -- pitch
float DefaultAngle_X, DefaultAngle_Y;

float MotorSpeedTarget_X, MotorSpeedTarget_Y;

void Ctrl_Init(void) {
  float pitch, roll, yaw;
  while (MPU_dmp_get_data(&pitch, &roll, &yaw))
    ;  // retry when reading failed.

  DefaultAngle_X = roll, DefaultAngle_Y = pitch;

  AngleX.SetPIDParam(-1000000, 0, 0, 900000, 5000000);
  // AngleX.SetPIDParam(-180, 0, -4.5, 900, 800);
  // AngleX.SetPIDParam(-698, 0, -1, 900, 800);
  // AngleX.SetPIDParam(-145000, 0, 0, 0, 1000000);
  // AngleX.SetPIDParam(-550, 0, -15, 900, 800);
  // AngleX.SetPIDParam(-200, 0, -20, 900, 800);
  // AngleX.SetPIDParam(0, 0, 0, 900, 800);
  // SpeedX.SetPIDParam(0, 0.1, 0, 5, 6);
  // AngleY.SetPIDParam(22, 0, 10, 900, 800);
  // AngleY.SetPIDParam(0, 0, 0, 900, 800);
  // SpeedY.SetPIDParam(0, 0.1, 0, 5, 6);

  MotorSpeedX.SetPIDParam(0.000700000028, 0.000500000077, 0, 900, 800);
  MotorSpeedY.SetPIDParam(0, 0, 0, 900, 800);
  AngleX.DeadZone = 0;
  AngleY.DeadZone = 0;
	MotorSpeedX.I_SeparThresh = 200000;
	MotorSpeedY.I_SeparThresh = 200000;
  SpeedTargetDeadZone_X = 3500;
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
	
	// Motor_SetOutput(MotorUpper, 500);
	
	// 200Hz
	{
		// Ctrl_PID_MotorSpeed_Y();
		Ctrl_PID_MotorSpeed_X();
	}

  // 50Hz
  if (!(cnt % 4)) {
    Ctrl_PID_Angle_X();
    // if (roll > -5 && roll < 5) {
		// 	if (AngleX.Out > 0)
		// 		Motor_SetOutput(MotorUpper, AngleX.Out + DeadZone_Upper);
		// 	else
		// 		Motor_SetOutput(MotorUpper, AngleX.Out - DeadZone_Upper);
		// }
    // else
    //   Motor_SetOutput(MotorUpper, 0);
		
    // Ctrl_PID_Angle_Y();
    // if (pitch > -5 && pitch < 5) {
		// 	if (AngleY.Out > 0)
		// 		Motor_SetOutput(MotorLower, AngleY.Out + DeadZone_Lower);
		// 	else
		// 		Motor_SetOutput(MotorLower, AngleY.Out - DeadZone_Lower);
		// }
    // else
    //   Motor_SetOutput(MotorLower, 0);
  }
  // 40Hz
  if (!(cnt % 5)) {
    // Ctrl_PID_Speed_X();
  }

  // 20Hz
  if (!cnt % 10) {
    // Ctrl_PID_Speed_Y();
  }
	++cnt;
}

void Ctrl_PID_Angle_X(void) {
  AngleX.Current = pitch;
  AngleX.Adjust();
  Ctrl_SetMotorSpeedX(AngleX.Out);
}

void Ctrl_PID_Angle_Y(void) {
  AngleY.Current = roll;
  AngleY.Adjust();
	Ctrl_SetMotorSpeedY(AngleY.Out);
}

void Ctrl_PID_Speed_X(void) {
  SpeedX.Current = MotorUpper->CurrentSpeed;
  SpeedX.Adjust();
  AngleX.Target = DefaultAngle_X - SpeedX.Out;
}

void Ctrl_PID_Speed_Y(void) {
  SpeedY.Current = MotorLower->CurrentSpeed;
  SpeedY.Adjust();
  AngleY.Target = DefaultAngle_Y - SpeedY.Out;
}

void Ctrl_PID_MotorSpeed_X(void) {
  MotorSpeedX.Target = MotorSpeedTarget_X;
  MotorSpeedX.Current = MotorUpper->CurrentSpeed;
  MotorSpeedX.Adjust();
    if (pitch > -10 && pitch < 10) {
			if (MotorSpeedX.Out > 0)
				Motor_SetOutput(MotorUpper, MotorSpeedX.Out + DeadZone_Upper);
			else
				Motor_SetOutput(MotorUpper, MotorSpeedX.Out - DeadZone_Upper);
		}
    else
      Motor_SetOutput(MotorUpper, 0);

}

void Ctrl_PID_MotorSpeed_Y(void) {
  MotorSpeedY.Target = MotorSpeedTarget_Y;
  MotorSpeedY.Current = MotorLower->CurrentSpeed;
  MotorSpeedY.Adjust();
    if (roll > -10 && roll < 10) {
			if (MotorSpeedY.Out > 0)
				Motor_SetOutput(MotorLower, MotorSpeedY.Out + DeadZone_Upper);
			else
				Motor_SetOutput(MotorLower, MotorSpeedY.Out - DeadZone_Upper);
		}
    else
      Motor_SetOutput(MotorLower, 0);
}


void Ctrl_SetMotorSpeedX(float speed) {
  if (speed > 0)
    MotorSpeedTarget_X = speed + SpeedTargetDeadZone_X;
  else
    MotorSpeedTarget_X = speed - SpeedTargetDeadZone_X;
}

void Ctrl_SetMotorSpeedY(float speed) { MotorSpeedTarget_Y = speed; }
