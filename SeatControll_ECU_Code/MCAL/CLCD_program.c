/***********************************************************************/
/***********************************************************************/
/********************** author     : Abdelhamed Ahmed *********************/
/********************** Layer      : HAL              *********************/
/********************** SWC        : LCD              *********************/
/********************** version    : 1.00             *********************/
/***********************************************************************/
/***********************************************************************/
#include "STD_TYPEs.h"
#include "util/delay.h"
#include "DIO_interface.h"
#include "CLCD_config.h"
#include "CLCD_interface.h"
#include "CLCD_private.h"
#include<string.h>
/* Send Command to LCD */

void CLCD_voidSendCommand(u8 Copu_u8Command)
{
	/*Set RS pin to low for command */
	DIO_u8SetPinValue  (CLCD_CTRL_PORT , CLCD_RS_PIN , DIO_u8PIN_LOW);

	/*Set RW pin to low for command */
	DIO_u8SetPinValue  (CLCD_CTRL_PORT , CLCD_RW_PIN , DIO_u8PIN_LOW);

	/* SET command Data */
	DIO_u8SetPortValue (CLCD_DATA_PORT , Copu_u8Command);

	/*Send Enable pin to low for command */
	DIO_u8SetPinValue  (CLCD_CTRL_PORT , CLCD_E_PIN , DIO_u8PIN_HIGH);

	_delay_ms(2);

	DIO_u8SetPinValue  (CLCD_CTRL_PORT , CLCD_E_PIN , DIO_u8PIN_LOW);
}

/* Send Data to LCD */
void CLCD_voidSendData(u8 Copy_u8Data)  
{
	/*Set RS pin to high for data */
	DIO_u8SetPinValue  (CLCD_CTRL_PORT , CLCD_RS_PIN , DIO_u8PIN_HIGH);

	/*Set RW pin to low for data */
	DIO_u8SetPinValue  (CLCD_CTRL_PORT , CLCD_RW_PIN , DIO_u8PIN_LOW);

	/* SET command Data */
	DIO_u8SetPortValue (CLCD_DATA_PORT , Copy_u8Data);

	/*Send Enable pin to low for command */
	DIO_u8SetPinValue  (CLCD_CTRL_PORT , CLCD_E_PIN , DIO_u8PIN_HIGH);

	_delay_ms(2);

	DIO_u8SetPinValue  (CLCD_CTRL_PORT , CLCD_E_PIN , DIO_u8PIN_LOW);
}

/********** INITALIZE LCD *********** */
void CLCD_voidInitialize(void)
{
	/*wait for more than 30 Ms */
	_delay_ms(40);
	
	/*Function Set command 2lines , 5*8 font size*/
	CLCD_voidSendCommand(0b00111000);
	/* display on off control  display enable ,disable cursor , no Blink cursor*/
	CLCD_voidSendCommand(0b00001100);

	/* clear display*/
	CLCD_voidSendCommand(1);
}

void CLCD_voidSendString(const char*Copy_pcharString)
{
	u8 Counter  = 0;
	while(Copy_pcharString[Counter]!= '\0')
	{
		CLCD_voidSendData(Copy_pcharString[Counter]);
		Counter++;
	}
}

void CLCD_voidGotoX_Y(u8 Copy_u8XPos,u8 Copy_u8YPos)
{
	u8 Local_u8Address;
	if(Copy_u8XPos == 0)
	{
		/* location is at First line */
		Local_u8Address = Copy_u8YPos ;
	}
	else if(Copy_u8XPos == 1)
	{
		/* location is at Second line */
		Local_u8Address = Copy_u8YPos + 0x40;
	}
	CLCD_voidSendCommand(Local_u8Address + 128);
}

void CLCD_voidWriteSpecialChar(u8*Copy_pu8Array , u8 COpy_u8BlockNumber , u8 Copy_u8Xpos,u8 Copy_u8Ypos)
{
	u8 Local_u8CGRamAddress=0;
	u8 Local_u8Iterator ;
	
	/*         Calculate CG RAM address           */
	
	Local_u8CGRamAddress = COpy_u8BlockNumber * 8;
	
	/*         writing CG RAM address           */
	
	CLCD_voidSendCommand(COpy_u8BlockNumber + 64);

	for(Local_u8Iterator =0 ;Local_u8Iterator < 8 ;Local_u8Iterator++)
	{
		CLCD_voidSendData(Copy_pu8Array[Local_u8Iterator]);
	}
	/*     Go back to DDRam to display array       */
	
	CLCD_voidGotoX_Y(  Copy_u8Xpos ,Copy_u8Ypos);

	/* Write the Array in the block number itself  */
	
	CLCD_voidSendData(COpy_u8BlockNumber);
}

void CLCD_voidWriteNumber(u32 Copy_u32Number) 
{
    char buffer[12]; // Buffer to hold the number as a string, max 10 digits + null terminator
    u8 i = 0;
    if (Copy_u32Number == 0) 
	{
        CLCD_voidSendData('0');
        return;
    }
    // Extract digits from the number starting from the least significant digit
    while (Copy_u32Number != 0) 
	{
        buffer[i++] = (Copy_u32Number % 10) + '0'; // Convert digit to character and store in buffer
        Copy_u32Number /= 10;
    }
    // Reverse the buffer to get the correct order of digits
    for (u8 j = 0; j < i / 2; j++) 
	{
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
    // Null-terminate the string
    buffer[i] = '\0';
    // Send each character in the buffer to the LCD
    for (u8 k = 0; k < i; k++) 
	{
        CLCD_voidSendData(buffer[k]);
    }
}
void uint8_to_string(u8 value, u8 *array)
{
	u8 temp_array[4] = {0};
	u8 i=0;
	memset(array,' ',3);
	temp_array[3]='\0';
	sprintf((char *)temp_array, "%i", value);
	while(temp_array[i]!='\0')
	{
		array[i]=temp_array[i];
		i++;
	}
}
void uint16_to_string(u16 value, u8 *array)
{
	u8 temp_array[6] = {0};
	u8 i=0;
	memset(array,' ',5);
	temp_array[5]='\0';
	sprintf((char *)temp_array, "%i", value);
	while(temp_array[i]!='\0')
	{
		array[i]=temp_array[i];
		i++;
	}
}
void SendStringX_Y(const char*Copy_pcharString,u8 Copy_u8XPos,u8 Copy_u8YPos)
{
	CLCD_voidGotoX_Y(Copy_u8XPos,Copy_u8YPos);
	CLCD_voidSendString(Copy_pcharString);

}
void float_to_string(float value, char* buffer, int decimal_places)
{
    int int_part = (int)value; // Separate the integer part
    float decimal_part = value - int_part; // Separate the decimal part

    // Convert the integer part to string
    itoa(int_part, buffer, 10); // Convert integer to string

    // Add the decimal point
    char* decimal_position = buffer + strlen(buffer);
    *decimal_position++ = '.';

    // Convert the decimal part to string with specified decimal places
    for (int i = 0; i < decimal_places; i++) {
        decimal_part *= 10;
        int digit = (int)decimal_part;
        *decimal_position++ = digit + '0'; // Add digit as character
        decimal_part -= digit; // Remove the digit from the decimal part
    }

    // Null-terminate the string
    *decimal_position = '\0';
}
void CLCD_voidClearScreen(void)
{

	CLCD_voidSendCommand(CLEAR);
	_delay_ms(1.53);
}
