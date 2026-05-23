/*
 * CanIf.c
 *
 *  Created on: Apr 18, 2026
 *      Author: sahal
 */

#include "CanIf.h"
#include "Det.h"

CanIfTxPduCfg* CanIf_GetTxPdu(PduIdType TxPduId);

/*Status variable to check if the module is initialized or not*/
CanIf_State CanIfCurrent_State = CANIF_UNINIT;

/*Status variable which holds the state of the CANIF */
Can_ControllerStateType CanIf_ControllerMode[CAN_CONTROLLERS_NUM];

/*Status variable which holds the state of the PDU channel */
CanIf_PduModeType CanIf_ChannelPduMode[CAN_CONTROLLERS_NUM];

/*An array to map the HRH to the HOH*/
uint8_t HOH_HRH_MAP[HRH_NUMBER];



void CanIf_Init(CanIf_ConfigType* ConfigPtr)
{
	if(NULL_PTR == ConfigPtr)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INSTANCE_ID, CANIF_E_UNINIT);
#endif
	}
	else
	{
		uint8_t count;
		if(CANIF_UNINIT == CanIfCurrent_State)
		{
#if(CanIfPublicReadRxPduNotifyStatusApi)
			for(count=0;count<CanIfMaxRxPduCfg;count++)
			{
				CanIf_TxNotificationFlag[count] = CANIF_NO_NOTIFICATION;
			}
#endif
#if(CanIfPublicReadRxPduNotifyStatusApi)
			for(count=0;count<CanIfMaxRxPduCfg;count++)
			{
				CanIf_RxNotificationFlag[count] = CANIF_NO_NOTIFICATION;
			}
#endif

			for(count=0;count<CAN_CONTROLLERS_NUM;count++)
			{
				CanIf_ControllerMode[count] = CAN_CS_STOPPED;
				CanIf_ChannelPduMode[count] = CANIF_OFFLINE;
			}

		}
		else
		{
            /*MISRA: Do Nothing*/
		}
		uint8_t HRH_INDEX = 0;
		for(count=0;count<CAN_HOH_NUM;count++)
		{
			if(Can_Configuration.CanConfigSet.CanHardwareObject[count].CanObjectType == RECIEVE)
			{
				HOH_HRH_MAP[HRH_INDEX] = count;
				HRH_INDEX++;
			}
		}
		CanIfCurrent_State = CANIF_READY;
	}


}

void CanIf_Deinit(void)
{
	uint8_t count;
	for(count=0;count<CAN_CONTROLLERS_NUM;count++)

	{
		CanIf_ControllerMode[count] = CAN_CS_UNINIT;
		CanIf_ChannelPduMode[count] = CANIF_OFFLINE;
	}
	CanIfCurrent_State = CANIF_UNINIT;


}

CanIfTxPduCfg* CanIf_GetTxPdu(PduIdType TxPduId)
{
	if(TxPduId<CanIfMaxTxPduCfg)
	{
		return (CanIfTxPduCfg*)&CanIf_Configuration.CanIfInitConfig.CanIfTxPduCfg[TxPduId];
	}
	else
	{
		return NULL_PTR;
	}
}

void CanIf_Transmit(uint8_t TxPduId,PduInfoType* PduInfoPtr)
{
	uint32_t TxPDU_CanId;
	Std_ReturnType CanIf_Transmit_Return;
	Std_ReturnType Can_Write_Return;
	Can_PduType Can_PduData;
	CanIfTxPduCfg* TxPDU = NULL_PTR;
	uint8_t Controller_Id;
	CanIfTxPduCanIdType Pdu_CanId_Type = TxPDU->CanIfTxPduCanIdType;

	TxPDU = CanIf_GetTxPdu(TxPduId);

	TxPDU->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlCanCtrlRef->CanControllerId;

	uint8_t Hth = TxPDU->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthIdSymRef->CanObjectId;

	TxPDU_CanId = TxPDU->CanIfTxPduCanId;

	if(CANIF_UNINIT == CanIfCurrent_State)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TRANSMIT_SID, CANIF_E_UNINIT);
#endif
	}
	else if(TxPduId >= CanIfMaxTxPduCfg)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TRANSMIT_SID, CANIF_E_INVALID_TXPDUID);
#endif
	}
	else if(PduInfoPtr == NULL_PTR)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TRANSMIT_SID, CANIF_E_PARAM_POINTER);
