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
// #define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define NUM_NODES 	         36 // 6 by 6 prototype mat
#define WAITTIME 	         30000 // time in seconds to sample mat before ending program
#define CALIBRATION_TIME 	 10000 // time in seconds to calibrate mat
#define CALIBRATION_DELAY 	 10 // time in milliseconds between mat callibration readings
#define UART_BUF_SIZE 	     NUM_NODES*5 // 4 byte for each node + comma
#define FILE_LINE_SIZE       5 * NUM_NODES// (9 + (4 * NUM_NODES) + NUM_NODES)
#define VOLTAGE_THRESH       2.0
#define VOLTAGE_THRESH_CNT   5

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
FIL fil;
FRESULT fr = FR_OK;     /* FatFs return code */
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

char date[13];
char file_name[30] = "struct1_210g.csv";
RTC_DateTypeDef nDate;
RTC_TimeTypeDef nTime;

/* Wifi-related variables */
char buffer[100];

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
void writeCurrentTime(void);
void logData2SDCard(int *data, int len);
void readSDCardSendUART();
uint32_t getSpaceFree(void);


/* Muxes */
void muxInit(void);
void selectChannel(int pin, int array[]);
void enableMux(GPIO_TypeDef *type, int pin);
void disableMux(GPIO_TypeDef *type, int pin);
int readPressure(void);
bool checkTime(uint32_t start_time);
void samplePrototypeMat(int pwr_mux, int sense_mux, int* data);
int sampleSingleSquare(int pwr_mux, int sense_mux, int pwr_sel, int sense_sel);
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


  int pressure_data[NUM_NODES] = {0};
//
//    /* Mount the SD card */
  fr = f_mount(&fs, "", 0) && FR_OK;


//  /* USER CODE END 2 */
//
//  /* Infinite loop */
//  /* USER CODE BEGIN WHILE */
//


  int cycle_cnt = 0;


    // Wait for Button press
    while (HAL_GPIO_ReadPin(BTN_TEST_GPIO_Port, BTN_TEST_Pin) == GPIO_PIN_SET){}
//
//    // Start time for 30 seconds to settle
    HAL_GPIO_WritePin(GPIOC, GPIO_RGB_R_Pin, GPIO_PIN_SET);
    HAL_Delay(WAITTIME);
    HAL_GPIO_WritePin(GPIOC, GPIO_RGB_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_RGB_B_Pin, GPIO_PIN_SET);

    /*Open the file*/
    fr = f_open(&fil, file_name, FA_CREATE_ALWAYS | FA_WRITE) && FR_OK;

    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    	if (fr != FR_OK) {
    		Error_Handler();
    	}

//    	sampleSingleSquare(0,0,0,0);
    	samplePrototypeMat(0, 0, pressure_data);

//      /* Write to SD card */
    	logData2SDCard(pressure_data, sizeof(pressure_data)/sizeof(*pressure_data));
    	HAL_Delay(50);


      if (cycle_cnt >= 15) {
  		HAL_GPIO_WritePin(GPIOC, GPIO_RGB_B_Pin, GPIO_PIN_RESET);

		// Read SD card and send data to ESP8266 via UART
//		readSDCardSendUART();

	    /* Unmount the default drive */
  		fr = f_close(&fil);
		fr = f_mount(0, "", 0);

	    exit(0);
      }

      cycle_cnt++;
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
                          |PWR_S2_Pin|PWR_S3_Pin|PWR_EN4_L_Pin|MCU_PA12_Pin, GPIO_PIN_RESET);

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
                           PWR_S2_Pin PWR_S3_Pin PWR_EN4_L_Pin MCU_PA12_Pin */
  GPIO_InitStruct.Pin = SENSE_S1_Pin|SENSE_S2_Pin|SENSE_S3_Pin|PWR_S1_Pin
                          |PWR_S2_Pin|PWR_S3_Pin|PWR_EN4_L_Pin|MCU_PA12_Pin;
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
    * @brief
    * @param  :
    * @retval :
    */
