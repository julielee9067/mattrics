/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "fatfs_sd.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define NUM_NODES_FULL 	        1824 													/* Missing 7 rows on the mat */
#define NUM_NODES_PARTIAL    	96 														/* Nodes selected for partial sampling */
#define WAIT_TIME 	         	20000 													/* Time in ms to sample mat before ending program */
#define CALIBRATION_DELAY 	 	10 														/* Time in ms between mat calibration readings */
#define CALIBRATION_CYCLES 	 	3
#define SAMPLE_CYCLES 	     	5
#define ADC_DELAY        	 	3
#define RUNTIME 	          	10000 													/* Time in ms to sample mat before ending program */
// #define BREATHING_PERIOD		500													    /* Time in ms of breathing cycle sampling period */
// #define PRESSURE_MAP_PERIOD		300000													/* Time in ms of pressure map cycle sampling period */
#define BREATHING_PERIOD		500													    /* Time in ms of breathing cycle sampling period */
#define PRESSURE_MAP_PERIOD		30000													/* Time in ms of pressure map cycle sampling period */
#define FILE_LINE_SIZE_FULL   	(9 + (4 * NUM_NODES_FULL) + NUM_NODES_FULL) 			/* Time + (ADC integer reading (4 bytes) + comma) * NUM_NODES_FULL */
#define FILE_LINE_SIZE_PARTIAL	(9 + (4 * NUM_NODES_PARTIAL) + NUM_NODES_PARTIAL) 		/* Time + (ADC integer reading (4 bytes) + comma) * NUM_NODES_PARTIAL */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* SD card-related variables */
FATFS fs;
FRESULT fr = FR_OK;
FIL pressure_map_fil;
FIL breathing_fil;
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

char date[13];
char pressure_map_file_name[30] = "pressure_map_data.csv";
char breathing_file_name[30] = "breathing_data.csv";

RTC_DateTypeDef nDate;
RTC_TimeTypeDef nTime;

/* Muxing-related variables */
/* At 3.3V, expect ADC reading of 4095 */
const float ADC_VOLTAGE_CONVERSION = 0.0008056641;

const int pwrMuxEnable[4] = {PWR_EN1_L_Pin, PWR_EN2_L_Pin, PWR_EN3_L_Pin, PWR_EN4_L_Pin};
const int senseMuxEnable[8] = {SENSE_EN1_L_Pin, SENSE_EN2_L_Pin, SENSE_EN3_L_Pin, SENSE_EN4_L_Pin, SENSE_EN5_L_Pin, SENSE_EN6_L_Pin, SENSE_EN7_L_Pin, SENSE_EN8_L_Pin};

GPIO_TypeDef * pwrMuxType[4] = {GPIOB, GPIOC, GPIOC, GPIOA};
GPIO_TypeDef * senseMuxType[8] = {GPIOC, GPIOC, GPIOB, GPIOB, GPIOB, GPIOC, GPIOC, GPIOC};

int senseMuxSelect[3] = {SENSE_S1_Pin, SENSE_S2_Pin, SENSE_S3_Pin};
int pwrMuxSelect[3] = {PWR_S1_Pin, PWR_S2_Pin, PWR_S3_Pin};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);

/* USER CODE BEGIN PFP */
/* SD card */
int _write(int file, char* ptr, int len);
void writeCurrentTime(FIL* fil);
void logData2SDCard(FIL* fil, int file_line_size, int data[], int len, bool write_timestamp);
void readSDCardSendUART(FIL* fil, char* file_name, int file_line_size);
uint32_t getSpaceFree(void);

