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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include <stdbool.h>
#include "UartRingbuffer.h"
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define NUM_NODES 2048
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
const char* SSID = "Cloudwifi-167-504-P";
const char* PASSWD = "CWAE1923";

// At 3.3V, expect ADC reading of 4095
const float ADC_VOLTAGE_CONVERSION = 0.0008056641;

const float VOLTAGE_THRESH = 2.0; // Calculated using voltage divider equation with 3V3 sense measuring 2.2V
const int VOLTAGE_THRESH_CNT = 5;

const int pwrMuxEnable[4] = {PWR_EN1_L_Pin, PWR_EN2_L_Pin, PWR_EN3_L_Pin, PWR_EN4_L_Pin};
GPIO_TypeDef * pwrMuxType[4] = {GPIOB, GPIOC, GPIOC, GPIOA};

int senseMuxSelect[3] = {SENSE_S1_Pin, SENSE_S2_Pin, SENSE_S3_Pin};
int pwrMuxSelect[3] = {PWR_S1_Pin, PWR_S2_Pin, PWR_S3_Pin};

const int senseMuxEnable[8] = {SENSE_EN1_L_Pin, SENSE_EN2_L_Pin, SENSE_EN3_L_Pin, SENSE_EN4_L_Pin, SENSE_EN5_L_Pin, SENSE_EN6_L_Pin, SENSE_EN7_L_Pin, SENSE_EN8_L_Pin};
GPIO_TypeDef * senseMuxType[8] = {GPIOC, GPIOC, GPIOB, GPIOB, GPIOB, GPIOC, GPIOC, GPIOC};

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


void enableMux(GPIO_TypeDef *type, int pin) {
	HAL_GPIO_WritePin(type,  pin,  GPIO_PIN_RESET);
}

void disableMux(GPIO_TypeDef *type, int pin) {
	HAL_GPIO_WritePin(type,  pin,  GPIO_PIN_SET);
}

void logSDCard(char* print_statement) {
	// TODO
}

void ADC_Select_CH3(void) { // Used for 3V3
/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  ADC_ChannelConfTypeDef sConfig = {0};

  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
	Error_Handler();
  }
}

void ADC_Select_CH9(void) { // Used for sense mux

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  ADC_ChannelConfTypeDef sConfig = {0};

  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
	Error_Handler();
  }
}

float read3V3() {
  ADC_Select_CH3();
  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
  float data = HAL_ADC_GetValue(&hadc) * ADC_VOLTAGE_CONVERSION;
  HAL_ADC_Stop(&hadc);
  HAL_Delay(100);
  return data;
}

int readPressure() {
//  ADC_Select_CH9();
  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
  int data = HAL_ADC_GetValue(&hadc);
  HAL_ADC_Stop(&hadc);
  return data;
}

bool belowVoltageThresh() {
	float voltage_value = read3V3();
	if (voltage_value < VOLTAGE_THRESH) {
		// Turn on red LED
	    HAL_GPIO_WritePin(GPIOC, GPIO_RGB_R_Pin, GPIO_PIN_SET);

		// Log error to SD card
	    // TODO
//	    logSDCard("ERROR Voltage too low");
	    return true;
	} else {
		return false;
	}
}

void wifi_init() {
	// Initialize circular buffer API for tx and rx of AT commands
	Ringbuf_init();
	char data[80];

	// Set WIFI Enable pin high
	HAL_GPIO_WritePin(GPIOC, WIFI_EN_Pin, GPIO_PIN_SET);

	Uart_sendstring("AT+RST\r\n");
	HAL_Delay(2000);

	/********* AT **********/
	Uart_flush();
	Uart_sendstring("AT\r\n");
	while(!(Wait_for("OK\r\n")));

//	/********* AT+CWMODE=1 **********/
//	Uart_flush();
//	Uart_sendstring("AT+CWMODE=1\r\n");
//	while (!(Wait_for("OK\r\n")));
//	//
//	/********* AT+CWJAP="SSID","PASSWD" **********/
//	Uart_flush();
//	sprintf (data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWD);
//	Uart_sendstring(data);
//	while (!(Wait_for("OK\r\n")));

}

void mux_init() {
	// Set load switch
	HAL_GPIO_WritePin(GPIOC, PWR_MUX_IN_Pin, GPIO_PIN_SET);

	// All muxes are active low. We want to set them high (disabled) at startup
	for (int pwr_mux = 0; pwr_mux < 4; pwr_mux++) {
		disableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
	}
	for (int sense_mux = 0; sense_mux < 8; sense_mux++) {
		disableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
	}
}

void sendToDatabase(){}

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
  ITM_Port32(31) = 1;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  // TODO: Initialize Wifi + database
//  wifi_init();
  mux_init();

  ITM_Port32(31) = 2;

//  int voltage_thresh_count = 0;
  int pressure_data[NUM_NODES] = {0};
  int array_cnt = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	read3V3();
	// Check 3.3V threshold
//	if (voltage_thresh_count > VOLTAGE_THRESH_CNT) {
//		  // TODO: Voltage too low, exiting
//		  exit(EXIT_SUCCESS);
//	}

	// TODO: setup timer to read from SD card and send to database

	for (int pwr_mux = 0; pwr_mux < 4; pwr_mux++) {
		enableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
		for (int pwr_sel = 0; pwr_sel < 8; pwr_sel++) {
			selectChannel(pwr_sel, pwrMuxSelect);

			// TODO: Timer for 3v3 checking
//			if (belowVoltageThresh()) { // ADC channel for 3v3 sense
//				voltage_thresh_count++;
//			}

			for (int sense_mux = 0; sense_mux < 8; sense_mux++) {
				enableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
				for (int sense_sel = 0; sense_sel < 8; sense_sel++) {
					selectChannel(sense_sel, senseMuxSelect);

					// Read voltage sense
					int raw_ADC_pressure = readPressure(); // ADC channel for voltage
					float test_voltage = raw_ADC_pressure * ADC_VOLTAGE_CONVERSION;
//					printf("raw pressure voltage: %d", raw_pressure_voltage);
					pressure_data[array_cnt] = raw_ADC_pressure;
					array_cnt++;
					// TODO: write to SD card
				}
				disableMux(senseMuxType[sense_mux], senseMuxEnable[sense_mux]);
			}
		}
		disableMux(pwrMuxType[pwr_mux], pwrMuxEnable[pwr_mux]);
	 }
//	printf("array cnt: %d", array_cnt);
	array_cnt = 0;
	memset(pressure_data, 0, sizeof(pressure_data));
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
// Use GCC for printf
// Avoid wasting a UART terminal just for writing
int _write(int file, char *ptr, int len) {
	int DataIdx;
	for (DataIdx = 0; DataIdx < len; DataIdx++) {
		ITM_SendChar(*ptr++);
	}
	return len;
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

