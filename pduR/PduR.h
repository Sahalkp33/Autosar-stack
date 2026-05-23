/*
 * PduR.h
 *
 *  Created on: May 12, 2026
 *      Author: sahal
 */

#ifndef PDUR_PDUR_H_
#define PDUR_PDUR_H_

#include <stdint.h>
#include "PduR_Types.h"

/* PduR Module Id */
#define PDUR_MODULE_ID			(51U)

/* PduR Instance Id */
#define PDUR_INSTANCE_ID		(0U)

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for PduR_Init API */
#define PDUR_INIT_SID			(uint8_t)0xF0
/* Service ID for PduR_ComTransmit API */
#define PDUR_COMTRANSMIT_SID	(uint8_t)0x49


/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* Invalid configuration pointer */
#define PDUR_E_INIT_FAILED		(uint8_t)0x00
/* API service used without module initialization */
#define PDUR_E_UNINIT			(uint8_t)0x01
/* Invalid PDU identifier */
#define PDUR_E_PDU_ID_INVALID	(uint8_t)0x02
/* Null pointer has been passed as an argument */
#define PDUR_E_PARAM_POINTER	(uint8_t)0x09

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
extern PduR_StateType PduRCurrent_State;
extern PduR_ConfigType PduR_Configuration;

#endif /* PDUR_PDUR_H_ */