/* Muxes */
void muxInit(void);
void selectChannel(int pin, int array[]);
void enableMux(GPIO_TypeDef *type, int pin);
void disableMux(GPIO_TypeDef *type, int pin);
int readPressure(void);
void sampleMatFull(int data[], int len);
void sampleMatPartial(int data[], int len);
void calibrateMat(int data[], int len);
void calibrate(int data[], int len);
bool checkTime(uint32_t start_time, int threshold);

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
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_USART3_UART_Init();
  MX_FATFS_Init();
  MX_RTC_Init();

  /* USER CODE BEGIN 2 */
  muxInit();

  int pressure_map_data[NUM_NODES_FULL] = {0};
  int breathing_data[NUM_NODES_PARTIAL] = {0};

  int pressure_map_calibration_data[NUM_NODES_FULL] = {0};
  int breathing_calibration_data[NUM_NODES_PARTIAL] = {0};

  uint32_t breathing_time;
  uint32_t pressure_map_time;

  /* Mount the SD card */
  fr = f_mount(&fs, "", 0) && FR_OK;
  // int cycle_cnt = 0;

  // HAL_RTC_GetDate(&hrtc, &nDate, RTC_FORMAT_BIN);
  // sprintf(date, "%02u-%02u-%02u.csv", nDate.Month, nDate.Date, nDate.Year);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /* Wait for user button press to start the program */
  while (HAL_GPIO_ReadPin(BTN_TEST_GPIO_Port, BTN_TEST_Pin) == GPIO_PIN_SET){}

  /* Set RED LED to start measuring calibration data with nothing on the mat */
  HAL_GPIO_WritePin(GPIOC, GPIO_RGB_R_Pin, GPIO_PIN_SET);

  /* Calibration for 30s */
  calibrate(pressure_map_calibration_data, sizeof(pressure_map_calibration_data)/sizeof(*pressure_map_calibration_data));

  /* Reset the RED LED after calibration */
  HAL_GPIO_WritePin(GPIOC, GPIO_RGB_R_Pin, GPIO_PIN_RESET);

  /* Wait for user to get on the mat and press the button */
  while (HAL_GPIO_ReadPin(BTN_TEST_GPIO_Port, BTN_TEST_Pin) == GPIO_PIN_SET){}

  /* Set RED LED */
  HAL_GPIO_WritePin(GPIOC, GPIO_RGB_R_Pin, GPIO_PIN_SET);

  /* Wait for reading to settle */
  HAL_Delay(WAIT_TIME);

  /* Turn off RED LED */
  HAL_GPIO_WritePin(GPIOC, GPIO_RGB_R_Pin, GPIO_PIN_RESET);

  /* Open the file to write pressure map data to */
  fr = f_open(&pressure_map_fil, pressure_map_file_name, FA_CREATE_ALWAYS | FA_WRITE) && FR_OK;

  if (fr != FR_OK)
  {
	Error_Handler();
  }

  /* Open the file to write breathing data to */
  fr = f_open(&breathing_fil, breathing_file_name, FA_CREATE_ALWAYS | FA_WRITE) && FR_OK;

  if (fr != FR_OK)
  {
	Error_Handler();
  }

  // uint16_t start_time = HAL_GetTick();
  breathing_time = HAL_GetTick();
  pressure_map_time = HAL_GetTick();

  /* Set GREEN LED */
  HAL_GPIO_WritePin(GPIOC, GPIO_RGB_G_Pin, GPIO_PIN_SET);

  /* Button press stops the code */
  while(HAL_GPIO_ReadPin(BTN_TEST_GPIO_Port, BTN_TEST_Pin) == GPIO_PIN_SET)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Toggle timing GPIO */
	// HAL_GPIO_WritePin(GPIOA, TIM_TOGG_Pin, GPIO_PIN_SET);

	if (checkTime(breathing_time, BREATHING_PERIOD))
	{
      /* Toggle timing GPIO */
      // HAL_GPIO_WritePin(GPIOA, TIM_TOGG_Pin, GPIO_PIN_SET);

      /* Update timer */
      breathing_time = HAL_GetTick();

	  /* Sample selected nodes on mat for breathing */
	  sampleMatPartial(breathing_data, sizeof(breathing_data)/sizeof(*breathing_data));

	  /* Write sampled data to SD card */
	  logData2SDCard(&breathing_fil, FILE_LINE_SIZE_PARTIAL, breathing_data, NUM_NODES_PARTIAL, true);

	  /* Toggle timing GPIO */
	  // HAL_GPIO_WritePin(GPIOA, TIM_TOGG_Pin, GPIO_PIN_RESET);
	}

	if (checkTime(pressure_map_time, PRESSURE_MAP_PERIOD))
	{
	  /* Toggle timing GPIO */
	  // HAL_GPIO_WritePin(GPIOA, TIM_TOGG_Pin, GPIO_PIN_SET);

	  /* Update timer */
	  pressure_map_time = HAL_GetTick();

	  /* Sample all nodes on mat */
	  sampleMatFull(pressure_map_data, sizeof(pressure_map_data)/sizeof(*pressure_map_data));

	  /* Write partial sampled data to SD card */
	  logData2SDCard(&pressure_map_fil, FILE_LINE_SIZE_FULL, pressure_map_data, NUM_NODES_FULL, true);

	  /* Toggle timing GPIO */
	  // HAL_GPIO_WritePin(GPIOA, TIM_TOGG_Pin, GPIO_PIN_RESET);
	}

