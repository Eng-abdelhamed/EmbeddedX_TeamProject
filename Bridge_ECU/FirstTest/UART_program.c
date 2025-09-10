#include "STD_TYPEs.h"
#include "BIT_MATH.h"
#include "UART_Register.h"
#include "UART_interface.h"
#include "UART_config.h"
#include "UART_private.h"

static void (*function_Tx)(void) = 0;
static void (*function_Rx)(void) = 0;
static void (*function_UDR)(void) = 0;

void USART_voidINIT(u16 baudrate)
{
	/**/
		UCSRA=0b00100010;
		/*bit7---->disable interrupt
		 *bit6---->disable interrupt
		 *bit6---->disable
		 *bit4---->enable tx
		 *bit3---->enable rx
		 *bit2---->0--->8bit size
		 *bit1---->init 0
		 *bit1---->u2x   */
		UCSRB=0b10111000;
		/*bit7-------->select reg
		 * bit6--------->Asyn
		 * bit5-------->0
		 * bit4------->0 disable parity
		 * bit3-------->1 bit stop
		 * bit2-------->
		 * bit1--------->8 bit size
		 * */
		UCSRC=0b10000110;
		/*buad rate=51-------->9600*/
		UBRRL=103;
		UBRRH=0;
}
void USART_voidTRANSMIT(u8 DATA)
{
    while (!(UCSRA & (1 << UCSRA_UDRE)));  // Wait for Data Register Empty
    UDR = DATA;  // Send data
}

u8 USART_u8RECIVER(void)
{
    while (!(UCSRA & (1 << UCSRA_RXC)));  // Wait for Reception to complete
    return UDR;  // Return received data
}

void USART_voidSendString(u8 *COPYu8StringPtr)
{
    u8 i = 0;
    while (COPYu8StringPtr[i] != '\0')
    {
        USART_voidTRANSMIT(COPYu8StringPtr[i]);
        i++;
    }
}

void USART_voidReceiveString(u8 *COPYu8StringPtr)
{
    u8 i = 0;
    COPYu8StringPtr[i] = USART_u8RECIVER();
    while (COPYu8StringPtr[i] != '#')  // Use '#' as string terminator
    {
        i++;
        COPYu8StringPtr[i] = USART_u8RECIVER();
    }
    COPYu8StringPtr[i] = '\0';  // Null-terminate the string
}

u8 USART_SetDataRegisterEmptyCallback(void(*copy_callback)(void))
{
    if (copy_callback == 0)
        return 0;
    else
        function_UDR = copy_callback;
    return 1;
}

u8 USART_SetTxCompleteCallback(void(*copy_callback)(void))
{
    if (copy_callback == 0)
        return 0;
    else
        function_Tx = copy_callback;
    return 1;
}

u8 USART_SetRxCompleteCallback(void(*copy_callback)(void))
{
    if (copy_callback == 0)
        return 0;
    else
        function_Rx = copy_callback;
    return 1;
}

/* ISR for UDRE (Data Register Empty) */
void __vector_14(void) __attribute__((signal));
void __vector_14(void)
{
    if (function_UDR != 0)
        function_UDR();
}

/* ISR for RX Complete */
void __vector_13(void) __attribute__((signal));
void __vector_13(void)
{
    if (function_Rx != 0)
        function_Rx();
}

/* ISR for TX Complete */
void __vector_15(void) __attribute__((signal));
void __vector_15(void)
{
    if (function_Tx != 0)
        function_Tx();
}
