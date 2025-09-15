/*
 * rte.h
 *
 *  Created on: Sep 15, 2025
 *      Author: Wagdy
 */
/* rte.h */
#ifndef RTE_H_
#define RTE_H_

#include <stdint.h>


/* ========== Sender/Receiver APIs ========== */
void Rte_Write_BtnCtrl_Command(uint8_t data);
void Rte_Read_BtnCtrl_Command(uint8_t* data);


/* Client/Server */

/* ========== Client/Server APIs ========== */
uint8_t Rte_Call_BtnCtrl_GetMotorStatus(void);

#endif