//    /* TODO: Check timer. If pass 2 minutes, open SD card file, read data and write to UART */
//    if (checkTime(start_time, RUNTIME))
//    {
//  	  if (cycle_cnt >= SAMPLE_CYCLES)
//  	  {
//  		/* Set Green pin to indicate logging is occurring */
//		HAL_GPIO_WritePin(GPIOC, GPIO_RGB_G_Pin, GPIO_PIN_SET);
//
//		/* Write calibration data to SD card on the last row (without timestamp) */
//		logData2SDCard(pressure_map_calibration_data, NUM_NODES_FULL, false);
//
//		/* Read SD card and send data to ESP8266 via UART */
//		readSDCardSendUART();
//
//	    /* Unmount the default drive */
//  		fr = f_close(&fil);
//		fr = f_mount(0, "", 0);
//
//	    break;
//      }
//
//      cycle_cnt++;
//    }

	/* Toggle timing GPIO */
	// HAL_GPIO_WritePin(GPIOA, TIM_TOGG_Pin, GPIO_PIN_RESET);
  }

  /* Turn off GREEN LED */
  HAL_GPIO_WritePin(GPIOC, GPIO_RGB_G_Pin, GPIO_PIN_RESET);

  /* Close the files and unmount the default drive */
  fr = f_close(&pressure_map_fil);
  fr = f_close(&breathing_fil);

  fr = f_mount(0, "", 0);

  return EXIT_SUCCESS;
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */
  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */
  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = ADC_AUTOWAIT_DISABLE;
  hadc.Init.LowPowerAutoPowerOff = ADC_AUTOPOWEROFF_DISABLE;
  hadc.Init.ChannelsBank = ADC_CHANNELS_BANK_A;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_48CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */
  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */
  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */
  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */
  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 22;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_FRIDAY;
  sDate.Month = RTC_MONTH_MARCH;
  sDate.Date = 11;
  sDate.Year = 22;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 6;
  sAlarm.AlarmTime.Minutes = 0;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */
  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */
  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */
  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */
  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */
  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SENSE_EN6_L_Pin|SENSE_EN7_L_Pin|SENSE_EN8_L_Pin|PWR_MUX_IN_Pin
                          |SENSE_EN2_L_Pin|SENSE_EN1_L_Pin|PWR_EN3_L_Pin|PWR_EN2_L_Pin
                          |GPIO_RGB_B_Pin|GPIO_RGB_G_Pin|GPIO_RGB_R_Pin|WIFI_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SENSE_S1_Pin|SENSE_S2_Pin|SENSE_S3_Pin|PWR_S1_Pin
                          |PWR_S2_Pin|PWR_S3_Pin|PWR_EN4_L_Pin|TIM_TOGG_Pin
                          |MCU_PA12_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PWR_EN1_L_Pin|MCU_BOOT1_Pin|SENSE_EN3_L_Pin|SENSE_EN4_L_Pin
                          |SENSE_EN5_L_Pin|SPI_CS2_L_Pin|SD_CS_L_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SENSE_EN6_L_Pin SENSE_EN7_L_Pin SENSE_EN8_L_Pin PWR_MUX_IN_Pin
                           SENSE_EN2_L_Pin SENSE_EN1_L_Pin PWR_EN3_L_Pin PWR_EN2_L_Pin
                           GPIO_RGB_B_Pin GPIO_RGB_G_Pin GPIO_RGB_R_Pin WIFI_EN_Pin */
  GPIO_InitStruct.Pin = SENSE_EN6_L_Pin|SENSE_EN7_L_Pin|SENSE_EN8_L_Pin|PWR_MUX_IN_Pin
                          |SENSE_EN2_L_Pin|SENSE_EN1_L_Pin|PWR_EN3_L_Pin|PWR_EN2_L_Pin
                          |GPIO_RGB_B_Pin|GPIO_RGB_G_Pin|GPIO_RGB_R_Pin|WIFI_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SENSE_S1_Pin SENSE_S2_Pin SENSE_S3_Pin PWR_S1_Pin
                           PWR_S2_Pin PWR_S3_Pin PWR_EN4_L_Pin TIM_TOGG_Pin
                           MCU_PA12_Pin */
  GPIO_InitStruct.Pin = SENSE_S1_Pin|SENSE_S2_Pin|SENSE_S3_Pin|PWR_S1_Pin
                          |PWR_S2_Pin|PWR_S3_Pin|PWR_EN4_L_Pin|TIM_TOGG_Pin
                          |MCU_PA12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PWR_EN1_L_Pin MCU_BOOT1_Pin SENSE_EN3_L_Pin SENSE_EN4_L_Pin
                           SENSE_EN5_L_Pin SPI_CS2_L_Pin SD_CS_L_Pin */
  GPIO_InitStruct.Pin = PWR_EN1_L_Pin|MCU_BOOT1_Pin|SENSE_EN3_L_Pin|SENSE_EN4_L_Pin
                          |SENSE_EN5_L_Pin|SPI_CS2_L_Pin|SD_CS_L_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BTN_TEST_Pin */
  GPIO_InitStruct.Pin = BTN_TEST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_TEST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LS_FLAG_Pin */
  GPIO_InitStruct.Pin = LS_FLAG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LS_FLAG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : WIFI_RST_Pin */
  GPIO_InitStruct.Pin = WIFI_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WIFI_RST_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
  * @brief Writes the current time to the file
  * @param None
  * @retval None
  */
