/*
 * LED.c
 *
 *  Created on: Jun 20, 2025
 *      Author: Wagdy
 */

#include "../HAl/LED.h"
#include "../common/common_macros.h"
#include<avr/io.h>
void LEDS_init(void)
{
    // RED
    SET_BIT(RED_LED_DDR, RED_LED_PIN);
#if (RED_LED_LOGIC == POSITIVE_LOGIC)
    CLEAR_BIT(RED_LED_PORT, RED_LED_PIN);
#else
    SET_BIT(RED_LED_PORT, RED_LED_PIN);
#endif

    // GREEN
    SET_BIT(GREEN_LED_DDR, GREEN_LED_PIN);
#if (GREEN_LED_LOGIC == POSITIVE_LOGIC)
    CLEAR_BIT(GREEN_LED_PORT, GREEN_LED_PIN);
#else
    SET_BIT(GREEN_LED_PORT, GREEN_LED_PIN);
#endif

    // BLUE
    SET_BIT(BLUE_LED_DDR, BLUE_LED_PIN);
#if (BLUE_LED_LOGIC == POSITIVE_LOGIC)
    CLEAR_BIT(BLUE_LED_PORT, BLUE_LED_PIN);
#else
    SET_BIT(BLUE_LED_PORT, BLUE_LED_PIN);
#endif
}

// ==== RED ====
void RED_LED_on(void)
{
#if (RED_LED_LOGIC == POSITIVE_LOGIC)
    SET_BIT(RED_LED_PORT, RED_LED_PIN);
#else
    CLEAR_BIT(RED_LED_PORT, RED_LED_PIN);
#endif
}

void RED_LED_off(void)
{
#if (RED_LED_LOGIC == POSITIVE_LOGIC)
    CLEAR_BIT(RED_LED_PORT, RED_LED_PIN);
#else
    SET_BIT(RED_LED_PORT, RED_LED_PIN);
#endif
}

// ==== GREEN ====
void GREEN_LED_on(void)
{
#if (GREEN_LED_LOGIC == POSITIVE_LOGIC)
    SET_BIT(GREEN_LED_PORT, GREEN_LED_PIN);
#else
    CLEAR_BIT(GREEN_LED_PORT, GREEN_LED_PIN);
#endif
}

void GREEN_LED_off(void)
{
#if (GREEN_LED_LOGIC == POSITIVE_LOGIC)
    CLEAR_BIT(GREEN_LED_PORT, GREEN_LED_PIN);
#else
    SET_BIT(GREEN_LED_PORT, GREEN_LED_PIN);
#endif
}

// ==== BLUE ====
void BLUE_LED_on(void)
{
#if (BLUE_LED_LOGIC == POSITIVE_LOGIC)
    SET_BIT(BLUE_LED_PORT, BLUE_LED_PIN);
#else
    CLEAR_BIT(BLUE_LED_PORT, BLUE_LED_PIN);
#endif
}

void BLUE_LED_off(void)
{
#if (BLUE_LED_LOGIC == POSITIVE_LOGIC)
    CLEAR_BIT(BLUE_LED_PORT, BLUE_LED_PIN);
#else
    SET_BIT(BLUE_LED_PORT, BLUE_LED_PIN);
#endif
}
