/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sht3x.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RECEIVED_MESSAGE_SIZE 512	// Max size for ESP message
#define IP_ADDRESS "192.168.1.249"	// Server IP Address
#define PORT "9999"					// Server Port number
// AT command for connecting to server
#define SERVER_CONNECT "AT+CIPSTART=\"TCP\",\""IP_ADDRESS"\","PORT"\r\n"
#define WIFI_SSID "Telekom-2D6325"	// Local WIFI SSID
#define WIFI_PASS "4njteenm6s7cx4cb"// Local WIFI password
// AT command for connecting to wifi
#define WIFI_CONNECT "AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASS"\"\r\n"
// Event flag
#define EVENT_FLAG1 0x00000001U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c4;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SendDataWithESP */
osThreadId_t SendDataWithESPHandle;
uint32_t SendStringBuffer[ 128 ];
osStaticThreadDef_t SendStringControlBlock;
const osThreadAttr_t SendDataWithESP_attributes = {
  .name = "SendDataWithESP",
  .cb_mem = &SendStringControlBlock,
  .cb_size = sizeof(SendStringControlBlock),
  .stack_mem = &SendStringBuffer[0],
  .stack_size = sizeof(SendStringBuffer),
  .priority = (osPriority_t) osPriorityRealtime5,
};
/* Definitions for SetUpESP */
osThreadId_t SetUpESPHandle;
uint32_t FSMBuffer[ 128 ];
osStaticThreadDef_t FSMControlBlock;
const osThreadAttr_t SetUpESP_attributes = {
  .name = "SetUpESP",
  .cb_mem = &FSMControlBlock,
  .cb_size = sizeof(FSMControlBlock),
  .stack_mem = &FSMBuffer[0],
  .stack_size = sizeof(FSMBuffer),
  .priority = (osPriority_t) osPriorityRealtime6,
};
/* Definitions for ReadSensorData */
osThreadId_t ReadSensorDataHandle;
uint32_t readDataBuffer[ 128 ];
osStaticThreadDef_t readDataControlBlock;
const osThreadAttr_t ReadSensorData_attributes = {
  .name = "ReadSensorData",
  .cb_mem = &readDataControlBlock,
  .cb_size = sizeof(readDataControlBlock),
  .stack_mem = &readDataBuffer[0],
  .stack_size = sizeof(readDataBuffer),
  .priority = (osPriority_t) osPriorityRealtime4,
};
/* Definitions for ReadESP */
osThreadId_t ReadESPHandle;
uint32_t ReadUartBuffer[ 128 ];
osStaticThreadDef_t ReadUartControlBlock;
const osThreadAttr_t ReadESP_attributes = {
  .name = "ReadESP",
  .cb_mem = &ReadUartControlBlock,
  .cb_size = sizeof(ReadUartControlBlock),
  .stack_mem = &ReadUartBuffer[0],
  .stack_size = sizeof(ReadUartBuffer),
  .priority = (osPriority_t) osPriorityRealtime7,
};
/* Definitions for queueTempAndHumid */
osMessageQueueId_t queueTempAndHumidHandle;
const osMessageQueueAttr_t queueTempAndHumid_attributes = {
  .name = "queueTempAndHumid"
};
/* Definitions for queueRxDataChar */
osMessageQueueId_t queueRxDataCharHandle;
const osMessageQueueAttr_t queueRxDataChar_attributes = {
  .name = "queueRxDataChar"
};
/* Definitions for semaphoreHaltUntilString */
osSemaphoreId_t semaphoreHaltUntilStringHandle;
const osSemaphoreAttr_t semaphoreHaltUntilString_attributes = {
  .name = "semaphoreHaltUntilString"
};
/* Definitions for semaphoreESPResponseValid */
osSemaphoreId_t semaphoreESPResponseValidHandle;
const osSemaphoreAttr_t semaphoreESPResponseValid_attributes = {
  .name = "semaphoreESPResponseValid"
};
/* Definitions for eventESPSetUpFinished */
osEventFlagsId_t eventESPSetUpFinishedHandle;
const osEventFlagsAttr_t eventESPSetUpFinished_attributes = {
  .name = "eventESPSetUpFinished"
};
/* USER CODE BEGIN PV */
// Sensor handle
sht3x_handle_t sht31 = {&hi2c4, SHT3X_I2C_DEVICE_ADDRESS_ADDR_PIN_LOW};
enum state {	STATE_CONNECT_WIFI, STATE_CONNECT_SERVER, STATE_SEND_CIPSEND, STATE_SEND_DATA,
				STATE_SEND_ATE0, STATE_READY, STATE_ERROR, STATE_CONFIG};
enum state currentState = STATE_READY;