void writeCurrentTime(FIL* fil)
{
  char time[10];

  HAL_RTC_GetTime(&hrtc, &nTime, RTC_FORMAT_BIN);
  sprintf(time, "%02u:%02u:%02u,", nTime.Hours, nTime.Minutes, nTime.Seconds);
  f_lseek(fil, f_size(fil));
  fr = f_printf( fil, "%s", time);
}

/**
  * @brief Change _write function in stdio to be used in printf to redirect output to the SWV
  * @param file: name of file to write to
  * @param ptr: data to write
  * @param len: length of data to write
  * @retval int: length of data to write
  */
int _write(int file, char* ptr, int len)
{
  int data_index;
  for (data_index = 0; data_index < len; data_index++)
  {
	ITM_SendChar(*ptr++);
  }

  return len;
}

/**
  * @brief Write data to the SD card
  * @param data: buffer with data to write
  * @param len: length of the data to write
  * @param write_timestamp: controls if the add a time stamp to the entry at the front
  * @retval None
  */
void logData2SDCard(FIL* fil, int file_line_size, int data[], int len, bool write_timestamp)
{
  /* Make space for line of data */
  f_lseek(fil, file_line_size);
  f_lseek(fil, f_size(fil));

  if (write_timestamp)
  {
   	writeCurrentTime(fil);
  }

  /* Construct string to put into file */
  for(int node = 0; node < len - 1; node++)
  {
    fr = f_printf(fil, "%d,", data[node]) && FR_OK;
  }

  fr = f_printf(fil, "%d\n", data[len - 1]) && FR_OK;

  /* Flush file after every mat reading */
  fr = f_sync(fil) && FR_OK;

}

/**
  * @brief Read data from the SD card and send it over UART
  * @param None
  * @retval None
  */
