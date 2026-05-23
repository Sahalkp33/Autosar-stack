/*
 * CanIf_Cbk.c
 *
 *  Created on: May 6, 2026
 *      Author: sahal
 */

#include "CanIf_Cbk.h"

extern HOH_HRH_MAP[HRH_NUMBER];

/*Rx Notification Flags*/
#if(CanIfPublicReadRxPduNotifyStatusApi == STD_ON)
CanIf_NotifStatusType CanIf_RxNotificationFlag[CanIfMaxRxPduCfg];
#endif

/*Tx Notification Flags*/
#if(CanIfPublicReadTxPduNotifyStatusApi == STD_ON)
CanIf_NotifStatusType CanIf_TxNotificationFlag[CanIfMaxTxPduCfg];
#endif

#if(CanIfPublicReadRxPduDataApi == STD_ON)
Can_PduType CanIf_RxBuffer[CanIfMaxRxPduCfg];
#endif


void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr)
{
	CanIfRxPduCfg* RxPDU = NULL_PTR;
	CanIfHrhRangeCfg* RxPDU_Range = NULL_PTR;
	CanIfHrhCfg* HRH_Ptr = NULL_PTR;

	uint8_t RxPDU_Index,HOH_Index,HRH_Index;

	if (CANIF_UNINIT == CanIfCurrent_State)
	{
#if(STD_ON == CanIfDevErrorDetect)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_SID, CANIF_E_UNINIT);
#endif
	}
	else if(Mailbox->Hoh > CAN_HOH_NUM)
	{
#if(STD_ON == CanIfDevErrorDetect)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_SID, CANIF_E_PARAM_HOH);
#endif
	}
	else if( (Mailbox->CanId > CANNIF_EXTENDED_MAX) || (Mailbox->CanId > CANNIF_STANDARD_MAX) )
	{
#if(STD_ON == CanIfDevErrorDetect)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_SID, CANIF_E_PARAM_CANID);
#endif
	}
	else if( (NULL_PTR == Mailbox) || (NULL_PTR == PduInfoPtr))
	{
#if(STD_ON == CanIfDevErrorDetect)
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_SID, CANIF_E_PARAM_POINTER);
#endif
	}
	else
	{
		RxPDU = (CanIfRxPduCfg*)&CanIf_Configuration.CanIfInitConfig.CanIfRxPduCfg;

		uint8_t RECIEVED_PDU_INDEX;
		for(RECIEVED_PDU_INDEX=0;RECIEVED_PDU_INDEX<CanIfMaxRxPduCfg;RECIEVED_PDU_INDEX)
		{
			if(Mailbox->CanId == CanIf_Configuration.CanIfInitConfig.CanIfRxPduCfg[RECIEVED_PDU_INDEX].CanIfRxPduCanId)
			{
				RxPDU_Index = RECIEVED_PDU_INDEX;
				break;
			}
			else
			{

			}
		}

		HRH_Ptr = CanIf_Configuration.CanIfInitConfig.CanIfRxPduCfg[RxPDU_Index].CanIfRxPduHrhIdRef;
		HOH_Index = HRH_Ptr->CanIfHrhIdSymRef->CanObjectId;

		uint8_t iter=0;
		for(iter=0;iter<HRH_NUMBER;iter++)
		{
			if(HOH_HRH_MAP[iter] == HOH_Index)
			{
				HRH_Index = iter;
				break;
			}
			else
			{

			}
		}

		RxPDU_Range = (CanIfHrhRangeCfg*)&CanIf_Configuration.CanIfInitConfig.CanIfInitHohCfg[CAN_INSTANCE_ID].CanIfHrhCfg[HRH_Index].CanIfHrhRangeCfg;

		if(BASIC == RxPDU->CanIfRxPduHrhIdRef->CanIfHrhIdSymRef->CanHandleType)
		{
			if(TRUE == RxPDU->CanIfRxPduHrhIdRef->CanIfHrhSoftwareFilter)
			{
				if( ((Mailbox->CanId & RxPDU->CanIfRxPduCanIdMask) >= RxPDU_Range->CanIfHrhRangeRxPduLowerCanId ) && (( Mailbox->CanId & RxPDU->CanIfRxPduCanIdMask) <= RxPDU_Range->CanIfHrhRangeRxPduUpperCanId ) )
				{
#if(CanIfPrivateDataLengthCheck == STD_ON)
					if(PduInfoPtr->SduLength > RxPDU->CanIfRxPduDataLength)
					{
#if(CanIfDevErrorDetect == STD_ON)
						Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_SID,CANIF_E_INVALID_DATA_LENGTH);
#endif
					}
					else
#endif
					{
#if(CanIfPublicReadRxPduDataApi == STD_ON)
						if(RxPDU->CanIfRxPduReadData == TRUE)
						{
							CanIf_RxBuffer[RxPDU_Index].id = Mailbox->CanId;
							CanIf_RxBuffer[RxPDU_Index].length = PduInfoPtr->SduLength;
							CanIf_RxBuffer[RxPDU_Index].sdu = PduInfoPtr->SduDataPtr;
						}
						else
						{

						}
#endif
					}
#if(CanIfPublicReadRxPduNotifyStatusApi == STD_ON)
					if(RxPDU->CanIfRxPduReadNotifyStatus)
					{
						CanIf_RxNotificationFlag[RxPDU_Index] = CANIF_TX_RX_NOTIFICATION;
					}
					else
					{

					}
#endif
					switch(RxPDU->CanIfRxPduUserRxIndicationName)
					{
					case PDUR:
					{
						PduInfoType RxPduPDUR;
						RxPduPDUR.SduLength = PduInfoPtr->SduLength;
						RxPduPDUR.SduDataPtr = PduInfoPtr->SduDataPtr;
						RxPduPDUR.MetaDataPtr = PduInfoPtr->MetaDataPtr;
						PduR_CanIfRxIndication(RxPDU_Index,&RxPduPDUR);
						break;
					}
					default:
						break;
					}
				}
				else
				{

				}
			}
		}
		else
		{
			if( (Mailbox->CanId & RxPDU->CanIfRxPduCanIdMask) == (RxPDU->CanIfRxPduCanId & RxPDU->CanIfRxPduCanIdMask) )
			{
#if(CanIfPrivateDataLengthCheck == STD_ON)
				if(PduInfoPtr->SduLength > RxPDU->CanIfRxPduDataLength)
				{
#if(CanIfDevErrorDetect == STD_ON)
					Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_SID,CANIF_E_INVALID_DATA_LENGTH);
#endif
				}
				else
#endif
				{
#if(CanIfPublicReadRxPduDataApi == STD_ON)
					if(RxPDU->CanIfRxPduReadData == TRUE)
					{
						CanIf_RxBuffer[RxPDU_Index].id = Mailbox->CanId;
						CanIf_RxBuffer[RxPDU_Index].length = PduInfoPtr->SduLength;
						CanIf_RxBuffer[RxPDU_Index].sdu = PduInfoPtr->SduDataPtr;
					}
					else
					{

					}
#endif
				}
#if(CanIfPublicReadRxPduNotifyStatusApi == STD_ON)
				if(RxPDU->CanIfRxPduReadNotifyStatus)
				{
					CanIf_RxNotificationFlag[RxPDU_Index] = CANIF_TX_RX_NOTIFICATION;
				}
				else
				{

				}
#endif
				switch(RxPDU->CanIfRxPduUserRxIndicationName)
				{
				case PDUR:
				{
					PduInfoType RxPduPDUR;
					RxPduPDUR.SduLength = PduInfoPtr->SduLength;
					RxPduPDUR.SduDataPtr = PduInfoPtr->SduDataPtr;
					RxPduPDUR.MetaDataPtr = PduInfoPtr->MetaDataPtr;
					PduR_CanIfRxIndication(RxPDU_Index,&RxPduPDUR);
					break;
				}
				default:
					break;
				}
			}
			else
			{

			}


		}





	}
}

#if(PduRTxConfirmation == STD_ON)
void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
	if(CanIfCurrent_State == CANIF_UNINIT)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TX_CONFIRMATION_SID, CANIF_E_UNINIT);
#endif
	}
	else if(CanTxPduId > CanIfMaxTxPduCfg)
	{
#if(CanIfDevErrorDetect == STD_ON)
		Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TX_CONFIRMATION_SID, CANIF_E_PARAM_LPDU);
#endif
	}
	else
	{
		CanIfTxPduCfg* TxPDU_Ptr = NULL_PTR;
		TxPDU_Ptr = (CanIfTxPduCfg*)&CanIf_Configuration.CanIfInitConfig.CanIfTxPduCfg[CanTxPduId];

#if(CanIfPublicReadTxPduNotifyStatusApi == STD_ON)
		if(CanIfPublicReadTxPduNotifyStatusApi == TRUE)
		{
			CanIf_TxNotificationFlag[CanTxPduId] = CANIF_TX_RX_NOTIFICATION;
		}
#endif
		if(TxPDU_Ptr->CanIfTxPduUserTxConfirmationUL == PDUR)
		{
			PduR_CanIfTxConfirmation(CanTxPduId);
		}
		else
		{

		}

	}



}
#endif






