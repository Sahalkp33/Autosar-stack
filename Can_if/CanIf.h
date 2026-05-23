/*
 * CanIf.h
 *
 *  Created on: Apr 20, 2026
 *      Author: sahal
 */

#ifndef CAN_IF_CANIF_H_
#define CAN_IF_CANIF_H_

#include "CanIf_Cfg.h"
#include "CanIf_Types.h"
#include "CanIf_Cbk.h"

#include "ComStack_Types.h"


#include "PduR_CanIf.h"

#include "Compiler.h"
#include "can.h"

/*Can if Module id */
#define CANIF_MODULE_ID					(60U)

/*CanIf instance Id */
#define CANIF_INSTANCE_ID				(0U)


/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/*Service ID for CanIf Init Api */
#define CANIF_INIT_SID							(uint8_t)0x01

/*Service ID for CanIf De_Init Api*/
#define CANIF_DE_INIT_SID						(uint8_t)0x02

/*Service ID for CanIf Set Controller Mode Api*/
#define CANIF_SET_CONTROLLER_MODE_SID			(uint8_t)0x03

/* Service ID for CanIf Transmit API*/
#define CANIF_TRANSMIT_SID						(uint8_t)0x49

/* Service ID for CanIf Set PDU Mode API*/
#define CANIF_SET_PDU_MODE_SID					(uint8_t)0x09

/* Service ID for CanIf_ReadRxPduData  API */
#define CanIf_ReadRxPduData_SID					(uint8_t)0x06

/*Service ID for CanIf_ReadTxNotifStatus Api*/
#define CanIf_ReadTxNotifStatus_SID				(uint8_t)0x07

/*Service ID for CanIf_ReadRxNotifStatus Api*/
#define CanIf_ReadRxNotifStatus_SID				(uint8_t)0x08

/*Service ID for CanIf */

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/

/*DET Code to Report that an API Service Called With Wrong Parameter */
#define CANIF_E_PARAM_CANID				(uint8_t)10
#define CANIF_E_PARAM_HOH				(uint8_t)12
#define CANIF_E_PARAM_LPDU              (uint8_t)13
#define CANIF_E_PARAM_CONTROLLERID		(uint8_t)15
#define CANIF_E_PARAM_CTRLMODE			(uint8_t)21
#define CANIF_E_PARAM_PDU_MODE			(uint8_t)22

/* DET code to report that an API service called with invalid pointer */
#define CANIF_E_PARAM_POINTER			(uint8_t)20

/* DET Code to report that an API Service used without initialization */
#define CANIF_E_UNINIT                  (uint8_t)30

/* DET code to report that Transmit PDU ID is invalid */
#define CANIF_E_INVALID_TXPDUID			(uint8_t)50

/* DET code to report that Recieve PDU ID is invalid */
#define CANIF_E_INVALID_RXPDUID			(uint8_t)60

#define TWO_MSB_STANDARD_MASK			(0X00000000)
#define TWO_MSB_EXTENDED_MASK			(0X80000000)
#define CANNIF_STANDARD_MAX             (0x400007FF)   /*max number for Standard32Bit*/
#define CANNIF_EXTENDED_MAX             (0xDFFFFFFF)   /*max number for Extended32Bit*/
#define CANIF_STANDARD_MAX_SDU_LENGTH	(uint8_t)8
#define CANIF_FD_MAX_SDU_LENGTH			(uint8_t)64
#define CANIF_MAX_PDU_LENGTH			(uint8_t)8

/*******************************************************************************
 *                     RUN TIME Error Codes                                        *
 *******************************************************************************/
/*RUN TIME Code to report that CAN Interface Fails Data Length Check*/
#define CANIF_E_INVALID_DATA_LENGTH		(uint8_t)61

/* RUN TIME Code to report the  Data Length Mismatch */
#define CANIF_E_DATA_LENGTH_MISMATCH	(uint8_t)62

/* RUN TIME Code to report that Message length  exceeded the maximum length */
#define CANIF_E_TXPDU_LENGTH_EXCEEDED	(uint8_t)90
/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
extern CanIf_State CanIfCurrent_State;
extern CanIf_ConfigType CanIf_Configuration;

#endif /* CAN_IF_CANIF_H_ */
