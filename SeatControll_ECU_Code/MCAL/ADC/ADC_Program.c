#include"../Common/STD_TYPEs.h"
#include"../Common/BIT_MATH.h"
#include"ADC_Register.h"
#include"ADC_Interface.h"
#include"ADC_Config.h"
#include"ADC_Private.h"


void (*Interupt_Handler)(void);
static inline void PRescalar_select(ADC_T*adc);
static inline void channel_select(ADC_T*adc);
static inline void voltage_Select(ADC_T*adc);
static inline void Reference_Selection_dBits(ADC_T*adc);
static void select_channel(ADC_CHANNEL_T channel);
void ADC_INIT(ADC_T*adc)
{
	voltage_Select(adc);
	PRescalar_select(adc);
#if INTERUPT_ENABLE == ENABLE
	SET_BIT(ADCSRA,ADCSRA_ADIE);
	Interupt_Handler = adc->Call_BckFun;
#endif
	Reference_Selection_dBits(adc);
	SET_BIT(ADCSRA,ADCSRA_ADEN);
}

void START_CONVERSION(ADC_T*adc)
{
 SET_BIT(ADCSRA,ADCSRA_ADSC);
}

void ADC_CONVERSION_RESULT(ADC_T*adc,u16*Result)
{
	*Result = ADC;
}
void ADC_GETCONVERSION_POLLING(ADC_T*adc , ADC_CHANNEL_T Channle , u16*Result)
{
	select_channel(Channle);
	START_CONVERSION(adc);
	while(GET_BIT(ADCSRA,ADCSRA_ADIF)==0);
	SET_BIT(ADCSRA,ADCSRA_ADIF);
	ADC_CONVERSION_RESULT(adc,Result);
}

void ADC_GETCONVERSION_INTRUPT(ADC_T*adc , ADC_CHANNEL_T Channle)
{
	select_channel(Channle);
	START_CONVERSION(adc);
}

static void PRescalar_select(ADC_T*adc)
{
  switch(adc->CLK)
  {
   case PRESCALALR_DIV2:
	   SET_BIT(ADCSRA,ADCSRA_ADPS0);
	   CLR_BIT(ADCSRA,ADCSRA_ADPS1);
	   CLR_BIT(ADCSRA,ADCSRA_ADPS2);
	   break;
   case PRESCALALR_DIV4:
	   CLR_BIT(ADCSRA,ADCSRA_ADPS0);
	   SET_BIT(ADCSRA,ADCSRA_ADPS1);
	   CLR_BIT(ADCSRA,ADCSRA_ADPS2);
	   break;
   case PRESCALALR_DIV8:
	   SET_BIT(ADCSRA,ADCSRA_ADPS0);
  	   SET_BIT(ADCSRA,ADCSRA_ADPS1);
  	   CLR_BIT(ADCSRA,ADCSRA_ADPS2);
  	   break;

   case PRESCALALR_DIV16:
	   CLR_BIT(ADCSRA,ADCSRA_ADPS0);
	   CLR_BIT(ADCSRA,ADCSRA_ADPS1);
  	   SET_BIT(ADCSRA,ADCSRA_ADPS2);
  	   break;

   case PRESCALALR_DIV32:
	   SET_BIT(ADCSRA,ADCSRA_ADPS0);
 	   CLR_BIT(ADCSRA,ADCSRA_ADPS1);
   	   SET_BIT(ADCSRA,ADCSRA_ADPS2);
   	   break;

   case PRESCALALR_DIV64:
	   CLR_BIT(ADCSRA,ADCSRA_ADPS0);
 	   SET_BIT(ADCSRA,ADCSRA_ADPS1);
       SET_BIT(ADCSRA,ADCSRA_ADPS2);
       break;

   case PRESCALALR_DIV128:
	   SET_BIT(ADCSRA,ADCSRA_ADPS0);
   	   SET_BIT(ADCSRA,ADCSRA_ADPS1);
       SET_BIT(ADCSRA,ADCSRA_ADPS2);
       break;
   default:
	   break;

  }
}
static void channel_select(ADC_T*adc)
{
  switch(adc->Channle)
  {
  case CHANNEL_0:
	  ADMUX &=MASKING;
	  ADMUX |=CHANNEL_0;
	  break;
  case CHANNEL_1:
	  ADMUX &=MASKING;
	  ADMUX |=CHANNEL_1;
	  break;
  case CHANNEL_2:
	  ADMUX &=MASKING;
	  ADMUX |=CHANNEL_2;
	  break;
  case CHANNEL_3:
	  ADMUX &=MASKING;
	  ADMUX |=CHANNEL_3;
	  break;
  case CHANNEL_4:
	  ADMUX &=MASKING;
	  ADMUX |=CHANNEL_4;
	  break;
  case CHANNEL_5:
	  ADMUX &=MASKING;
	  ADMUX |=CHANNEL_5;
	  break;
  case CHANNEL_6:
	  ADMUX &=MASKING;
	  ADMUX |=CHANNEL_6;
	  break;
  case CHANNEL_7:
	  ADMUX &=MASKING;
	  ADMUX |=CHANNEL_7;
	  break;
  default: break;
  }
}
static void voltage_Select(ADC_T*adc)
{
	CLR_BIT(SFIOR,SFIOR_ADTS0);
	CLR_BIT(SFIOR,SFIOR_ADTS1);
	CLR_BIT(SFIOR,SFIOR_ADTS2);
}
static void Reference_Selection_dBits(ADC_T*adc)
{

	switch(adc->Volt_Select)
		{

		case AREF_Internal:
			CLR_BIT(ADMUX,ADMUX_REFS1);
			CLR_BIT(ADMUX,ADMUX_REFS0);
			break;

		case AVCC:
				CLR_BIT(ADMUX,ADMUX_REFS1);
				SET_BIT(ADMUX,ADMUX_REFS0);
				break;

		case Internal_2_56:
			    SET_BIT(ADMUX,ADMUX_REFS1);
				SET_BIT(ADMUX,ADMUX_REFS0);
				break;
		default: break;
		}

}
static void select_channel(ADC_CHANNEL_T channel)
{
	switch(channel)
	  {
	  case CHANNEL_0:
		  ADMUX &=MASKING;
		  ADMUX |=CHANNEL_0;
		  break;
	  case CHANNEL_1:
		  ADMUX &=MASKING;
		  ADMUX |=CHANNEL_1;
		  break;
	  case CHANNEL_2:
		  ADMUX &=MASKING;
		  ADMUX |=CHANNEL_2;
		  break;
	  case CHANNEL_3:
		  ADMUX &=MASKING;
		  ADMUX |=CHANNEL_3;
		  break;
	  case CHANNEL_4:
		  ADMUX &=MASKING;
		  ADMUX |=CHANNEL_4;
		  break;
	  case CHANNEL_5:
		  ADMUX &=MASKING;
		  ADMUX |=CHANNEL_5;
		  break;
	  case CHANNEL_6:
		  ADMUX &=MASKING;
		  ADMUX |=CHANNEL_6;
		  break;
	  case CHANNEL_7:
		  ADMUX &=MASKING;
		  ADMUX |=CHANNEL_7;
		  break;
	  default: break;
	  }
}

void __vector_16(void) __attribute__((signal));  // ISR
void __vector_16(void)
{
	Interupt_Handler();
}
