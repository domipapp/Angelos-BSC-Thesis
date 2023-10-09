/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "libjpeg.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32h750b_discovery_qspi.h"
#include "stm32h750b_discovery_sdram.h"

#include "sht3x.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

#include "app_touchgfx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RECEIVED_MESSAGE_SIZE 512	// Max size for ESP message
#define IP_ADDRESS "192.168.1.249"	// Server IP Address
#define PORT "9000"					// Server Port number
// AT command for connecting to server
#define SERVER_CONNECT "AT+CIPSTART=\"TCP\",\""IP_ADDRESS"\","PORT"\r\n"
#define WIFI_SSID "Telekom-072404"	// Local WIFI SSID
#define WIFI_PASS "atc7habf4xt6"// Local WIFI password
// AT command for connecting to wifi
#define WIFI_CONNECT "AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASS"\"\r\n"
// Event flag
#define EVENT_FLAG1 0x00000001U
#define EVENT_FLAG_ESP_ERROR 0x00000001U
#define EVENT_FLAG_ESP_RESPONSE_TIMEOUT 0x00000010U
// Wait for event flags for 5000 ticks
#define EVENT_FLAG_WAIT 5000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

I2C_HandleTypeDef hi2c4;

JPEG_HandleTypeDef hjpeg;
MDMA_HandleTypeDef hmdma_jpeg_infifo_th;
MDMA_HandleTypeDef hmdma_jpeg_outfifo_th;

LTDC_HandleTypeDef hltdc;

QSPI_HandleTypeDef hqspi;

UART_HandleTypeDef huart2;

