/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os2.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
// Used to signal screen transition coming from Home screen
extern osSemaphoreId_t semaphoreTransitionFromHomeByButtonSettingsHandle;

extern char server_ip[50];
extern char server_port[50];
extern char wifi_ssid[50];
extern char wifi_pass[50];
extern uint16_t data_frequency;

extern osEventFlagsId_t eventConfigurationsLoadedHandle;

extern osEventFlagsId_t eventESPServerConnectedHandle;

extern osEventFlagsId_t eventDisconnectHandle;

extern osMessageQueueId_t queueTouchgfxTempAndHumidHandle;
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
#define USRT_TX_Pin GPIO_PIN_5
#define USRT_TX_GPIO_Port GPIOD
#define FRAME_RATE_Pin GPIO_PIN_6
#define FRAME_RATE_GPIO_Port GPIOB
#define USRT_RX_Pin GPIO_PIN_6
#define USRT_RX_GPIO_Port GPIOD
#define RENDER_TIME_Pin GPIO_PIN_7
#define RENDER_TIME_GPIO_Port GPIOB
#define LCD_DE_Pin GPIO_PIN_7
#define LCD_DE_GPIO_Port GPIOD
#define VSYNC_FREQ_Pin GPIO_PIN_3
#define VSYNC_FREQ_GPIO_Port GPIOG
#define LCD_BL_CTRL_Pin GPIO_PIN_0
#define LCD_BL_CTRL_GPIO_Port GPIOK
#define LCD_RESET_Pin GPIO_PIN_2
#define LCD_RESET_GPIO_Port GPIOA
#define RST_Pin GPIO_PIN_10
#define RST_GPIO_Port GPIOH
#define MCU_ACTIVE_Pin GPIO_PIN_6
#define MCU_ACTIVE_GPIO_Port GPIOA
#define GP0_Pin GPIO_PIN_12
#define GP0_GPIO_Port GPIOH
#define I2C_Clock_Pin GPIO_PIN_12
#define I2C_Clock_GPIO_Port GPIOD
#define I2C_Data_Pin GPIO_PIN_13
#define I2C_Data_GPIO_Port GPIOD
#define GP2_Pin GPIO_PIN_3
#define GP2_GPIO_Port GPIOA
#define CHE_Pin GPIO_PIN_4
#define CHE_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

#define DATA_FREQUENCY "5000"		// 5000ms data frequency
#define RECEIVED_MESSAGE_SIZE 512	// Max size for ESP message
#define IP_ADDRESS "192.168.1.71"	// Server IP Address
#define PORT "9000"					// Server Port number
// AT command for connecting to server
#define SERVER_CONNECT "AT+CIPSTART=\"TCP\",\""IP_ADDRESS"\","PORT"\r\n"

#define WIFI_SSID "Telekom-072404"	// Local WIFI SSID

#define WIFI_PASS "atc7habf4xt6"// Local WIFI password

// AT command for connecting to wifi
#define WIFI_CONNECT "AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASS"\"\r\n"

// Event flag
#define EVENT_FLAG_ESP_BASIC_SETUP_FINISHED 0x00000001U
#define EVENT_FLAG_ESP_WIFI_CONNECTED 		0x00000001U
#define EVENT_FLAG_ESP_SERVER_CONNECTED 	0x00000001U
#define EVENT_FLAG_ESP_WIFI_CONNECT 		0x00000001U
#define EVENT_FLAG_ESP_SERVER_CONNECT 		0x00000010U
#define EVENT_FLAG_ESP_ERROR 				0x00000001U
#define EVENT_FLAG_ESP_RESPONSE_TIMEOUT 	0x00000010U
#define EVENT_FLAG_ESP_RESPONSE_VALID		0x00000001U
#define EVENT_FLAG_DISCONNECT_REQUEST		0x00000001U
#define EVENT_FLAG_DISCONNECT_SUCCESSFUL	0x00000010U
// Wait for event flags for 5000 ticks
#define EVENT_FLAG_WAIT 5000
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
