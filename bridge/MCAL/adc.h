/*
 * adc.h
 *
 *  Created on: May 10, 2025
 *      Author: Wagdy
 */
#ifndef ADC_H_
#define ADC_H_

#include "../Common/stdtypes.h"

#define ADC_MAX_VALUE        1023
#define ADC_REF_VOLT_VALUE   2.56

void ADC_init(void);
uint16 ADC_readChannel(uint8 channel_num);

#endif /* ADC_H_ */
