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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "syslog.h"
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
#define HALL_INPUT_1_Pin GPIO_PIN_0
#define HALL_INPUT_1_GPIO_Port GPIOA
#define HALL_INPUT_2_Pin GPIO_PIN_1
#define HALL_INPUT_2_GPIO_Port GPIOA
#define HALL_INPUT_3_Pin GPIO_PIN_2
#define HALL_INPUT_3_GPIO_Port GPIOA
#define APP_BOOT_Pin GPIO_PIN_15
#define APP_BOOT_GPIO_Port GPIOE
#define APP_BOOT_EXTI_IRQn EXTI15_10_IRQn
#define LD1_Pin GPIO_PIN_11
#define LD1_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_15
#define LD2_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
