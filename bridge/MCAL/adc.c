/*
 * adc.c
 *
 *  Created on: May 10, 2025
 *      Author: Wagdy
 */
#include <avr/io.h>
#include "../Common/stdtypes.h"
#include "../Common/common_macros.h" // For BIT macros if needed
#include "../MCAL/ADC.h"
void ADC_init(void)
{
    // Select internal 2.56V reference voltage (REFS1 = 1, REFS0 = 1)
    ADMUX = (1 << REFS1) | (1 << REFS0);

    // Enable ADC and set prescaler to F_CPU/128 (ADPS2 = 1, ADPS1 = 1, ADPS0 = 1)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16 ADC_readChannel(uint8 channel_num)
{
    channel_num &= 0x07;               // Mask to keep it within 0-7
    ADMUX = (ADMUX & 0xF8) | channel_num; // Set channel, preserve voltage refs

    SET_BIT(ADCSRA, ADSC);             // Start conversion

    while (BIT_IS_CLEAR(ADCSRA, ADIF)); // Wait for conversion to complete

    SET_BIT(ADCSRA, ADIF);             // Clear the interrupt flag by writing 1

    return ADC;                        // Return the 10-bit ADC result
}