uint8_t received_message[RECEIVED_MESSAGE_SIZE] = {'\0'};
uint8_t rx_buffer;	// Global variable for UART callback func.
uint8_t expectedESPResponse[64] = {'\0'};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C4_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void prvTaskSendDataWithESP(void *argument);
void prvTaskSetUpESP(void *argument);
void prvTaskReadTempAndHumidity(void *argument);
void prvTaskReadESP(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C4_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of semaphoreHaltUntilString */
  semaphoreHaltUntilStringHandle = osSemaphoreNew(1, 1, &semaphoreHaltUntilString_attributes);

  /* creation of semaphoreESPResponseValid */
  semaphoreESPResponseValidHandle = osSemaphoreNew(1, 1, &semaphoreESPResponseValid_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of queueTempAndHumid */
  queueTempAndHumidHandle = osMessageQueueNew (16, sizeof(float), &queueTempAndHumid_attributes);

  /* creation of queueRxDataChar */
  queueRxDataCharHandle = osMessageQueueNew (512, sizeof(uint8_t), &queueRxDataChar_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of SendDataWithESP */
  SendDataWithESPHandle = osThreadNew(prvTaskSendDataWithESP, NULL, &SendDataWithESP_attributes);

  /* creation of SetUpESP */
  SetUpESPHandle = osThreadNew(prvTaskSetUpESP, NULL, &SetUpESP_attributes);

  /* creation of ReadSensorData */
  ReadSensorDataHandle = osThreadNew(prvTaskReadTempAndHumidity, NULL, &ReadSensorData_attributes);

  /* creation of ReadESP */
  ReadESPHandle = osThreadNew(prvTaskReadESP, NULL, &ReadESP_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of eventESPSetUpFinished */
  eventESPSetUpFinishedHandle = osEventFlagsNew(&eventESPSetUpFinished_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  /* USER CODE BEGIN I2C4_Init 1 */

  /* USER CODE END I2C4_Init 1 */
  hi2c4.Instance = I2C4;
  hi2c4.Init.Timing = 0x00707CBB;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C4_Init 2 */

  /* USER CODE END I2C4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  HAL_UART_Receive_IT(&huart2, &rx_buffer, 1);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, RST_Pin|GP0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GP2_Pin|CHE_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : RST_Pin GP0_Pin */
  GPIO_InitStruct.Pin = RST_Pin|GP0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : GP2_Pin CHE_Pin */
  GPIO_InitStruct.Pin = GP2_Pin|CHE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	// Store new data in received_message buffer
	osMessageQueuePut(queueRxDataCharHandle, &rx_buffer, 0, 0);
	// Re-enable IRQ
	HAL_UART_Receive_IT(&huart2, &rx_buffer,  1);
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_prvTaskSendDataWithESP */
/**
* @brief Function implementing the SendDataWithESP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_prvTaskSendDataWithESP */
void prvTaskSendDataWithESP(void *argument)
{
  /* USER CODE BEGIN prvTaskSendDataWithESP */
  /* Infinite loop */
	// Wait for ESP setup to finish
	osEventFlagsWait(eventESPSetUpFinishedHandle, EVENT_FLAG1,  osFlagsWaitAny, osWaitForever);

	float temperature, humidity;

    char dataString[14];  // -40<temp<125, 00<humid<100 => max 13 char + '\0'
    char cipsendString[18];
  for(;;)
  {
	// Get data
    osMessageQueueGet(queueTempAndHumidHandle, &temperature, 0, osWaitForever);
    osMessageQueueGet(queueTempAndHumidHandle, &humidity, 0, osWaitForever);

    // Format data into string
    snprintf(dataString, sizeof(dataString), "%.2f %.2fq", temperature, humidity);

    // Send CIPSEND to ESP
    snprintf(cipsendString, sizeof(cipsendString), "AT+CIPSEND=%d\r\n", strlen(dataString));
    HAL_UART_Transmit(&huart2, (uint8_t*)cipsendString, strlen(cipsendString), HAL_MAX_DELAY);

    // Set expected response
    osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
    strcpy((char*) expectedESPResponse,"OK\r\n");
    osSemaphoreRelease(semaphoreHaltUntilStringHandle);

    // Response valid
    osSemaphoreAcquire(semaphoreESPResponseValidHandle, osWaitForever);

    // Send data
    HAL_UART_Transmit(&huart2, (uint8_t*)dataString, strlen(dataString), HAL_MAX_DELAY);

    // Set expected response
    osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
    strcpy((char*) expectedESPResponse,"OK\r\n");
    osSemaphoreRelease(semaphoreHaltUntilStringHandle);
  }
  /* USER CODE END prvTaskSendDataWithESP */
}

/* USER CODE BEGIN Header_prvTaskSetUpESP */
/**
* @brief Function implementing the SetUpESP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_prvTaskSetUpESP */
void prvTaskSetUpESP(void *argument)
{
  /* USER CODE BEGIN prvTaskSetUpESP */
  /* Infinite loop */
  for(;;)
  {
	  // Halts threads until setup complete


	  // Wait for ESP to set up
	    // Set expected response
	    osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
	    strcpy((char*) expectedESPResponse,"ready\r\n");
	    osSemaphoreRelease(semaphoreHaltUntilStringHandle);

	    // Response valid
	    osSemaphoreAcquire(semaphoreESPResponseValidHandle, osWaitForever);


	  // Disable echo mode on ESP
	    // Send CMD to ESP
	    HAL_UART_Transmit(&huart2, (uint8_t*)"ATE0\r\n", strlen("ATE0\r\n"), HAL_MAX_DELAY);

	    // Set expected response
	    osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
	    strcpy((char*) expectedESPResponse,"OK\r\n");
	    osSemaphoreRelease(semaphoreHaltUntilStringHandle);

	    // Response valid
	    osSemaphoreAcquire(semaphoreESPResponseValidHandle, osWaitForever);


	  // Configure ESP so it is able to connect to wifi and server
		// Send CMD to ESP
		HAL_UART_Transmit(&huart2, (uint8_t*)"AT+CIPMODE=0\r\n", strlen("AT+CIPMODE=0\r\n"), HAL_MAX_DELAY);

		// Set expected response
		osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
		strcpy((char*) expectedESPResponse,"OK\r\n");
		osSemaphoreRelease(semaphoreHaltUntilStringHandle);

		// Response valid
		osSemaphoreAcquire(semaphoreESPResponseValidHandle, osWaitForever);

		// Send CMD to ESP
		HAL_UART_Transmit(&huart2, (uint8_t*)"AT+CWMODE=1\r\n", strlen("AT+CWMODE=1\r\n"), HAL_MAX_DELAY);

		// Set expected response
		osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
		strcpy((char*) expectedESPResponse,"OK\r\n");
		osSemaphoreRelease(semaphoreHaltUntilStringHandle);

		// Response valid
		osSemaphoreAcquire(semaphoreESPResponseValidHandle, osWaitForever);


	  // Configure ESP so it is able to connect to wifi and server
		// Send CMD to ESP
		HAL_UART_Transmit(&huart2, (uint8_t*)WIFI_CONNECT, strlen(WIFI_CONNECT), HAL_MAX_DELAY);

		// Set expected response
		osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
		strcpy((char*) expectedESPResponse,"OK\r\n");
		osSemaphoreRelease(semaphoreHaltUntilStringHandle);

		// Response valid
		osSemaphoreAcquire(semaphoreESPResponseValidHandle, osWaitForever);


	  // Configure ESP so it is able to connect to wifi and server
		// Send CMD to ESP
		HAL_UART_Transmit(&huart2, (uint8_t*)SERVER_CONNECT, strlen(SERVER_CONNECT), HAL_MAX_DELAY);

		// Set expected response
		osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
		strcpy((char*) expectedESPResponse,"OK\r\n");
		osSemaphoreRelease(semaphoreHaltUntilStringHandle);

		// Response valid
		osSemaphoreAcquire(semaphoreESPResponseValidHandle, osWaitForever);

		// Signal event
		osEventFlagsSet(eventESPSetUpFinishedHandle, EVENT_FLAG1);
		// Task has to run only once
		osThreadSuspend(SetUpESPHandle);
  }
  /* USER CODE END prvTaskSetUpESP */
}

/* USER CODE BEGIN Header_prvTaskReadTempAndHumidity */
/**
* @brief Function implementing the readData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_prvTaskReadTempAndHumidity */
void prvTaskReadTempAndHumidity(void *argument)
{
  /* USER CODE BEGIN prvTaskReadTempAndHumidity */
  /* Infinite loop */
	// Wait for ESP setup to finish
	osEventFlagsWait(eventESPSetUpFinishedHandle, EVENT_FLAG1,  osFlagsWaitAny, osWaitForever);

	float temperature;
	float humidity;
  for(;;)
  {	// Read sensor data every 1 second

	 if(sht3x_read_temperature_and_humidity(&sht31, &temperature, &humidity))	{
		 osMessageQueuePut(queueTempAndHumidHandle, &temperature, 0, osWaitForever);
		 osMessageQueuePut(queueTempAndHumidHandle, &humidity, 0, osWaitForever);
	 }

	 // Delay for 1 second
	 osDelay(1000);
  }
  /* USER CODE END prvTaskReadTempAndHumidity */
}

/* USER CODE BEGIN Header_prvTaskReadESP */
/**
* @brief Function implementing the ReadESP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_prvTaskReadESP */
void prvTaskReadESP(void *argument)
{
  /* USER CODE BEGIN prvTaskReadESP */
  /* Infinite loop */
	uint8_t rxChar = '\0';
  for(;;)
  {
	  // First, response is not valid
	  osSemaphoreAcquire(semaphoreESPResponseValidHandle,osWaitForever);

	  // Add char to message
	  osMessageQueueGet(queueRxDataCharHandle, &rxChar, 0, osWaitForever);
	  strcat((char*)received_message, (char*)&rxChar);

	  // Check if got expected response
	  osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
	  // If message is valid, signal and clear message
	  if(strstr((char*)received_message, (char*)expectedESPResponse) != NULL){
		  while(osSemaphoreRelease(semaphoreESPResponseValidHandle) != osOK){}
		  memset(received_message, '\0', strlen((char*)received_message));
	  }
	  else{
		  // TODO: if "\r\n" but not valid response => implement error counter
	  }
	  osSemaphoreRelease(semaphoreHaltUntilStringHandle);



  }
  /* USER CODE END prvTaskReadESP */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