void writeCurrentTime(void)
{
    char time[10];

    HAL_RTC_GetTime(&hrtc, &nTime, RTC_FORMAT_BIN);
    sprintf(time, "%02u:%02u:%02u,", nTime.Hours, nTime.Minutes, nTime.Seconds);
    f_lseek(&fil, f_size(&fil));
    fr = f_printf( &fil, "%s", time);
}

/**
    * @brief  Change _write function in stdio to be used in printf to redirect output to the SWV.
    * @param  file: name of file to write to
    * @param  ptr: data to write
    * @param len: length of data to write
    * @retval len: length of data to write
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
    * @brief
    * @param  :
    * @retval :
    */
void logData2SDCard(int *data, int len)
{
	/* Make space for line of data */
    f_lseek(&fil, FILE_LINE_SIZE);
    f_lseek(&fil, f_size(&fil));

	/* Construct string to put into file */
    for(int node = 0; node < len - 1; node++)
    {
        fr = f_printf(&fil, "%d,", data[node]) && FR_OK;
    }

    fr = f_printf(&fil, "%d\n", data[len - 1]) && FR_OK;

    // Flush file after every mat reading
    fr = f_sync(&fil) && FR_OK;
}

void readSDCardSendUART() {

    f_open(&fil, file_name, FA_READ); // Data can be read from file
    char line[FILE_LINE_SIZE]; /* Line buffer */

    /*Read every line*/
    while (f_gets(line, sizeof line, &fil)) {
    	// Send to UART
    	HAL_UART_Transmit(&huart3, (uint16_t *)line, sizeof(line), HAL_MAX_DELAY);
    	HAL_Delay(100);
    }

	/* Close the file */
	fr = f_close(&fil);

}

uint32_t getSpaceFree(void)
{
	// TODO
	return 1;
}


/**
    * @brief
    * @param  :
    * @retval :
    */
void muxInit(void) {
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
    * @brief  Sets to S0, S1, and S2 select pins
    */
void selectChannel(int pin, int array[]) {
	switch (pin) {
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
    * @brief
    * @param  :
    * @retval :
    */
void enableMux(GPIO_TypeDef *type, int pin)
{
	HAL_GPIO_WritePin(type,  pin,  GPIO_PIN_RESET);
}

/**
    * @brief
    * @param  :
    * @retval :
    */
void disableMux(GPIO_TypeDef *type, int pin)
{
	HAL_GPIO_WritePin(type,  pin,  GPIO_PIN_SET);
}


/**
    * @brief  :
    * @param  :
    * @retval :
    */
int readPressure(void)
{
	HAL_Delay(3);
	HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
    int data = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);
    return data;
}



/**
    * @brief  :
    * @param  :
    * @retval :
    */
void samplePrototypeMat(int pwr_mux, int sense_mux, int* data) {
	int array_cnt = 0;
	enableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
	enableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);

	for (int pwr_sel = 0; pwr_sel < 6; pwr_sel++) {
		selectChannel(pwr_sel, pwrMuxSelect);
		for (int sense_sel = 0; sense_sel < 6; sense_sel++) {

			selectChannel(sense_sel, senseMuxSelect);

			/* Read voltage */
			int raw_ADC_pressure = readPressure();

			data[array_cnt] = raw_ADC_pressure;
			array_cnt++;
		}
	}

}

/**
    * @brief  :
    * @param  :
    * @retval :
    */
int sampleSingleSquare(int pwr_mux, int sense_mux, int pwr_sel, int sense_sel)
{

	enableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
	enableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);

	selectChannel(pwr_sel, pwrMuxSelect);
	selectChannel(sense_sel, senseMuxSelect);

	/* Read voltage */
	int raw_ADC_pressure = readPressure();

	return raw_ADC_pressure;

}
bool checkTime(uint32_t start_time) {

  if((HAL_GetTick() - start_time) >= WAITTIME) // Run for 30 seconds
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
		HAL_GPIO_WritePin(GPIOC, GPIO_RGB_R_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOC, GPIO_RGB_R_Pin, GPIO_PIN_RESET);
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