#endif
	}
	else if( (  (Pdu_CanId_Type == STANDARD_CAN) || (Pdu_CanId_Type == EXTENDED_CAN) ) && (PduInfoPtr->SduLength > CANIF_STANDARD_MAX_SDU_LENGTH)   )
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TRANSMIT_SID, CANIF_E_DATA_LENGTH_MISMATCH);
#endif
	}
	else if( (  (Pdu_CanId_Type == STANDARD_FD_CAN) || (Pdu_CanId_Type == EXTENDED_FD_CAN) ) && (PduInfoPtr->SduLength > CANIF_FD_MAX_SDU_LENGTH)   )
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TRANSMIT_SID, CANIF_E_DATA_LENGTH_MISMATCH);
#endif
	}
	else if( (PduInfoPtr->SduLength > CANIF_MAX_PDU_LENGTH) && (TxPDU->CanIfTxPduTruncation == FALSE) )
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TRANSMIT_SID, CANIF_E_DATA_LENGTH_MISMATCH);
#endif
	}
	else
	{
		if( (CanIf_ControllerMode[Controller_Id]!=CAN_CS_STARTED) && (( CanIf_ChannelPduMode[Controller_Id] != CANIF_ONLINE)|| (CanIf_ChannelPduMode[Controller_Id]!=CANIF_TX_OFFLINE_ACTIVE)))
		{
			CanIf_Transmit_Return = E_NOT_OK;
		}
		else
		{
			if(TxPDU->CanIfTxPduCanIdType == STANDARD_CAN)
			{
				TxPDU_CanId = TxPDU_CanId | TWO_MSB_STANDARD_MASK;
			}
			else if(TxPDU->CanIfTxPduCanIdType == EXTENDED_CAN)
			{
				TxPDU_CanId = TxPDU_CanId | TWO_MSB_EXTENDED_MASK;
			}
			else
			{
                /*MISRA: Do Nothing*/
			}

			Can_PduData.id = TxPDU_CanId;

			if( (PduInfoPtr->SduLength > CANIF_MAX_PDU_LENGTH) && (TxPDU->CanIfTxPduTruncation == TRUE))
			{
				Can_PduData.length = CANIF_MAX_PDU_LENGTH;
			}
			else
			{
				Can_PduData.length = PduInfoPtr->SduLength;
			}

			Can_PduData.sdu = PduInfoPtr->SduDataPtr;
			Can_PduData.swPduHandle = TxPduId;

			Can_Write_Return = Can_Write(Hth, &Can_PduData);

			if(Can_Write_Return == E_OK)
			{
				CanIf_Transmit_Return = E_OK;
			}
			else
			{
				CanIf_Transmit_Return = E_NOT_OK;
			}
		}
	}


}

Std_ReturnType CanIf_SetControllerMode(uint8_t ControllerId,Can_ControllerStateType ControllerMode)
{
	Can_ControllerStateType Requested_Mode;
	Std_ReturnType Can_SetCOntrollerMode_Return;

	if(CANIF_UNINIT == CanIfCurrent_State)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_SID, CANIF_E_UNINIT);
#endif
	}
	else if(ControllerId >= CAN_CONTROLLERS_NUM)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_SID, CANIF_E_PARAM_CONTROLLERID);
#endif
	}
	else if( (ControllerMode!=CAN_CS_STARTED) && (ControllerMode!=CAN_CS_SLEEP) && (ControllerMode!=CAN_CS_STOPPED))
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_SID, CANIF_E_PARAM_CTRLMODE);
#endif
	}
	else
	{
		switch(ControllerMode)
		{
		case CAN_CS_STOPPED:
			Requested_Mode = CAN_CS_STOPPED;
			CanIf_ChannelPduMode[ControllerId] = CANIF_TX_OFFLINE;
			Can_SetCOntrollerMode_Return = Can_SetControllerMode(ControllerId,Requested_Mode);
			if(Can_SetCOntrollerMode_Return == E_OK)
			{
				CanIf_ControllerMode[ControllerId] = CAN_CS_STOPPED;
			}
			else
			{

			}
			break;
		case CAN_CS_STARTED:
			Requested_Mode = CAN_CS_STARTED;
			Can_SetCOntrollerMode_Return = Can_SetCOntrollerMode(ControllerId,Requested_Mode);
			if(Can_SetCOntrollerMode_Return == E_OK)
			{
				CanIf_ControllerMode[ControllerId] = CAN_CS_STARTED;
			}
			else
			{
                /*MISRA:Do Nothing */
			}
			break;
		default:
			break;
		}
	}
	return Can_SetCOntrollerMode_Return;
}

Std_ReturnType CanIf_SetPduMode(uint8_t ControllerId,CanIf_PduModeType PduModeRequest)
{
	Std_ReturnType Return_State;
	if(CANIF_UNINIT == CanIfCurrent_State)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_SID, CANIF_E_UNINIT);
#endif
	}
	else if(ControllerId>=CAN_CONTROLLERS_NUM)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_SID, CANIF_E_PARAM_CONTROLLERID);
#endif
	}
	else if((PduModeRequest!=CANIF_ONLINE) && (PduModeRequest!=CANIF_OFFLINE) && (PduModeRequest!=CANIF_TX_OFFLINE) && (PduModeRequest!=CANIF_TX_OFFLINE_ACTIVE))
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_SID, CANIF_E_PARAM_PDU_MODE);
#endif
	}
	else
	{
		if(CanIf_ControllerMode[ControllerId] != CAN_CS_STARTED)
		{
			Return_State = E_NOT_OK;
		}
		else
		{
			CanIf_ChannelPduMode[ControllerId] = PduModeRequest;
			Return_State =E_OK;
		}
	}
	return Return_State;
}

