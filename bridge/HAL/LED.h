/*
 * LED.h
 *
 *  Created on: Jun 20, 2025
 *      Author: Wagdy
 */

#ifndef LED_H_
#define LED_H_

#include "../common/stdtypes.h"

/* ==== User Configuration ==== */
#define POSITIVE_LOGIC  0
#define NEGATIVE_LOGIC  1
// RED LED
#define RED_LED_PORT   PORTA
#define RED_LED_DDR    DDRA
#define RED_LED_PIN    PA0
#define RED_LED_LOGIC  POSITIVE_LOGIC

// GREEN LED
#define GREEN_LED_PORT   PORTB
#define GREEN_LED_DDR    DDRB
#define GREEN_LED_PIN    PB6
#define GREEN_LED_LOGIC  POSITIVE_LOGIC

// BLUE LED
#define BLUE_LED_PORT   PORTA
#define BLUE_LED_DDR    DDRA
#define BLUE_LED_PIN    PA1
#define BLUE_LED_LOGIC  POSITIVE_LOGIC




void LEDS_init(void);
void RED_LED_on(void);
void RED_LED_off(void);
void GREEN_LED_on(void);
void GREEN_LED_off(void);
void BLUE_LED_on(void);
void BLUE_LED_off(void);

#endif /* LED_H_ */
