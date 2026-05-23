/*
 * Com.h
 *
 *  Created on: May 12, 2026
 *      Author: sahal
 */

#ifndef COM_COM_H_
#define COM_COM_H_

#include "ComStack_Types.h"
#include <stdint.h>

/* COM Module Id */
#define COM_MODULE_ID			(50U)

/* COM Instance Id */
#define COM_INSTANCE_ID			(0U)


/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Com_RxIndication API */
#define COM_RxIndication_SID				(uint8_t)0x42

/* Service ID for Com_SendSignal API */
#define Com_Send_Signal_SID					(uint8_t)0x0A

/* Service ID for Com_Recieve_Signal API */
#define Com_Recieve_Signal_SID				(uint8_t)0x48

/* Service ID for Com_MainFunctionRx API */
#define Com_MainFunctionRx_SID				(uint8_t)0x50

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* API service called with wrong parameter */
#define COM_E_PARAM							(uint8_t)0x01

/* Error code if any other API service, except Com_GetStatus, is
 * called before the AUTOSAR COM module was initialized with Com_Init or after a call
 * to Com_Deinit
 */
#define COM_E_UNINIT						(uint8_t)0x02

/* NULL pointer checking */
#define COM_E_PARAM_POINTER					(uint8_t)0x03


/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
extern Com_ConfigType Com;
extern Com_StatusType Com_CurrentState;
extern uint8_t PDU_INDEX;
extern PduInfoType PDU[ComMaxIpduCount];
extern uint8_t SignalObject[MAX_NUM_OF_SIGNALS];

#endif /* COM_COM_H_ */