SDRAM_HandleTypeDef hsdram2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GUITask */
osThreadId_t GUITaskHandle;
const osThreadAttr_t GUITask_attributes = {
  .name = "GUITask",
  .stack_size = 8096 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for videoTask */
osThreadId_t videoTaskHandle;
const osThreadAttr_t videoTask_attributes = {
  .name = "videoTask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for SendDataWithESP */
osThreadId_t SendDataWithESPHandle;
const osThreadAttr_t SendDataWithESP_attributes = {
  .name = "SendDataWithESP",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for SetUpESP */
osThreadId_t SetUpESPHandle;
const osThreadAttr_t SetUpESP_attributes = {
  .name = "SetUpESP",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ReadSensorData */
osThreadId_t ReadSensorDataHandle;
const osThreadAttr_t ReadSensorData_attributes = {
  .name = "ReadSensorData",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for ReadESP */
osThreadId_t ReadESPHandle;
const osThreadAttr_t ReadESP_attributes = {
  .name = "ReadESP",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ConnectToWifi */
osThreadId_t ConnectToWifiHandle;
const osThreadAttr_t ConnectToWifi_attributes = {
  .name = "ConnectToWifi",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ConnectToServer */
osThreadId_t ConnectToServerHandle;
const osThreadAttr_t ConnectToServer_attributes = {
  .name = "ConnectToServer",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
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
/* Definitions for semaphoreTransitionFromHome */
osSemaphoreId_t semaphoreTransitionFromHomeHandle;
const osSemaphoreAttr_t semaphoreTransitionFromHome_attributes = {
  .name = "semaphoreTransitionFromHome"
};
/* Definitions for eventESPBasicSetUpFinished */
osEventFlagsId_t eventESPBasicSetUpFinishedHandle;
const osEventFlagsAttr_t eventESPBasicSetUpFinished_attributes = {
  .name = "eventESPBasicSetUpFinished"
};
/* Definitions for eventESPResponseValid */
osEventFlagsId_t eventESPResponseValidHandle;
const osEventFlagsAttr_t eventESPResponseValid_attributes = {
  .name = "eventESPResponseValid"
};
/* Definitions for eventESPResponse */
osEventFlagsId_t eventESPResponseHandle;
const osEventFlagsAttr_t eventESPResponse_attributes = {
  .name = "eventESPResponse"
};
/* Definitions for eventConfigurationsLoaded */
osEventFlagsId_t eventConfigurationsLoadedHandle;
const osEventFlagsAttr_t eventConfigurationsLoaded_attributes = {
  .name = "eventConfigurationsLoaded"
};
/* Definitions for eventESPWifiConnected */
osEventFlagsId_t eventESPWifiConnectedHandle;
const osEventFlagsAttr_t eventESPWifiConnected_attributes = {
  .name = "eventESPWifiConnected"
};
/* Definitions for eventESPServerConnected */
osEventFlagsId_t eventESPServerConnectedHandle;
const osEventFlagsAttr_t eventESPServerConnected_attributes = {
  .name = "eventESPServerConnected"
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

char server_ip[50];
char server_port[50];
char wifi_ssid[50];
char wifi_pass[50];
uint16_t data_frequency = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Initialize(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_MDMA_Init(void);
static void MX_LTDC_Init(void);
static void MX_DMA2D_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_FMC_Init(void);
static void MX_JPEG_Init(void);
static void MX_CRC_Init(void);
static void MX_I2C4_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
extern void TouchGFX_Task(void *argument);
extern void videoTaskFunc(void *argument);
void prvTaskSendDataWithESP(void *argument);
void prvTaskSetUpESP(void *argument);
void prvTaskReadTempAndHumidity(void *argument);
void prvTaskReadESP(void *argument);
void prvTaskConnectToWifi(void *argument);
void prvTaskConnectToServer(void *argument);

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

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Explicit enabling interrupt to support debugging in CubeIDE when using external flash loader */
  __enable_irq();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_MDMA_Init();
  MX_LTDC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_LIBJPEG_Init();
  MX_JPEG_Init();
  MX_CRC_Init();
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

  /* creation of semaphoreTransitionFromHome */
  semaphoreTransitionFromHomeHandle = osSemaphoreNew(1, 1, &semaphoreTransitionFromHome_attributes);

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

  /* creation of GUITask */
  GUITaskHandle = osThreadNew(TouchGFX_Task, NULL, &GUITask_attributes);

  /* creation of videoTask */
  videoTaskHandle = osThreadNew(videoTaskFunc, NULL, &videoTask_attributes);

  /* creation of SendDataWithESP */
  SendDataWithESPHandle = osThreadNew(prvTaskSendDataWithESP, NULL, &SendDataWithESP_attributes);

  /* creation of SetUpESP */
  SetUpESPHandle = osThreadNew(prvTaskSetUpESP, NULL, &SetUpESP_attributes);

  /* creation of ReadSensorData */
  ReadSensorDataHandle = osThreadNew(prvTaskReadTempAndHumidity, NULL, &ReadSensorData_attributes);

  /* creation of ReadESP */
  ReadESPHandle = osThreadNew(prvTaskReadESP, NULL, &ReadESP_attributes);

  /* creation of ConnectToWifi */
  ConnectToWifiHandle = osThreadNew(prvTaskConnectToWifi, NULL, &ConnectToWifi_attributes);

  /* creation of ConnectToServer */
  ConnectToServerHandle = osThreadNew(prvTaskConnectToServer, NULL, &ConnectToServer_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of eventESPBasicSetUpFinished */
  eventESPBasicSetUpFinishedHandle = osEventFlagsNew(&eventESPBasicSetUpFinished_attributes);

  /* creation of eventESPResponseValid */
  eventESPResponseValidHandle = osEventFlagsNew(&eventESPResponseValid_attributes);

  /* creation of eventESPResponse */
  eventESPResponseHandle = osEventFlagsNew(&eventESPResponse_attributes);

  /* creation of eventConfigurationsLoaded */
  eventConfigurationsLoadedHandle = osEventFlagsNew(&eventConfigurationsLoaded_attributes);

  /* creation of eventESPWifiConnected */
  eventESPWifiConnectedHandle = osEventFlagsNew(&eventESPWifiConnected_attributes);

  /* creation of eventESPServerConnected */
  eventESPServerConnectedHandle = osEventFlagsNew(&eventESPServerConnected_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  // Enable UART interrupts
  HAL_UART_Receive_IT(&huart2, &rx_buffer, 1);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  // Take semaphore because transition has not occurred when setting up
  osSemaphoreAcquire(semaphoreTransitionFromHomeHandle, osWaitForever);

  MX_TouchGFX_Init();
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
  hdma2d.LayerCfg[1].ChromaSubSampling = DMA2D_NO_CSS;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

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
  hi2c4.Init.Timing = 0x10C0ECFF;
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
  * @brief JPEG Initialization Function
  * @param None
  * @retval None
  */
static void MX_JPEG_Init(void)
{

  /* USER CODE BEGIN JPEG_Init 0 */

  /* USER CODE END JPEG_Init 0 */

  /* USER CODE BEGIN JPEG_Init 1 */

  /* USER CODE END JPEG_Init 1 */
  hjpeg.Instance = JPEG;
  if (HAL_JPEG_Init(&hjpeg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN JPEG_Init 2 */

  /* USER CODE END JPEG_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 39;
  hltdc.Init.VerticalSync = 8;
  hltdc.Init.AccumulatedHBP = 42;
  hltdc.Init.AccumulatedVBP = 11;
  hltdc.Init.AccumulatedActiveW = 522;
  hltdc.Init.AccumulatedActiveH = 283;
  hltdc.Init.TotalWidth = 528;
  hltdc.Init.TotalHeigh = 285;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 272;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */
  BSP_QSPI_Init_t qspi_initParams ;
  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 3;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 26;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_ENABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */
  qspi_initParams.InterfaceMode = MT25TL01G_QPI_MODE;
  qspi_initParams.TransferRate  = MT25TL01G_DTR_TRANSFER ;
  qspi_initParams.DualFlashMode = MT25TL01G_DUALFLASH_ENABLE;
  BSP_QSPI_DeInit(0);
  if (BSP_QSPI_Init(0, &qspi_initParams) != BSP_ERROR_NONE)
  {
    Error_Handler( );
  }
  if(BSP_QSPI_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
  {
    Error_Handler( );
  }
  /* USER CODE END QUADSPI_Init 2 */

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

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable MDMA controller clock
  */
static void MX_MDMA_Init(void)
{

  /* MDMA controller clock enable */
  __HAL_RCC_MDMA_CLK_ENABLE();
  /* Local variables */

  /* MDMA interrupt initialization */
  /* MDMA_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MDMA_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(MDMA_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM2 memory initialization sequence
  */
  hsdram2.Instance = FMC_SDRAM_DEVICE;
  /* hsdram2.Init */
  hsdram2.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram2.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram2.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram2.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram2.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram2.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram2.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram2.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram2.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram2.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 5;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram2, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
  BSP_SDRAM_DeInit(0);
  if(BSP_SDRAM_Init(0) != BSP_ERROR_NONE)
  {
    Error_Handler( );
  }
  /* USER CODE END FMC_Init 2 */
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
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, FRAME_RATE_Pin|RENDER_TIME_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_DE_GPIO_Port, LCD_DE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(VSYNC_FREQ_GPIO_Port, VSYNC_FREQ_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RESET_Pin|MCU_ACTIVE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, RST_Pin|GP0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GP2_Pin|CHE_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : FRAME_RATE_Pin RENDER_TIME_Pin */
  GPIO_InitStruct.Pin = FRAME_RATE_Pin|RENDER_TIME_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_DE_Pin */
  GPIO_InitStruct.Pin = LCD_DE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_DE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : VSYNC_FREQ_Pin */
  GPIO_InitStruct.Pin = VSYNC_FREQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(VSYNC_FREQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_BL_CTRL_Pin */
  GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_RESET_Pin */
  GPIO_InitStruct.Pin = LCD_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RST_Pin GP0_Pin */
  GPIO_InitStruct.Pin = RST_Pin|GP0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : MCU_ACTIVE_Pin */
  GPIO_InitStruct.Pin = MCU_ACTIVE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(MCU_ACTIVE_GPIO_Port, &GPIO_InitStruct);

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

void my_snprintf(float temp, float humid, char* result) {

    // Check for negative values
    bool tempSign = (temp < 0) ? true : false;
    bool humidSign = (humid < 0) ? true : false;

    temp = (temp < 0) ? (-temp) : temp;
    humid = (humid < 0) ? (-humid) : humid;

    // Round temperature and humidity to two decimal places
    uint16_t temperature = (int)(roundf(temp * 100));
    uint16_t humidityValue = (int)(roundf(humid * 100));

    // Extract the integer and decimal parts for temperature
    uint8_t temperatureInteger = temperature / 100;
    uint8_t temperatureDecimal = temperature % 100;

    // Extract the integer and decimal parts for humidity
    uint8_t humidityInteger = humidityValue / 100;
    uint8_t humidityDecimal = humidityValue % 100;
    uint8_t i = -1;
    // Format the string manually
    if(tempSign)
        result[++i] = '-';
    result[++i] = (temperatureInteger / 10) + '0';
    result[++i] = (temperatureInteger % 10) + '0';
    result[++i] = '.';
    result[++i] = (temperatureDecimal / 10) + '0';
    result[++i] = (temperatureDecimal % 10) + '0';
    result[++i] = ' ';
    if(humidSign)
        result[++i] = '-';
    if(humid == 100){
        result[++i] = '1';
        result[++i] = '0';
        result[++i] = '0';
    }
    else{
    result[++i] = (humidityInteger / 10) + '0';
    result[++i] = (humidityInteger % 10) + '0';
    }
    result[++i] = '.';
    result[++i] = (humidityDecimal / 10) + '0';
    result[++i] = (humidityDecimal % 10) + '0';
    result[++i] = 'q';
    result[++i] = '\0';

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	// Store new data in received_message buffer
	if(rx_buffer == '\0')
		rx_buffer = '0';
	osMessageQueuePut(queueRxDataCharHandle, &rx_buffer, 0, 0);
	// Re-enable IRQ
	HAL_UART_Receive_IT(&huart2, &rx_buffer,  1);
}


bool send_and_recieve(char* send, char* recieve){

    // Set expected response
    osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
    strcpy((char*) expectedESPResponse, recieve);
    osSemaphoreRelease(semaphoreHaltUntilStringHandle);

    HAL_UART_Transmit(&huart2, (uint8_t*) send, strlen(send), HAL_MAX_DELAY);

    uint32_t flags = osEventFlagsWait(eventESPResponseValidHandle, EVENT_FLAG1,  osFlagsWaitAny, EVENT_FLAG_WAIT);
    if (flags & EVENT_FLAG1) {
        // Valid response received
    	return true;
    } else {
        // Timeout occurred
    	// Retry once
    	if(!(osEventFlagsGet(eventESPResponseHandle) & EVENT_FLAG_ESP_RESPONSE_TIMEOUT)){
        	osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_RESPONSE_TIMEOUT);
        	return send_and_recieve(send, recieve);
    	}

    	return false;
    }
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
	  // Restart MCU if error occurs
	  osEventFlagsWait(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR, 0, osWaitForever);
	  NVIC_SystemReset();
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
	osEventFlagsWait(eventESPServerConnectedHandle, EVENT_FLAG_ESP_SERVER_CONNECTED,  osFlagsWaitAny, osWaitForever);

	float temperature, humidity;

    static uint8_t dataString[15] = {'\0'} ;  // -40<temp<125, 00<humid<100 => max 13 char + q + '\0'
    static uint8_t cipsendString[16] = {'\0'};
  for(;;)
  {
		// Get data
	    osMessageQueueGet(queueTempAndHumidHandle, &temperature, 0, osWaitForever);
	    osMessageQueueGet(queueTempAndHumidHandle, &humidity, 0, osWaitForever);

	    // Format data into string
	    my_snprintf(temperature, humidity, (char*)dataString);

	    // Format CIPSEND
	    snprintf((char*)cipsendString, 16, "AT+CIPSEND=%d\r\n", (int)strlen((char*)dataString));

	    // Send CMD to send data
	    if(!send_and_recieve((char*) cipsendString, "OK\r\n"))
	    	osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR);

	    // Send data
	    if(!send_and_recieve((char*) dataString, "OK\r\n"))
	    	osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR);

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
	  	// Make configurations so user doesnt have to wait so long
	  	  // Restart ESP
	  	    if(!send_and_recieve("AT+RST\r\n", "ready\r\n"))
	  	    	osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR);

	  	  // Disable echo mode on ESP
	  	    if(!send_and_recieve("ATE0\r\n", "OK\r\n"))
	  	    	osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR);

	  	  // Configure ESP so it is able to connect to wifi and server
	  	    if(!send_and_recieve("AT+CIPMODE=0\r\n", "OK\r\n"))
	  	    	osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR);

	  	    if(!send_and_recieve("AT+CWMODE=1\r\n", "OK\r\n"))
	  	    	osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR);

	  	  // Signal setup finish
	  		osEventFlagsSet(eventESPBasicSetUpFinishedHandle, EVENT_FLAG1);
	  	  // Task has to run only once
	  		osThreadSuspend(SetUpESPHandle);
  }
  /* USER CODE END prvTaskSetUpESP */
}

/* USER CODE BEGIN Header_prvTaskReadTempAndHumidity */
/**
* @brief Function implementing the ReadSensorData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_prvTaskReadTempAndHumidity */
void prvTaskReadTempAndHumidity(void *argument)
{
  /* USER CODE BEGIN prvTaskReadTempAndHumidity */
  /* Infinite loop */
	// Wait for ESP setup to finish
	osEventFlagsWait(eventESPServerConnectedHandle, EVENT_FLAG_ESP_SERVER_CONNECTED,  osFlagsWaitAny, osWaitForever);

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
	uint16_t received_message_index = 0;
  for(;;)
  {


	  // Add char to message
	  osMessageQueueGet(queueRxDataCharHandle, &rxChar, 0, osWaitForever);
	  // Add data to string, string is "\0" all the way
	  received_message[received_message_index++] = rxChar; // Append the character

	  // Check if got expected response
	  osSemaphoreAcquire(semaphoreHaltUntilStringHandle, osWaitForever);
	  // If message is valid, signal and clear message
	  if(strstr((char*)received_message, (char*)expectedESPResponse) != NULL){
		  osEventFlagsSet(eventESPResponseValidHandle, EVENT_FLAG1);
		  memset(received_message, '\0', strlen((char*)received_message));
		  received_message_index = 0;

	  }
	  else{
		  // Signal if "ERROR" is recieved
		  if(strstr((char*)received_message, "ERROR\r\n") != NULL){
		  		  osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR);
		  	  }
	  }
	  osSemaphoreRelease(semaphoreHaltUntilStringHandle);



  }
  /* USER CODE END prvTaskReadESP */
}

/* USER CODE BEGIN Header_prvTaskConnectToWifi */
/**
* @brief Function implementing the ConnectToWifi thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_prvTaskConnectToWifi */
void prvTaskConnectToWifi(void *argument)
{
  /* USER CODE BEGIN prvTaskConnectToWifi */
  /* Infinite loop */
	char command[120];
	// Must not try before setup complete
	osEventFlagsWait(eventESPBasicSetUpFinishedHandle, EVENT_FLAG_ESP_BASIC_SETUP_FINISHED,  osFlagsWaitAny, osWaitForever);
  for(;;)
  {
  	  // Wait for user to connect
  	  	osEventFlagsWait(eventConfigurationsLoadedHandle, EVENT_FLAG_ESP_WIFI_CONNECT,  osFlagsWaitAny, osWaitForever);

  	  // Connect to WIFI
  	    snprintf(command, sizeof(command), "AT+CWJAP=\"%s\",\"%s\"\r\n", wifi_ssid, wifi_pass);
  	    if(!send_and_recieve(command, "OK"))
  	    	osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR);

  	    osEventFlagsSet(eventESPWifiConnectedHandle, EVENT_FLAG_ESP_WIFI_CONNECTED);
  }
  /* USER CODE END prvTaskConnectToWifi */
}

/* USER CODE BEGIN Header_prvTaskConnectToServer */
/**
* @brief Function implementing the ConnectToServer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_prvTaskConnectToServer */
void prvTaskConnectToServer(void *argument)
{
  /* USER CODE BEGIN prvTaskConnectToServer */
  /* Infinite loop */
	char command[120];
	// Must not try before setup complete
	osEventFlagsWait(eventESPBasicSetUpFinishedHandle, EVENT_FLAG_ESP_BASIC_SETUP_FINISHED,  osFlagsWaitAny, osWaitForever);
	// must wait to be connected to wifi
	osEventFlagsWait(eventESPWifiConnectedHandle, EVENT_FLAG_ESP_WIFI_CONNECTED,  osFlagsWaitAny, osWaitForever);
  for(;;)
  {
  	  // Wait for user to connect
	  	osEventFlagsWait(eventConfigurationsLoadedHandle, EVENT_FLAG_ESP_SERVER_CONNECT,  osFlagsWaitAny, osWaitForever);
  	  //Connect to server
  	    snprintf(command, sizeof(command), "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", server_ip, server_port);
  	    if(!send_and_recieve(command, "OK\r\n"))
  	    	osEventFlagsSet(eventESPResponseHandle, EVENT_FLAG_ESP_ERROR);

  	  osEventFlagsSet(eventESPServerConnectedHandle, EVENT_FLAG_ESP_SERVER_CONNECTED);
  }
  /* USER CODE END prvTaskConnectToServer */
}

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.Size = MPU_REGION_SIZE_128MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress = 0xD0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER4;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
