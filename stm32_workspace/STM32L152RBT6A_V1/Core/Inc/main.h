/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l1xx_hal.h"

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
#define SENSE_EN6_L_Pin GPIO_PIN_13
#define SENSE_EN6_L_GPIO_Port GPIOC
#define SENSE_EN7_L_Pin GPIO_PIN_14
#define SENSE_EN7_L_GPIO_Port GPIOC
#define SENSE_EN8_L_Pin GPIO_PIN_15
#define SENSE_EN8_L_GPIO_Port GPIOC
#define PWR_MUX_IN_Pin GPIO_PIN_0
#define PWR_MUX_IN_GPIO_Port GPIOC
#define SENSE_EN2_L_Pin GPIO_PIN_1
#define SENSE_EN2_L_GPIO_Port GPIOC
#define SENSE_EN1_L_Pin GPIO_PIN_3
#define SENSE_EN1_L_GPIO_Port GPIOC
#define SENSE_S1_Pin GPIO_PIN_0
#define SENSE_S1_GPIO_Port GPIOA
#define SENSE_S2_Pin GPIO_PIN_1
#define SENSE_S2_GPIO_Port GPIOA
#define SENSE_S3_Pin GPIO_PIN_2
#define SENSE_S3_GPIO_Port GPIOA
#define PWR_S1_Pin GPIO_PIN_4
#define PWR_S1_GPIO_Port GPIOA
#define PWR_S2_Pin GPIO_PIN_5
#define PWR_S2_GPIO_Port GPIOA
#define PWR_S3_Pin GPIO_PIN_6
#define PWR_S3_GPIO_Port GPIOA
#define PWR_EN4_L_Pin GPIO_PIN_7
#define PWR_EN4_L_GPIO_Port GPIOA
#define PWR_EN3_L_Pin GPIO_PIN_4
#define PWR_EN3_L_GPIO_Port GPIOC
#define PWR_EN2_L_Pin GPIO_PIN_5
#define PWR_EN2_L_GPIO_Port GPIOC
#define PWR_EN1_L_Pin GPIO_PIN_0
#define PWR_EN1_L_GPIO_Port GPIOB
#define SENSE_OUT_Pin GPIO_PIN_1
#define SENSE_OUT_GPIO_Port GPIOB
#define MCU_BOOT1_Pin GPIO_PIN_2
#define MCU_BOOT1_GPIO_Port GPIOB
#define SENSE_EN3_L_Pin GPIO_PIN_10
#define SENSE_EN3_L_GPIO_Port GPIOB
#define SENSE_EN4_L_Pin GPIO_PIN_11
#define SENSE_EN4_L_GPIO_Port GPIOB
#define SENSE_EN5_L_Pin GPIO_PIN_12
#define SENSE_EN5_L_GPIO_Port GPIOB
#define BTN_TEST_Pin GPIO_PIN_6
#define BTN_TEST_GPIO_Port GPIOC
#define GPIO_RGB_B_Pin GPIO_PIN_7
#define GPIO_RGB_B_GPIO_Port GPIOC
#define GPIO_RGB_G_Pin GPIO_PIN_8
#define GPIO_RGB_G_GPIO_Port GPIOC
#define GPIO_RGB_R_Pin GPIO_PIN_9
#define GPIO_RGB_R_GPIO_Port GPIOC
#define LS_FLAG_Pin GPIO_PIN_8
#define LS_FLAG_GPIO_Port GPIOA
#define TIM_TOGG_Pin GPIO_PIN_11
#define TIM_TOGG_GPIO_Port GPIOA
#define MCU_PA12_Pin GPIO_PIN_12
#define MCU_PA12_GPIO_Port GPIOA
#define UART_RX_Pin GPIO_PIN_10
#define UART_RX_GPIO_Port GPIOC
#define UART_TX_Pin GPIO_PIN_11
#define UART_TX_GPIO_Port GPIOC
#define WIFI_EN_Pin GPIO_PIN_12
#define WIFI_EN_GPIO_Port GPIOC
#define WIFI_RST_Pin GPIO_PIN_2
#define WIFI_RST_GPIO_Port GPIOD
#define SPI_CS2_L_Pin GPIO_PIN_5
#define SPI_CS2_L_GPIO_Port GPIOB
#define SD_CS_L_Pin GPIO_PIN_6
#define SD_CS_L_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_8
#define I2C_SDA_GPIO_Port GPIOB
#define I2C_SCL_Pin GPIO_PIN_9
#define I2C_SCL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
