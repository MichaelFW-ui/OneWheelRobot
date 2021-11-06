/**
 * @file mpu6050.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief MPU6050DMP库的引入
 * @version 0.1
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "mpu6050.h"



int MPU_IIC_WriteRegister(uint8_t slave_addr, uint8_t reg_addr, uint16_t len, uint8_t *data_ptr) {
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(&MPU_IIC_HANDLE, slave_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data_ptr, len, 0x00ff);
    if (ret == HAL_OK) {
        return 0;
    }
    return 1;
}
int MPU_IIC_ReadRegister(uint8_t slave_addr, uint8_t reg_addr, uint16_t len, uint8_t *data_ptr) {
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&MPU_IIC_HANDLE, slave_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data_ptr, len, 0x00ff);
    if (ret != HAL_OK) {
        return 0;
    }
    return 1;
}
