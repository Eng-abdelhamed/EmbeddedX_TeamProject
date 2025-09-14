#include "../MCAL/I2C.h"
#include "../MCAL/uart.h"
#include "../HAL/LED.h"
#include "../common/stdtypes.h"

#define ECU2_ADDR   0x30   // Slave address

TWI_ConfigType I2C_Config = {ECU2_ADDR, TWI_BAUD_100KHZ};
UART_ConfigType UART_Config = {UART_8BIT_MODE, even, UART_ONE_STOP_BIT, UART_BAUD_9600};

int main(void)
{
    TWI_init(&I2C_Config);     // I2C as slave
    UART_init(&UART_Config);   // UART to PC
    LEDS_init();

    uint8 received_data;

    while (1)
    {
        /* receive data from master (I2C) */
        received_data = TWI_readByteWithACK();

        /* act on LED */
        if (received_data == 1)
        {
            RED_LED_on();
        }
        else if (received_data == 0)
        {
            RED_LED_off();
        }

        /* forward same data to PC (UART) */
        UART_sendByte(received_data);
    }
}
