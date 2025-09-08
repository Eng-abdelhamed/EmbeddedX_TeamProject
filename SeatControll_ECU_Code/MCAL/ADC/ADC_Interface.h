#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H

#define MASKING 0b11100000
typedef enum
{
 FREE_Running_mode = 0,
 Analog_Comparator,
 External_Interrupt_Request,
 Timer_Counter0_Compare_Match,
 Timer_Counter0_Overflow,
 Timer_Counter1_Compare_Match_B,
 Timer_Counter1_Overflow,
 Timer_Counter1_Capture_Event

}ADC_Auto_TRIG_MODE;

typedef enum
{
  CHANNEL_0 =0,
  CHANNEL_1,
  CHANNEL_2,
  CHANNEL_3,
  CHANNEL_4,
  CHANNEL_5,
  CHANNEL_6,
  CHANNEL_7
}ADC_CHANNEL_T;

typedef enum
{
	PRESCALALR_DIV2= 0,
	PRESCALALR_DIV29,
	PRESCALALR_DIV4,
	PRESCALALR_DIV8,
	PRESCALALR_DIV16,
	PRESCALALR_DIV32,
	PRESCALALR_DIV64,
	PRESCALALR_DIV128
}PRESCALAR_T;

typedef enum
{
 AREF_Internal=0,
 AVCC,
 Reserved,
 Internal_2_56

}Reference_Selection_Bits;



typedef struct
{
#if INTERUPT_ENABLE == ENABLE
	void (*Call_BckFun)(void);
#endif
	ADC_CHANNEL_T Channle;
	PRESCALAR_T CLK;
	ADC_Auto_TRIG_MODE Mode;
	Reference_Selection_Bits Volt_Select;
}ADC_T;

void ADC_INIT(ADC_T*adc);
void START_CONVERSION(ADC_T*adc);
void ADC_CONVERSION_RESULT(ADC_T*adc,u16*Result);
void ADC_GETCONVERSION_POLLING(ADC_T*adc , ADC_CHANNEL_T Channle , u16*Result);
void ADC_GETCONVERSION_INTRUPT(ADC_T*adc , ADC_CHANNEL_T Channle);




#endif
