#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"
#include <string.h>
#include <stdio.h>

/* ================= Modes ================= */
typedef enum
{
	MANUAL_MODE = 0,
	AUTOMATIC_MODE,
} Mode_Selection_t;
Mode_Selection_t DefaultMode = MANUAL_MODE;

uint32_t Height_Variable  = 2;
uint32_t Slide_Variable   = 3;
uint32_t Incline_Variable = 67;

/* ================= Task Handles ================= */
osThreadId_t SwitchModeHandle;
osThreadId_t AdcHeightHandle;
osThreadId_t AdcSlideHandle;
osThreadId_t AdcInclineHandle;
osThreadId_t Buttons_Read_Manual_mode;
osThreadId_t UART_TaskHandle;

/* ================= Queue Handle ================= */
osMessageQueueId_t  UartQueueHandle;

/* ================= Task Attributes ================= */
const osThreadAttr_t switchmode_attributes = {
		.name = "SwitchModeTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityHigh,
};

const osThreadAttr_t adcHeight_attributes = {
		.name = "AdcHeightTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t adcSlide_attributes = {
		.name = "AdcSlideTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t adcIncline_attributes = {
		.name = "AdcInclineTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t Buttons_Attributes = {
		.name = "buttonsAttrib",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t uartTask_attributes = {
		.name = "UARTTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

/* ================= Function Prototypes ================= */
void SwitchModeTask(void *argument);
void AdcHeightTask(void *argument);
void AdcSlideTask(void *argument);
void AdcInclineTask(void *argument);
void Buttons_ReadTask(void * argument);
void UART_Task(void *argument);
uint32_t Read_ADC_Channel(uint32_t channel);

/* ================= FreeRTOS Init ================= */
void MX_FREERTOS_Init(void) {
	/* Create Queue (10 messages, each max 50 bytes) */
	UartQueueHandle = osMessageQueueNew(10, sizeof(char[50]), NULL);

	/* Tasks */
	SwitchModeHandle         = osThreadNew(SwitchModeTask, NULL, &switchmode_attributes);
	AdcHeightHandle          = osThreadNew(AdcHeightTask, NULL, &adcHeight_attributes);
	AdcSlideHandle           = osThreadNew(AdcSlideTask,  NULL, &adcSlide_attributes);
	AdcInclineHandle         = osThreadNew(AdcInclineTask,NULL, &adcIncline_attributes);
	Buttons_Read_Manual_mode = osThreadNew(Buttons_ReadTask, NULL, &Buttons_Attributes);
	UART_TaskHandle          = osThreadNew(UART_Task, NULL, &uartTask_attributes);
}

/* ================= UART Task ================= */
void UART_Task(void *argument)
{
	char msg[50];
	for(;;)
	{
		if (osMessageQueueGet(UartQueueHandle, &msg, NULL, osWaitForever) == osOK)
		{
			HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
		}
	}
}

/* ================= SwitchModeTask ================= */
void SwitchModeTask(void *argument)
{
	char msg_auto[]   = "Switched to AUTO mode\r\n";
	char msg_manual[] = "Switched to MANUAL mode\r\n";

	for(;;)
	{
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_SET)
		{
			DefaultMode = (DefaultMode == MANUAL_MODE) ? AUTOMATIC_MODE : MANUAL_MODE;

			if(DefaultMode == AUTOMATIC_MODE)
				osMessageQueuePut(UartQueueHandle, &msg_auto, 0, 0);
			else
				osMessageQueuePut(UartQueueHandle, &msg_manual, 0, 0);

			// Wait for button release (debounce)
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_SET)
			{
				osDelay(10);
			}
		}
		osDelay(50); // check every 50 ms
	}
}

/* ================= ADC Tasks ================= */
void AdcHeightTask(void *argument)
{
	char msg[50];
	for(;;)
	{
		if(DefaultMode == AUTOMATIC_MODE)
		{
			uint32_t value = Read_ADC_Channel(ADC_CHANNEL_0); // PA0
			snprintf(msg, sizeof(msg), "Height=%lu\r\n", value);
			osMessageQueuePut(UartQueueHandle, &msg, 0, osWaitForever);
		}
		osDelay(200);
	}
}

void AdcSlideTask(void *argument)
{
	char msg[50];
	for(;;)
	{
		if(DefaultMode == AUTOMATIC_MODE)
		{
			uint32_t value = Read_ADC_Channel(ADC_CHANNEL_1); // PA1
			snprintf(msg, sizeof(msg), "Slide=%lu\r\n", value);
			osMessageQueuePut(UartQueueHandle, &msg, 0, osWaitForever);
		}
		osDelay(250);
	}
}

void AdcInclineTask(void *argument)
{
	char msg[50];
	for(;;)
	{
		if(DefaultMode == AUTOMATIC_MODE)
		{
			uint32_t value = Read_ADC_Channel(ADC_CHANNEL_2); // PA2
			snprintf(msg, sizeof(msg), "Incline=%lu\r\n", value);
			osMessageQueuePut(UartQueueHandle, &msg, 0, osWaitForever);
		}
		osDelay(300);
	}
}

/* ================= Buttons with Debounce ================= */
void Buttons_ReadTask(void * argument)
{
	char msg_release[] = "Released Height Button\r\n";
	char msg[50];

	while(1)
	{
		if(DefaultMode == MANUAL_MODE)
		{
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET)
			{
				osDelay(20); // debounce
				if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET)
				{
					Height_Variable += 1;
					snprintf(msg, sizeof(msg), "Height_Variable = %lu\r\n ", Height_Variable);
					osMessageQueuePut(UartQueueHandle, &msg, 0, osWaitForever);

					// Wait until release
					while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET)
					{
						osDelay(10);
					}

					osMessageQueuePut(UartQueueHandle, &msg_release, 0, osWaitForever);
				}
			}
			else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)==GPIO_PIN_SET)
			{
				osDelay(20); // debounce
				if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET)
				{
					Slide_Variable += 1;
					snprintf(msg, sizeof(msg), "Slide_Variable = %lu\r\n ", Slide_Variable);
					osMessageQueuePut(UartQueueHandle, &msg, 0, osWaitForever);

					// Wait until release
					while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET)
					{
						osDelay(10);
					}

					osMessageQueuePut(UartQueueHandle, &msg_release, 0, osWaitForever);
				}
			}
			osDelay(50);
		}
	}
}

	/* ================= ADC Helper ================= */
	uint32_t Read_ADC_Channel(uint32_t channel)
	{
		ADC_ChannelConfTypeDef sConfig = {0};

		sConfig.Channel = channel;
		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

		HAL_ADC_ConfigChannel(&hadc1, &sConfig);

		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		uint32_t value = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);

		return value;
	}
