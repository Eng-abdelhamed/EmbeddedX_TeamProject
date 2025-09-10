#ifndef UART_REGISTER_H
#define UART_REGISTER_H

/* USART Data Register */
#define UDR           *((volatile u8*)0x2C)

/*********************************************************************************/
/* USART Control and Status Register A */
#define UCSRA         *((volatile u8*)0x2B)
#define UCSRA_RXC     7  // USART Receive Complete (flag)
#define UCSRA_TXC     6  // USART Transmit Complete (flag)
#define UCSRA_UDRE    5  // USART Data Register Empty
#define UCSRA_FE      4  // Frame Error
#define UCSRA_DOR     3  // Data Overrun (corrected: DOA -> DOR)
#define UCSRA_PE      2  // Parity Error
#define UCSRA_U2X     1  // Double USART Speed
#define UCSRA_MPCM    0  // Multi-Processor Communication Mode

/*********************************************************************************/
/* USART Control and Status Register B */
#define UCSRB         *((volatile u8*)0x2A)
#define UCSRB_RXCIE   7  // RX Complete Interrupt Enable
#define UCSRB_TXCIE   6  // TX Complete Interrupt Enable
#define UCSRB_UDRIE   5  // Data Register Empty Interrupt Enable
#define UCSRB_RXEN    4  // Receiver Enable
#define UCSRB_TXEN    3  // Transmitter Enable
#define UCSRB_UCSZ2   2  // Character Size Bit 2
#define UCSRB_RXB8    1  // Receive Data Bit 9
#define UCSRB_TXB8    0  // Transmit Data Bit 9

/*********************************************************************************/
/* USART Control and Status Register C */
#define UCSRC         *((volatile u8*)0x40)
#define UCSRC_URSEL   7  // Register Select (select between UCSRC or UBRRH)
#define UCSRC_UMSEL   6  // Mode Select (Synchronous/Asynchronous)
#define UCSRC_UPM1    5  // Parity Mode Bit 1
#define UCSRC_UPM0    4  // Parity Mode Bit 0
#define UCSRC_USBS    3  // Stop Bit Select
#define UCSRC_UCSZ1   2  // Character Size Bit 1
#define UCSRC_UCSZ0   1  // Character Size Bit 0
#define UCSRC_UCPOL   0  // Clock Polarity (for synchronous mode)

/*********************************************************************************/
/* Baud Rate Registers */
#define UBRRH         *((volatile u8*)0x40)  // Upper part of Baud Rate Register (same address as UCSRC)
#define UBRRL         *((volatile u8*)0x29)  // Lower part of Baud Rate Register

#endif
