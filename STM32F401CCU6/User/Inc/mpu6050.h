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

#define EMPL_TARGET_STM32F401
#define MPU6050

#include "stm32f4xx.h"
#include "i2c.h"

#define MPU_IIC_HANDLE hi2c1


int MPU_IIC_WriteRegister(uint8_t slave_addr, uint8_t reg_addr, uint16_t len, uint8_t *data_ptr);
int MPU_IIC_ReadRegister(uint8_t slave_addr, uint8_t reg_addr, uint16_t len, uint8_t *data_ptr);

#endif // !__MPU6050_H