void readSDCardSendUART(FIL* fil, char* file_name, int file_line_size)
{
  f_open(fil, file_name, FA_READ); // Data can be read from file
  char line[file_line_size]; /* Line buffer */

  int cnt = 1;

  /* Read every line*/
  while (f_gets(line, sizeof line, fil))
  {
   	if (cnt > 2)
   	{
   	  /* Skip first 2 lines of SD card because of garbage values */
      /* Send to UART */
      HAL_UART_Transmit(&huart3, (uint8_t *)line, sizeof(line), HAL_MAX_DELAY);
      HAL_Delay(2000);
    }

   	cnt++;
  }

  /* Close the file */
  fr = f_close(fil);
}

/**
  * @brief Gets space left in the SD card
  * @param None
  * @retval uint32_t: space left in SD card
  */
uint32_t getSpaceFree(void)
{
  /* TODO */
  return 1;
}


/**
  * @brief Initializes the hardware for muxing
  * @param None
  * @retval None
  */
void muxInit(void)
{
  /* Set load switch */
  HAL_GPIO_WritePin(GPIOC, PWR_MUX_IN_Pin, GPIO_PIN_SET);

  /* All muxes are active low. We want to set them high (disabled) at startup */
  for (int pwr_mux = 0; pwr_mux < 4; pwr_mux++)
  {
	disableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
  }

  for (int sense_mux = 0; sense_mux < 8; sense_mux++)
  {
	disableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
  }
}

/**
  * @brief Sets to S0, S1, and S2 select pins
  * @param pin: port bit to be written
  * @param array: GPIO information to write to the GPIO
  * @retval None
  */
void selectChannel(int pin, int array[])
{
  switch (pin)
  {
	case 0:
      HAL_GPIO_WritePin(GPIOA, array[0], GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, array[1], GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, array[2], GPIO_PIN_RESET);
	  break;
	case 1:
	  HAL_GPIO_WritePin(GPIOA, array[0], GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, array[1], GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, array[2], GPIO_PIN_RESET);
	  break;
	case 2:
      HAL_GPIO_WritePin(GPIOA, array[0], GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, array[1], GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA, array[2], GPIO_PIN_RESET);
      break;
	case 3:
	  HAL_GPIO_WritePin(GPIOA, array[0], GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, array[1], GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, array[2], GPIO_PIN_RESET);
	  break;
	case 4:
	  HAL_GPIO_WritePin(GPIOA, array[0], GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, array[1], GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, array[2], GPIO_PIN_SET);
	  break;
	case 5:
	  HAL_GPIO_WritePin(GPIOA, array[0], GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, array[1], GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, array[2], GPIO_PIN_SET);
	  break;
	case 6:
	  HAL_GPIO_WritePin(GPIOA, array[0], GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, array[1], GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, array[2], GPIO_PIN_SET);
	  break;
	case 7:
	  HAL_GPIO_WritePin(GPIOA, array[0], GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, array[1], GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, array[2], GPIO_PIN_SET);
	  break;
  }
}

/**
  * @brief Enables mux
  * @param type: GPIO type
  * @param pin: port bit to be written
  * @retval None
  */
void enableMux(GPIO_TypeDef *type, int pin)
{
  HAL_GPIO_WritePin(type,  pin,  GPIO_PIN_RESET);
}

/**
  * @brief Disables mux
  * @param  type: GPIO type
  * @param pin: port bit to be written
  * @retval None
  */
void disableMux(GPIO_TypeDef *type, int pin)
{
  HAL_GPIO_WritePin(type,  pin,  GPIO_PIN_SET);
}

/**
  * @brief Reads the ADC value
  * @param None
  * @retval int: ADC reading
  */
int readPressure(void)
{
  HAL_Delay(ADC_DELAY);
  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
  int data = HAL_ADC_GetValue(&hadc);
  HAL_ADC_Stop(&hadc);

  return data;
}

/**
  * @brief Reads the entire mat for one cycle
  * @param data: buffer to store ADC readings
  * @param len: length of the buffer
  * @retval None
  */
