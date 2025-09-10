#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H








void USART_voidINIT(u16 baudrate);
void USART_voidTRANSMIT(u8 DATA);
u8   USART_u8RECIVER(void);

void USART_VoidSendString(u8 *COPYu8StringPtr);
void USART_RecieveString(u8 *COPYu8StringPtr);

u8   USART_SetDataRegisterEmptyCallback(void(*copy_callback)(void));

u8   USART_SetTxCompleteCallback(void(*copy_callback)(void));
u8   USART_SetRxCompleteCallback(void(*copy_callback)(void));


#endif
