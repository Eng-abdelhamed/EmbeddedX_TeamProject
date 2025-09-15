/*
 * rte.c
 *
 *  Created on: Sep 15, 2025
 *      Author: Wagdy
 */


/* rte.c */
#include "../RTE/rte.h"
#include "../MCAL/motor.h"
#include "../common/events.h"

/* Shared Data */
static uint8_t Rte_Command;
static uint8_t lastCommand = 0xFF; // علشان نعرف حصل تغيير ولا لا

void Rte_Write_BtnCtrl_Command(uint8_t data) {
    Rte_Command = data;

    /* Generate DataReceivedEvent لو فيه تغيير */
    if(data != lastCommand) {
        extern void MotorControl_Runnable(EventType ev);
        MotorControl_Runnable(EVENT_DATA_RECEIVED);
        lastCommand = data;
    }
}

void Rte_Read_BtnCtrl_Command(uint8_t* data) {
    *data = Rte_Command;
}

uint8_t Rte_Call_BtnCtrl_GetMotorStatus(void) {
    return Motor_GetStatus();  // OperationInvokedEvent (Server)
}
