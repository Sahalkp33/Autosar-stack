/*
 * CanIf_Cbk.h
 *
 *  Created on: May 10, 2026
 *      Author: sahal
 */

#ifndef CAN_IF_CANIF_CBK_H_
#define CAN_IF_CANIF_CBK_H_

#include "CanIf.h"
#include "CanIf_Types.h"

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/

/*Service ID for CanIf Tx Confirmation Api*/
#define CANIF_TX_CONFIRMATION_SID				(uint8_t)0x13

/* Service ID for CanIf Rx Indication API*/
#define CANIF_RX_INDICATION_SID					(uint8_t)0x14

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/


/*Rx Notification Flags*/
#if(CanIfPublicReadRxPduNotifyStatusApi == STD_ON)
extern CanIf_NotifStatusType CanIf_RxNotificationFlag[CanIfMaxRxPduCfg];
#endif

/*Tx Notification Flags*/
#if(CanIfPublicReadTxPduNotifyStatusApi == STD_ON)
extern CanIf_NotifStatusType CanIf_TxNotificationFlag[CanIfMaxTxPduCfg];
#endif

#if(CanIfPublicReadRxPduDataApi == STD_ON)
extern Can_PduType CanIf_RxBuffer[CanIfMaxRxPduCfg];
#endif

#endif /* CAN_IF_CANIF_CBK_H_ */
