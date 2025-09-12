/* freertos.c - Full corrected version for testing */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"
#include "spi.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* ================= Modes ================= */
typedef enum
{
    MANUAL_MODE = 0,
    AUTOMATIC_MODE,
} Mode_Selection_t;

Mode_Selection_t DefaultMode = MANUAL_MODE;

/* ================= Application variables ================= */
uint32_t Height_Variable  = 2;   // cm
uint32_t Slide_Variable   = 3;   // cm
uint32_t Incline_Variable = 67;  // degrees (approx)

/* ================= Task Handles ================= */
osThreadId_t SwitchModeHandle;
osThreadId_t AdcHeightHandle;
osThreadId_t AdcSlideHandle;
osThreadId_t AdcInclineHandle;
osThreadId_t Buttons_Read_Manual_mode_high;
osThreadId_t Buttons_Read_Manual_mode_Low;
osThreadId_t UART_TaskHandle;

/* ================= Queue Handle ================= */
osMessageQueueId_t UartQueueHandle;

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
    .name = "ButtonsHighAttrib",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t Button_attrib_low = {
    .name = "ButtonsLowAttrib",
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
void Buttons_ReadTask_highmode(void * argument);
void Buttons_ReadTask_Lowmode(void * argument);
void UART_Task(void *argument);
uint32_t Read_ADC_Channel(uint32_t channel);

/* ================= FreeRTOS Init ================= */
void MX_FREERTOS_Init(void)
{
    /* Create Queue (10 messages, each up to 50 bytes) */
    UartQueueHandle = osMessageQueueNew(10, 50, NULL); // 10 entries, 50 bytes each

    /* Create Tasks */
    SwitchModeHandle               = osThreadNew(SwitchModeTask, NULL, &switchmode_attributes);
    AdcHeightHandle                = osThreadNew(AdcHeightTask, NULL, &adcHeight_attributes);
    AdcSlideHandle                 = osThreadNew(AdcSlideTask,  NULL, &adcSlide_attributes);
    AdcInclineHandle               = osThreadNew(AdcInclineTask,NULL, &adcIncline_attributes);
    Buttons_Read_Manual_mode_high  = osThreadNew(Buttons_ReadTask_highmode, NULL, &Buttons_Attributes);
    Buttons_Read_Manual_mode_Low   = osThreadNew(Buttons_ReadTask_Lowmode, NULL, &Button_attrib_low);
    UART_TaskHandle                = osThreadNew(UART_Task, NULL, &uartTask_attributes);
}

/* ================= UART Task =================
 * Waits for messages (strings) in the queue and sends them over UART.
 */
void UART_Task(void *argument)
{
    char msg[50];

    for(;;)
    {
        // Block until a message arrives
        if (osMessageQueueGet(UartQueueHandle, msg, NULL, osWaitForever) == osOK)
        {
            // Ensure null-terminated (snprintf used to fill messages so should be OK)
            msg[49] = '\0';
            HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        }
    }
}

/* ================= SwitchModeTask =================
 * Toggle between MANUAL and AUTOMATIC on button (PA3). Sends notification to UART queue.
 */
void SwitchModeTask(void *argument)
{
    char msg_auto[]   = "Switched to AUTO mode\r\n";
    char msg_manual[] = "Switched to MANUAL mode\r\n";

    for(;;)
    {
        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_SET)
        {
            // Toggle mode
            DefaultMode = (DefaultMode == MANUAL_MODE) ? AUTOMATIC_MODE : MANUAL_MODE;

            if(DefaultMode == AUTOMATIC_MODE)
                osMessageQueuePut(UartQueueHandle, msg_auto, 0, 10); // timeout 10ms to avoid blocking forever
            else
                osMessageQueuePut(UartQueueHandle, msg_manual, 0, 10);

            /* Wait for button release (debounce) */
            while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_SET) {
                osDelay(10);
            }
        }
        osDelay(50); // poll every 50 ms
    }
}

/* ================= ADC Tasks =================
 * These tasks read respective ADC channels only when in AUTOMATIC mode
 * and send the raw readings to UART via queue.
 */
