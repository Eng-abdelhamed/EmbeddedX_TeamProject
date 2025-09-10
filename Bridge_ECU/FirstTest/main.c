#include "BIT_MATH.h"
#include "STD_TYPEs.h"
#include "DIO_interface.h"
#include "UART_interface.h"
#include "I2C_Interface.h"
#include "util/delay.h"
#include "avr/io.h"
u8 data;
u8 ahmed;
TWI_EnumError_Status errors;

void main(void)
{
	USART_voidINIT(33);
	I2C_SlaveInit(0x60);
    DIO_u8SetPinDirection(DIO_u8PORTA,0,1);
    while(1)
    {
    	I2C_ReadDataByteWithAck(&data);
    	USART_voidTRANSMIT(data);
        if(data == 'G')
        {
        	DIO_u8SetPinValue(DIO_u8PORTA,0,1);
        }else if(data == 'C')
        {
        	DIO_u8SetPinValue(DIO_u8PORTA,0,0);
        }
        _delay_ms(1000);
    }
}
