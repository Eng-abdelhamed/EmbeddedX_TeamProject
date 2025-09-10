#ifndef UART_CONFIG_H
#define UART_CONFIG_H

#define FOSC 8000000UL
/**
 *
 * 1 - Synchoronus
 * 2 - Asynchoronus
 */
#define USART_Mode_Select     ASynchoronus

/**
 * Parity Mode
 * 1 - Disabled
 * 2 - Enabled_Even_Parity
 * 3 - Enabled_Odd_Parity
 */
#define Parity_Mode_Select    Disabled

/**
 * Stop Bit Select
 * 1 - one_Bit
 * 2 - two_Bit
 */
#define Stop_Bit_Select       one_Bit

/**
 * Choose Character Size
 *  1-five_Bit
 *  1-six_Bit
 *  1-Seven_Bit
 *  1-eight_bit
 *  1-nine_bit
 */
#define Charac_Size_Select eight_bit

/**
 * rising data and falling data
 *  1 - RisingXCK_FallingXCK
 *  2 - FallingXCK_RisingXCK
 *
 */

#define Clock_Polarity_Select  RisingXCK_FallingXCK

/**
 * USART TX Complete Interrupt Enable
 * 1 -enable
 * 2 -disable
 */
#define TXIEState disable


/**
 * USART RX Complete Interrupt Enable
 * 1 -enable
 * 2 -disable
 */
#define RXIEState disable


/**
 * USART RX Complete Interrupt Enable
 * 1 -enable
 * 2 -disable
 */
#define UDIEState disable
#endif
