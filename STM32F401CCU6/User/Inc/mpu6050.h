/**
 * @file mpu6050.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief MPU6050DMP库的引入
 * @version 0.1
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MPU6050_H
#define __MPU6050_H

#define EMPL_TARGET_MSP430
#define MPU6050
#define q30  1073741824.0f


#include "stm32f4xx.h"
#include "i2c.h"

#define MPU_IIC_HANDLE hi2c1


int MPU_IIC_WriteRegister(uint8_t slave_addr, uint8_t reg_addr, uint16_t len, uint8_t *data_ptr);
int MPU_IIC_ReadRegister(uint8_t slave_addr, uint8_t reg_addr, uint16_t len, uint8_t *data_ptr);

int MPU_GetTickCount(unsigned long *count);


uint8_t MPU_run_self_test(void);
uint8_t MPU_dmp_init(void);
uint8_t MPU_dmp_get_data(float *pitch,float *roll,float *yaw);
HAL_StatusTypeDef MPU_Init(void);

#endif // !__MPU6050_H

