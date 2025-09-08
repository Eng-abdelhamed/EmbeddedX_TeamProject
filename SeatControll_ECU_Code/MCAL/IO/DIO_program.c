
/*********************************************   **************************/
/******************************************      **************************/
/********************** author     : Abdelhamed Ahmed *********************/
/********************** Layer      : MCAL             *********************/
/********************** SWC  : DIO                    *********************/
/********************** version    : 1.00             *********************/
/**********************************************   *************************/
/*********************************************   **************************/
#include"../Common/STD_TYPEs.h"
#include"DIO_config.h"
#include"DIO_interface.h"
#include"DIO_register.h"
#include"DIO_private.h"
#include"../Common/BIT_MATH.h"


u8 DIO_u8SetPinDirection(u8 Copy_u8PORT ,u8 Copy_u8PIN,u8 Copy_u8Direction)
{
	u8 Local_u8ErrorState = 0 ;
	if(Copy_u8PIN <= DIO_u8PIN7)
	{
		if(Copy_u8Direction == DIO_u8PIN_INPUT)
		{
			switch(Copy_u8PORT)
			{
			case DIO_u8PORTA : CLR_BIT(DDRA,Copy_u8PIN);  break;
			case DIO_u8PORTB : CLR_BIT(DDRB,Copy_u8PIN);  break;
			case DIO_u8PORTC : CLR_BIT(DDRC,Copy_u8PIN);  break;
			case DIO_u8PORTD : CLR_BIT(DDRD,Copy_u8PIN);  break;
			default          :    Local_u8ErrorState =1;
			}
		}
		else if(Copy_u8Direction == DIO_u8PIN_Output)
		{
			switch(Copy_u8PORT)
			{
			case DIO_u8PORTA : SET_BIT(DDRA,Copy_u8PIN);  break;
			case DIO_u8PORTB : SET_BIT(DDRB,Copy_u8PIN);  break;
			case DIO_u8PORTC : SET_BIT(DDRC,Copy_u8PIN);  break;
			case DIO_u8PORTD : SET_BIT(DDRD,Copy_u8PIN);  break;
			default          :    Local_u8ErrorState =1;
			}
		}
	}
	else
	{
		Local_u8ErrorState =1;
	}
	return Local_u8ErrorState;
}
u8 DIO_u8SetPortDirection(u8 Copy_u8PORT  , u8 Copy_u8Direction)
{
	u8 Local_u8ErrorState = 0;
	switch(Copy_u8PORT)
	{
	case DIO_u8PORTA:    DDRA = Copy_u8Direction ; break;
	case DIO_u8PORTB:    DDRB = Copy_u8Direction ; break;
	case DIO_u8PORTC:    DDRC = Copy_u8Direction ; break;
	case DIO_u8PORTD:    DDRD = Copy_u8Direction ; break;
	default : Local_u8ErrorState = 1;

	}
	return Local_u8ErrorState;
}
u8 DIO_u8SetPinValue(u8 Copy_u8PORT  ,u8 Copy_u8PIN , u8 Copy_u8Value)
{
	u8 Local_u8ErrorState = 0;
	if(Copy_u8PIN <= DIO_u8PIN7)
	{
		if(Copy_u8Value == DIO_u8PIN_LOW)
		{
			switch(Copy_u8PORT)
			{
			           case DIO_u8PORTA : CLR_BIT(PORTA,Copy_u8PIN);  break;
			           case DIO_u8PORTB : CLR_BIT(PORTB,Copy_u8PIN);  break;
			           case DIO_u8PORTC : CLR_BIT(PORTC,Copy_u8PIN);  break;
			           case DIO_u8PORTD : CLR_BIT(PORTD,Copy_u8PIN);  break;
			           default          :    Local_u8ErrorState =1;
			}

		}
		else if(Copy_u8Value == DIO_u8PIN_HIGH)
		{
			switch(Copy_u8PORT)
			{
			           case DIO_u8PORTA : SET_BIT(PORTA,Copy_u8PIN);  break;
			           case DIO_u8PORTB : SET_BIT(PORTB,Copy_u8PIN);  break;
			           case DIO_u8PORTC : SET_BIT(PORTC,Copy_u8PIN);  break;
			           case DIO_u8PORTD : SET_BIT(PORTD,Copy_u8PIN);  break;
			           default          :    Local_u8ErrorState =1;
			}

		}
	}
	else
	{
		Local_u8ErrorState =1;
	}
	return Local_u8ErrorState;
}
u8 DIO_u8SetPortValue(u8 Copy_u8PORT , u8 Copy_u8Value)
{
	u8 Local_u8ErrorState = 0;
	switch(Copy_u8PORT)
	{
	 case DIO_u8PORTA:  PORTA = Copy_u8Value ;  break;
	 case DIO_u8PORTB:  PORTB = Copy_u8Value ;  break;
	 case DIO_u8PORTC:  PORTC =Copy_u8Value  ;  break;
	 case DIO_u8PORTD:  PORTD =Copy_u8Value  ;  break;
	 default         : Local_u8ErrorState =1;
	}
	return Local_u8ErrorState;
}
u8 DIO_u8GetPinValue(u8 Copy_u8PORT , u8 Copy_u8PIN , u8 * Copy_Pu8Value)
{
	u8 Local_u8ErrorState = 0;
	if(Copy_u8PIN <= DIO_u8PIN7 && Copy_Pu8Value != NULL)
	{
		switch(Copy_u8PORT)
		{
		     case DIO_u8PORTA:  * Copy_Pu8Value = GET_BIT(PINA , Copy_u8PIN) ;  break;
		     case DIO_u8PORTB:  * Copy_Pu8Value = GET_BIT(PINB , Copy_u8PIN) ;  break;
		     case DIO_u8PORTC:  * Copy_Pu8Value = GET_BIT(PINC , Copy_u8PIN) ;  break;
		     case DIO_u8PORTD:  * Copy_Pu8Value = GET_BIT(PIND , Copy_u8PIN) ;  break;
			 default : Local_u8ErrorState =1 ;
		}
	}
	else
	{
		Local_u8ErrorState =1;
	}
	return Local_u8ErrorState ;
}
