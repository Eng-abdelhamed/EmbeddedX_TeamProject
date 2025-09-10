 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "../common/stdtypes.h"

       //=== BIT MODE FOR UART ===//
typedef enum {
UART_5BIT_MODE = 0,

UART_6BIT_MODE = 1,

UART_7BIT_MODE = 2,

UART_8BIT_MODE = 3

}UART_BitDataType;


       //===  PARITY TYPE FOR UART ===//
typedef enum{
	even=2,
	odd=1,
	no_parity=0

}UART_ParityType;

         //=== STOP BITS ===//
typedef enum {
	UART_ONE_STOP_BIT=1,
	UART_TWO_STOP_BITS=2
} UART_StopBitType;

             // === BAUD RATE ===//
typedef enum {
	    UART_BAUD_2400 = 2400,
		UART_BAUD_4800 = 4800,
		UART_BAUD_9600 = 9600,
		UART_BAUD_14400 = 14400,
		UART_BAUD_19200 = 19200,
		UART_BAUD_38400 = 38400,
		UART_BAUD_57600 = 57600,
		UART_BAUD_115200 = 115200
}UART_BaudRateType;

typedef struct {
UART_BitDataType bit_data;
UART_ParityType parity;
UART_StopBitType stop_bit;
UART_BaudRateType baud_rate;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
