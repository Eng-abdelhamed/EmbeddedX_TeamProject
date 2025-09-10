
#include "../MCAL/I2C.h"
#include "../MCAL/uart.h"
#include "../common/stdtypes.h"

int main(void)
{
   /*init of i2c*/
    TWI_ConfigType I2C_Config = {0x20, TWI_BAUD_100KHZ};	/*address of slave*/
    TWI_init(&I2C_Config);

   /* init of uart */
    UART_ConfigType UART_Config = {UART_8BIT_MODE, even, UART_ONE_STOP_BIT, 9600};
    UART_init(&UART_Config);

    uint8 received_data;

    while(1)
    {
    	/*recieving data from master with ack*/

    	received_data = TWI_readByteWithACK();

        /*sending the recieved data from slave tio pc*/

        UART_sendByte(received_data);
    }
}