void sampleMatFull(int data[], int len)
{
  int array_cnt = 0;

  if (len != NUM_NODES_FULL)
  {
    /* Wrongly sized array */
    return;
  }

  for (int pwr_mux = 0; pwr_mux < 4; pwr_mux++)
  {
    enableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
	for (int pwr_sel = 0; pwr_sel < 8; pwr_sel++)
    {
	  selectChannel(pwr_sel, pwrMuxSelect);
	  for (int sense_mux = 0; sense_mux < 8; sense_mux++)
      {
	    enableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
		for (int sense_sel = 0; sense_sel < 8; sense_sel++)
        {
		  if ((sense_mux == 0) && ((sense_sel == 0 ) || (sense_sel == 1)))
		  {
		    continue;
		  }

		  if ((sense_mux == 7) && (sense_sel > 2))
		  {
			continue;
		  }

		  selectChannel(sense_sel, senseMuxSelect);

		  /* Read voltage */
		  int raw_ADC_pressure = readPressure();

		  data[array_cnt] = raw_ADC_pressure;
		  array_cnt++;
		}

		disableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
	  }
	}

	disableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
  }
}

/**
  * @brief Reads the entire mat for one cycle
  * @param data: buffer to store ADC readings
  * @param len: length of the buffer
  * @retval None
  */
void sampleMatPartial(int data[], int len)
{
  int array_cnt = 0;

  if (len != NUM_NODES_PARTIAL)
  {
    /* Wrongly sized array */
    return;
  }

  for (int pwr_mux = 1; pwr_mux < 3; pwr_mux++)
  {
    enableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
	for (int pwr_sel = 0; pwr_sel < 8; pwr_sel += 2)
    {
	  selectChannel(pwr_sel, pwrMuxSelect);
	  for (int sense_mux = 1; sense_mux < 4; sense_mux++)
      {
	    enableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
		for (int sense_sel = 0; sense_sel < 8; sense_sel += 2)
        {
		  selectChannel(sense_sel, senseMuxSelect);

		  /* Read voltage */
		  int raw_ADC_pressure = readPressure();

		  data[array_cnt] = raw_ADC_pressure;
		  array_cnt++;
		}

		disableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
	  }
	}

	disableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
  }
}

/**
  * @brief Calibrates the mat by offsetting by the mean of ambient readings
  * @param data: buffer to store ADC readings
  * @param len: length of buffer
  * @retval None
  */
void calibrateMat(int data[], int len)
{
  int array_cnt = 0;

  if (len != NUM_NODES_FULL)
  {
    /* Wrongly sized array */
    return;
  }

  for (int pwr_mux = 0; pwr_mux < 4; pwr_mux++)
  {
	enableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
	for (int pwr_sel = 0; pwr_sel < 8; pwr_sel++)
    {
	  selectChannel(pwr_sel, pwrMuxSelect);
	  for (int sense_mux = 0; sense_mux < 8; sense_mux++)
      {
	    enableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
		for (int sense_sel = 0; sense_sel < 8; sense_sel++)
        {
		  if ((sense_mux == 0) && ((sense_sel == 0 ) || (sense_sel == 1)))
		  {
		    continue;
          }

		  if ((sense_mux == 7) && (sense_sel > 2))
          {
		    continue;
		  }

		  selectChannel(sense_sel, senseMuxSelect);

		  /* Read voltage */
		  int raw_ADC_pressure = readPressure();
		  data[array_cnt] += raw_ADC_pressure;
		  array_cnt++;
		}

	  disableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
	  }
	}

  disableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
  }
}

/**
  * @brief Calibrates the mat
  * @param
  * @retval
  */
void calibrate(int data[], int len)
{
  /* Calibrate over CALIBRATION_CYCLES number of mat readings - Don't use time based calibration in case of overflow */
  for(int round = 0; round < CALIBRATION_CYCLES; round++)
  {
    /* Sum x rounds for each point */
    calibrateMat(data, len);
  }

  /* Taking the mean */
  for (int i = 0; i < len; i++)
  {
	/* Take mean of each node over x rounds */
	data[i] = round(data[i]/CALIBRATION_CYCLES);
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
bool checkTime(uint32_t start_time, int threshold)
{
  if((HAL_GetTick() - start_time) >= threshold)
  {
	return true;
  }

  return false;
}

/* USER CODE END 4 */

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
