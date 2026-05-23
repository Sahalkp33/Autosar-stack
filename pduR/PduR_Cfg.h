/*
 * PduR_Cfg.h
 *
 *  Created on: Apr 16, 2026
 *      Author: sahal
 */

#ifndef PDUR_PDUR_CFG_H_
#define PDUR_PDUR_CFG_H_

#include <stdint.h>
#include "Std_Types.h"
#include "ComStack_Types.h"


/************************PduRGeneral Container************************/
/*Switches the development error detection and notification on or off.*/
#define PduRDevErrorDetect			(STD_ON)


#define PduRMaxRoutingPathCount		(uint8_t)3


/************************PduRDestPdu subContainer************************/

/* PDU identifier assigned by PDU Router. Used by communication interface
 * and transport protocol modules for confirmation
 *(PduR_<Lo>TxConfirmation) and for TriggerTransmit
 *(PduR_<Lo>TriggerTransmit)
 */
#define     PduRDestPduHandleId_0   (uint16_t)0
#define     PduRDestPduHandleId_1   (uint16_t)1
#define     PduRDestPduHandleId_2   (uint16_t)2

/* This parameter is only for communication interfaces. Transport protocol
 * modules will always call the TxConfirmation function.
 * If set the destination communication interface module will call the
 * TxConfirmation. However the TxConfirmation may be not called due to
 * error. So the PduR shall not block until the TxConfirmation is called.
 */
#define PduRTransmissionConfirmation                      (STD_ON)

#define PduRSrcPduHandleId_0		(uint8_t)0

/* Maximum PDU ID that Can be routed */
#define PduR_MaxPduId				(uint8_t)255

#endif /* PDUR_PDUR_CFG_H_ */