#if(CanIfPublicReadTxPduNotifyStatusApi == STD_ON)
CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId)
{
	CanIf_NotifStatusType ReturnState = CANIF_NO_NOTIFICATION;

	if(CanIfCurrent_State == CANIF_UNINIT)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CanIf_ReadTxNotifStatus_SID, CANIF_E_UNINIT);
#endif
	}
	else if(CanIfTxSduId > CanIfMaxTxPduCfg)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CanIf_ReadTxNotifStatus_SID, CANIF_E_INVALID_TXPDUID);
#endif
	}
	else
	{
		const CanIfTxPduCfg* TxPDU_Ptr = NULL_PTR;
		TxPDU_Ptr = (CanIfTxPduCfg*)&CanIf_Configuration.CanIfInitConfig.CanIfTxPduCfg[CanIfTxSduId];

		if(TxPDU_Ptr->CanIfTxPduReadNotifyStatus == TRUE)
		{
			if(CANIF_TX_RX_NOTIFICATION == CanIf_TxNotificationFlag[CanIfTxSduId])
			{
				ReturnState = CANIF_TX_RX_NOTIFICATION;
				CanIf_TxNotificationFlag[CanIfTxSduId] = CANIF_NO_NOTIFICATION;
			}
			else
			{
				ReturnState = CANIF_NO_NOTIFICATION;
			}
		}
		else
		{
			ReturnState = CANIF_NO_NOTIFICATION;
		}

	}
	return ReturnState;
}
#endif

#if(CanIfPublicReadRxPduNotifyStatusApi == STD_ON)
CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId)
{
	CanIf_NotifStatusType ReturnState = CANIF_NO_NOTIFICATION;

	if(CANIF_UNINIT == CanIfCurrent_State)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CanIf_ReadRxNotifStatus_SID, CANIF_E_UNINIT);
#endif
	}
	else if(CanIfRxSduId > CanIfMaxRxPduCfg)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CanIf_ReadRxNotifStatus_SID, CANIF_E_INVALID_RXPDUID);
#endif
	}
	else
	{
		CanIfRxPduCfg* RxPDU_Ptr = NULL_PTR;
		RxPDU_Ptr = (CanIfRxPduCfg*)&CanIf_Configuration.CanIfInitConfig.CanIfRxPduCfg[CanIfRxSduId];

		if(RxPDU_Ptr->CanIfRxPduReadNotifyStatus == TRUE)
		{
			if(CANIF_TX_RX_NOTIFICATION == CanIf_RxNotificationFlag[CanIfRxSduId])
			{
				ReturnState = CANIF_TX_RX_NOTIFICATION;
				CanIf_RxNotificationFlag[CanIfRxSduId] = CANIF_NO_NOTIFICATION;
			}
			else
			{
				ReturnState = CANIF_NO_NOTIFICATION;
			}
		}
		else
		{
			ReturnState = CANIF_NO_NOTIFICATION;
		}
	}

	return ReturnState;
}
#endif

#if(CanIfPublicReadRxPduDataApi == STD_ON)
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId,PduInfoType* CanIfRxInfoPtr)
{
	Std_ReturnType Return_Value;
	uint8_t Controller_Id;

	if(CANIF_UNINIT == CanIfCurrent_State)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Return_Value = Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CanIf_ReadRxPduData_SID, CANIF_E_UNINIT);
#endif
	}
	else if(CanIfRxSduId > CanIfMaxRxPduCfg)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Return_Value = Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CanIf_ReadRxPduData_SID, CANIF_E_INVALID_RXPDUID);
#endif
	}
	else if(CanIfRxInfoPtr == NULL_PTR)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Return_Value = Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CanIf_ReadRxPduData_SID, CANIF_E_PARAM_POINTER);
#endif
	}
	else
	{
		Controller_Id = CanIf_Configuration.CanIfInitConfig.CanIfRxPduCfg[CanIfRxSduId].CanIfRxPduHrhIdRef->CanIfHrhCanCtrlIdRef->CanIfCtrlId;
		if(CanIf_ControllerMode[Controller_Id] |= CAN_CS_STARTED)
		{
			return E_NOT_OK;
		}
		else
		{
			CanIfRxPduCfg* RxPDU_Ptr = NULL_PTR;
			RxPDU_Ptr = (CanIfRxPduCfg*)&CanIf_Configuration.CanIfInitConfig.CanIfRxPduCfg[CanIfRxSduId];
			if(RxPDU_Ptr->CanIfRxPduHrhIdRef->CanIfHrhIdSymRef->CanHandleType == BASIC)
			{
				return E_NOT_OK;
			}
			else
			{
				if(RxPDU_Ptr->CanIfRxPduReadData == TRUE)
				{
					CanIfRxInfoPtr->SduDataPtr = CanIf_RxBuffer[CanIfRxSduId].sdu;
					CanIfRxInfoPtr->SduLength = CanIf_RxBuffer[CanIfRxSduId].length;

					Return_Value = E_OK;
				}
				else
				{
					/*MISRA:Do Nothing*/
				}

			}
		}
	}
	return Return_Value;
}
#endif



