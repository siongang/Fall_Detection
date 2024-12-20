/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "string.h"
#include "lsm9ds1_read_data_polling.h"
#include "math.h"
#include "stm32f4xx_hal.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define Flash_Address_Start  0x08040000 // for flash storage


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

  uint8_t count = 0;
  float accel[3];
  float gyro[3];
  float mag[3];

  float accel_x;
  float accel_y;
  float accel_z;

  float gyro_x;
  float gyro_y;
  float gyro_z;

  float mag_x;
  float mag_y;
  float mag_z;

  float svm;

  int data_num = 500;
  int feature_num = 11;
  int32_t write_data = 100;

  int row = 0;
  int col = 0;

  float time_stamp = 0;
  uint32_t time_elapsed;



  GPIO_PinState button_state;
  uint8_t fall_detected = 0;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Printf
int _write(int file, char *ptr, int len) {
	for (int i = 0; i < len; i++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}



void EraseFlash(void) {
	printf("erasing memory\n");

	FLASH_EraseInitTypeDef FlashEraseDefinition;

	uint32_t FlashEraseFault=0;
	HAL_FLASH_Unlock();
	printf("unlocked hal flash");

	FlashEraseDefinition.TypeErase = FLASH_TYPEERASE_SECTORS;
	FlashEraseDefinition.Banks = FLASH_BANK_1;
	FlashEraseDefinition.NbSectors = 1;
	FlashEraseDefinition.Sector = FLASH_SECTOR_6;
	FlashEraseDefinition.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	HAL_StatusTypeDef eraseStatus = HAL_FLASHEx_Erase(&FlashEraseDefinition, &FlashEraseFault);
    if (eraseStatus != HAL_OK) {
        // Handle error (e.g., log or return error code)
    	printf("Erase did not work\n");
    }
	HAL_FLASH_Lock();
	printf("locked hal flash\n");
}


// writing float data
// Address: 4 bytes
void WriteData(int32_t Data, uint32_t Address) {

	HAL_StatusTypeDef status;

    if (Address % 4 != 0) {
    	printf("Address is not 4 bytes");
    	// Handle error: address not 4-byte aligned
        return;
    }

	HAL_FLASH_Unlock();
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (volatile)Address, Data);

	if (status != HAL_OK) {
	   printf("Write data did not work\n");
		// Handle error (e.g., log or return error code)
	}

	HAL_FLASH_Lock();
}



int32_t RetrieveData(uint32_t Address) {
	return *((int32_t*)Address);

//	float recoveredFloat = *((float*)&Data);
//	return recoveredFloat;
}




void write_data_loop() {

	printf("Starting to Erase Flash\n");
	EraseFlash();
	printf("Finished erasing sector 6 memory\n");

	HAL_TIM_Base_Start(&htim11);

	uint32_t current_address = Flash_Address_Start;

	for(int i = 0 ; i < 100 ;i++ )
	{

	  __HAL_TIM_SET_COUNTER(&htim11, 0);


	  lsm9ds1_read(accel,gyro,mag);
	  accel_x = accel[0];
	  accel_y = accel[1];
	  accel_z = accel[2];

	  gyro_x = gyro[0];
	  gyro_y = gyro[1];
	  gyro_z = gyro[2];

	  mag_x = mag[0];
	  mag_y = mag[1];
	  mag_z = mag[2];


	  svm = sqrt(pow(accel_x,2)+pow(accel_y,2)+pow(accel_z,2));

	  current_address+=4;
	  WriteData((int32_t)accel_x, current_address);
	  current_address+=4;
	  WriteData((int32_t)accel_y, current_address);
	  current_address+=4;
	  WriteData((int32_t)accel_z, current_address);
	  current_address+=4;

	  WriteData((int32_t)gyro_x, current_address);
	  current_address+=4;
	  WriteData((int32_t)gyro_y, current_address);
	  current_address+=4;
	  WriteData((int32_t)gyro_z, current_address);
	  current_address+=4;

	  WriteData((int32_t)mag_x, current_address);
	  current_address+=4;
	  WriteData((int32_t)mag_y, current_address);
	  current_address+=4;
	  WriteData((int32_t)mag_z, current_address);
	  current_address+=4;

	  WriteData((int32_t)svm, current_address);
//	  current_address+=4;



	  HAL_Delay(300);

	  time_elapsed = __HAL_TIM_GET_COUNTER(&htim11); // timer
	  time_stamp = time_stamp + (float)(time_elapsed/1000000.0); // divided by 1 million to get seconds

	  current_address -= 4*10;
	  WriteData((int32_t)svm, current_address);
	  current_address += 4*10;
	  current_address += 4;

	  printf("time stamp: %f, time_elapsed: %f\n", time_stamp, (float)time_elapsed);



	  }
}


// is it here??
void read_data_loop() {

	uint32_t current_address = Flash_Address_Start;



//	FILE *file = fopen("Sensor_Reading_Data.txt", "w");
//
//
//	// Check if the file was opened successfully
//	if (file == NULL) {
//	  printf("Error opening file.\n");
//	  return;  // Return an error code
//	}
//
//
//
//	fclose(file);
//	fprintf(file, "Time(s) acc_x(g) acc_y(g) acc_z(g) gyr_x(°/s) gyr_y(°/s) gyr_z(°/s) mag_x(G) mag_y(G) mag_z(G) SVM(g)\n");

	for (int m = 0; m < 100; m++) {
		for (int k = 0; k < 11; k++) {
			int32_t read_data = RetrieveData(current_address);
			float read_data_f = (float) (read_data);
			if (k == 10) {
				printf("%f\n", read_data_f); //123  for ending
			} else {
				printf("%f;", read_data_f); //123;
			}
		}
	}
//	fclose(file);
}

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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM11_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  lsm9ds1_read_data_polling_init();
  printf("finished init, going to data polling\n");


  uint8_t message;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 1); // Turn alert status LED  ON
                       // 0.5-second delay
  printf("turning led on\n");



	  while(1) {
		  fall_detected = 1;

		  button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12); // Assuming button on PC13



		  lsm9ds1_read(accel,gyro,mag);
		  accel_x = accel[0];
		  accel_y = accel[1];
		  accel_z = accel[2];

		  gyro_x = gyro[0];
		  gyro_y = gyro[1];
		  gyro_z = gyro[2];

		  mag_x = mag[0];
		  mag_y = mag[1];
		  mag_z = mag[2];

//		  svm = sqrt(pow(accel_x,2)+pow(accel_y,2)+pow(accel_z,2));




		  if (gyro_y < -60 && accel_x < 0.88) { // falling
			  fall_detected = 0;

			                                   // 1-second delay

		  }




		  // Check conditions
		  if (fall_detected == 0|| button_state == GPIO_PIN_RESET) { // Active low button
			  printf("button pressed\n");
			  printf("USER HAS FALLEN");
			  HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 0); // Turn LED on
			  message = 1; // Send '1' if either condition is true
		  } else {
			  HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 1); // Turn LED off
			  message = 0; // Send '0' otherwise
		  }

		  // Transmit message to slave
		  HAL_UART_Transmit(&huart1, &message, 1, 100);


	//		/* Print IMU data to console */
	//		printf("IMU - Accel [g]: %.2f, %.2f, %.2f | ang [dps]: %.2f, %.2f, %.2f | mag: %.2f, %.2f, %.2f\n",
	//			 accel_x, accel_y, accel_z,
	//			 gyro_x, gyro_y, gyro_z,
	//			 mag_x,mag_y,mag_z
	//		);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  }



	//  write_data_loop();
	//  read_data_loop();
	  printf("done main loop");
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
