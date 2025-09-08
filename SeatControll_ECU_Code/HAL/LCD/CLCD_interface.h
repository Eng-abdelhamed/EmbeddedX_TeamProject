/***********************************************************************/
/***********************************************************************/
/********************** author     : Abdelhamed Ahmed *********************/
/********************** Layer      : HAL             *********************/
/********************** SWC        : LCD             *********************/
/********************** version    : 1.00          *********************/
/***********************************************************************/
/***********************************************************************/
/* Pre Processor Directive */
#ifndef CLCD_INTERFACE_H
#define CLCD_INTERFACE_H

/* Send Command to LCD */
void CLCD_voidSendCommand(u8 Copu_u8Command);

/* Send Data to LCD */
void CLCD_voidSendData(u8 Copy_u8Data);


/* INITALIZE LCD */
void CLCD_voidInitialize(void);

/*Send string to LCD*/
void CLCD_voidSendString(const char*Copy_pcharString);


/*go to position function */
void CLCD_voidGotoX_Y(u8 Copy_u8XPos,u8 Copy_u8YPos);

/*function to write characters */
void CLCD_voidWriteSpecialChar(u8*Copy_pu8Array , u8 COpy_u8BlockNumber , u8 Copy_u8Xpos,u8 Copy_u8Ypos);


/*   write numbers on LCD */
void CLCD_voidWriteNumber(u32 Copy_u32Number);

/*Delete Char on Screen*/
void CLCD_voidClearScreen(void);

void uint16_to_string(u16 value, u8 *array);

void uint8_to_string(u8 value, u8 *array);

void float_to_string(float value, char* buffer, int decimal_places);

void SendStringX_Y(const char*Copy_pcharString,u8 Copy_u8XPos,u8 Copy_u8YPos);









#endif
