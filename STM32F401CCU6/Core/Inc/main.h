/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM2_EncodeManually 1
#define TIM10_period 1
#define Uart1_Debug 1
#define Uart2_ESP32 1
#define I2C1_MPU 1
#define TIM1_PWMGenerate 1
#define SPI1_NRF 1
#define TIM9_MicroSecond 1
#define NRF_IRQ_Pin GPIO_PIN_1
#define NRF_IRQ_GPIO_Port GPIOA
#define MotorUpper_PWM_Pin GPIO_PIN_2
#define MotorUpper_PWM_GPIO_Port GPIOA
#define MotorUpper_Direc_Pin GPIO_PIN_3
#define MotorUpper_Direc_GPIO_Port GPIOA
#define ESP_EN_Pin GPIO_PIN_4
#define ESP_EN_GPIO_Port GPIOA
#define NRF_SCK_Pin GPIO_PIN_5
#define NRF_SCK_GPIO_Port GPIOA
#define NRF_MISO_Pin GPIO_PIN_6
#define NRF_MISO_GPIO_Port GPIOA
#define NRF_MOSI_Pin GPIO_PIN_7
#define NRF_MOSI_GPIO_Port GPIOA
#define NRF_CE_Pin GPIO_PIN_0
#define NRF_CE_GPIO_Port GPIOB
#define NRF_CSN_Pin GPIO_PIN_1
#define NRF_CSN_GPIO_Port GPIOB
#define Motor_BIN2_Pin GPIO_PIN_12
#define Motor_BIN2_GPIO_Port GPIOB
#define Motor_BIN1_Pin GPIO_PIN_13
#define Motor_BIN1_GPIO_Port GPIOB
#define Motor_STBY_Pin GPIO_PIN_14
#define Motor_STBY_GPIO_Port GPIOB
#define Motor_AIN1_Pin GPIO_PIN_15
#define Motor_AIN1_GPIO_Port GPIOB
#define Motor_AIN2_Pin GPIO_PIN_8
#define Motor_AIN2_GPIO_Port GPIOA
#define Encoder_Upper_Pin GPIO_PIN_15
#define Encoder_Upper_GPIO_Port GPIOA
#define Encoder_Lower_Pin GPIO_PIN_3
#define Encoder_Lower_GPIO_Port GPIOB
#define MPU_INT_Pin GPIO_PIN_4
#define MPU_INT_GPIO_Port GPIOB
#define MPU_AD0_Pin GPIO_PIN_5
#define MPU_AD0_GPIO_Port GPIOB
#define MPU_SCL_Pin GPIO_PIN_6
#define MPU_SCL_GPIO_Port GPIOB
#define MPU_SDA_Pin GPIO_PIN_7
#define MPU_SDA_GPIO_Port GPIOB
#define Encoder_LowerSub_Pin GPIO_PIN_8
#define Encoder_LowerSub_GPIO_Port GPIOB
#define Encoder_UpperSub_Pin GPIO_PIN_9
#define Encoder_UpperSub_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