void AdcHeightTask(void *argument)
{
    char msg[50];

    for(;;)
    {
        if(DefaultMode == AUTOMATIC_MODE)
        {
            uint32_t value = Read_ADC_Channel(ADC_CHANNEL_0); // PA0
            int len = snprintf(msg, sizeof(msg), "Height_raw=%lu\r\n", value);
            if (len > 0) {
                // put with short timeout to avoid blocking the task forever if queue full
                osMessageQueuePut(UartQueueHandle, msg, 0, 10);
            }
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
            int len = snprintf(msg, sizeof(msg), "Slide_raw=%lu\r\n", value);
            if (len > 0) {
                osMessageQueuePut(UartQueueHandle, msg, 0, 10);
            }
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
            int len = snprintf(msg, sizeof(msg), "Incline_raw=%lu\r\n", value);
            if (len > 0) {
                osMessageQueuePut(UartQueueHandle, msg, 0, 10);
            }
        }
        osDelay(300);
    }
}

/* ================= Buttons with Debounce (HIGH mode => increase values) ================= */
void Buttons_ReadTask_highmode(void * argument)
{
    char msg1[] = "Height Increasing\r\n";
    char msg2[] = "Slider Increasing\r\n";
    char incline_inc[] = "Incline Increase\r\n";
    char incline_inc_max[] = "Incline Increase Max\r\n";
    char msg3[] = "Maximum Height\r\n";
    char msg4[] = "Maximum Slider\r\n";

    for(;;)
    {
        if(DefaultMode == MANUAL_MODE)
        {
            // Height increase (PA4)
            if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET)
            {
                if(Height_Variable >= 5)
                {
                    osMessageQueuePut(UartQueueHandle, msg3, 0, 10);
                }
                else
                {
                    Height_Variable += 1;
                    osMessageQueuePut(UartQueueHandle, msg1, 0, 10);
                }
                // wait until release
                while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET) osDelay(10);
            }
            // Slide increase (PA5)
            else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET)
            {
                if(Slide_Variable >= 7)
                {
                    osMessageQueuePut(UartQueueHandle, msg4, 0, 10);
                }
                else
                {
                    Slide_Variable += 1;
                    osMessageQueuePut(UartQueueHandle, msg2, 0, 10);
                }
                while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET) osDelay(10);
            }
            // Incline increase (PA6)
            else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
            {
                if(Incline_Variable >= 105)
                {
                    osMessageQueuePut(UartQueueHandle, incline_inc_max, 0, 10);
                }
                else
                {
                    Incline_Variable += 1;
                    osMessageQueuePut(UartQueueHandle, incline_inc, 0, 10);
                }
                while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET) osDelay(10);
            }

            osDelay(50); // main polling delay for manual buttons
        }
        else
        {
            osDelay(100); // if not manual, sleep longer
        }
    }
}

/* ================= Buttons with Debounce (LOW mode => decrease values) ================= */
void Buttons_ReadTask_Lowmode(void * argument)
{
    char Notification_height_dec[] = "Height Decrease\r\n";
    char Notification_height_maxdec[] = "Height Decrease Max\r\n";

    char Notification_slide_dec[] = "Slide Decrease\r\n";
    char Notification_slide_maxdec[] = "Slide Decrease Max\r\n";

    char Notification_incline_dec[] = "Incline Decrease\r\n";
    char Notification_incline_maxdec[] = "Incline Decrease Max\r\n";

    for(;;)
    {
        if (DefaultMode == MANUAL_MODE)
        {
            // Height decrease (PA7)
            if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET)
            {
                if(Height_Variable <= 2)
                {
                    osMessageQueuePut(UartQueueHandle, Notification_height_maxdec, 0, 10);
                }
                else
                {
                    Height_Variable -= 1;
                    osMessageQueuePut(UartQueueHandle, Notification_height_dec, 0, 10);
                }
                while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET) osDelay(10);
            }
            // Slide decrease (PB0)
            else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET)
            {
                if(Slide_Variable <= 3)
                {
                    osMessageQueuePut(UartQueueHandle, Notification_slide_maxdec, 0, 10);
                }
                else
                {
                    Slide_Variable -= 1;
                    osMessageQueuePut(UartQueueHandle, Notification_slide_dec, 0, 10);
                }
                while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET) osDelay(10);
            }
            // Incline decrease (PB1)
            else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
            {
                if(Incline_Variable <= 67)
                {
                    osMessageQueuePut(UartQueueHandle, Notification_incline_maxdec, 0, 10);
                }
                else
                {
                    Incline_Variable -= 1;
                    osMessageQueuePut(UartQueueHandle, Notification_incline_dec, 0, 10);
                }
                while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET) osDelay(10);
            }

            osDelay(50);
        }
        else
        {
            osDelay(100);

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
