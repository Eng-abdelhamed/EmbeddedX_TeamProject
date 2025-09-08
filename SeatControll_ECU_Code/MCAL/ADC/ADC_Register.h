#ifndef ADC_REGISTER_H
#define ADC_REGISTER_H



#define ADMUX           *((volatile u8*)0x27)           //ADC multiplixer selection regester
#define ADMUX_REFS1     7                               // bit 1
#define ADMUX_REFS0     6                               // bit 0
#define ADMUX_ADLAR     5                               // left adjustment(ADCH , ADCL)




#define ADCSRA           *((volatile u8*)0x26)             //ADC control and status register
#define ADCSRA_ADEN      7                                 // enable
#define ADCSRA_ADSC      6                                 // start conversion
#define ADCSRA_ADATE     5                                 // auto trigerring enable
#define ADCSRA_ADIF      4                                 // interupt flag
#define ADCSRA_ADIE      3                                 // interupt enable
#define ADCSRA_ADPS2     2                                 // prescalar pit 3
#define ADCSRA_ADPS1     1                                 // prescalar pit 2
#define ADCSRA_ADPS0     0                                 // prescalar pit 1



#define SFIOR            *((volatile u8 *)0x50)
#define SFIOR_ADTS0      5
#define SFIOR_ADTS1      6
#define SFIOR_ADTS2      7


#define ADCH    *((volatile u8*)0x25)                       // ADC high register
#define ADCL    *((volatile u8*)0x24)                       // ADC low register
#define ADC     *((volatile u16*)0x24)              // returning the value of the ADCL  in 10 bit resolution









#endif
