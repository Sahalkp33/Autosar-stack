/*
 * PduR_CanIf.c
 *
 *  Created on: May 12, 2026
 *      Author: sahal
 */

#include "PduR_Cfg.h"
#include "PduR_CanIf.h"
#include "PduR.h"
#include "Std_Types.h"


#if(PduRTransmissionConfirmation == STD_ON)
void PduR_CanIfTxConfirmation(PduIdType TxPduId)
{
	uint8_t RoutingPathIndex;
	PduRRoutingPaths* RoutingPathPointer = NULL_PTR;
	PduIdType UpperLayerPduId;
	if(PDUR_UNINIT == PduRCurrent_State)
	{
#if(PduRDevErrorDetect == STD_ON)
		Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CANIFTXCONFIRMATION_SID, PDUR_E_UNINIT);
#endif
	}
	else if(TxPduId >= PduR_MaxPduId)
	{
#if(PduRDevErrorDetect == STD_ON)
		Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CANIFTXCONFIRMATION_SID, PDUR_E_PDU_ID_INVALID);
#endif
	}
	else
	{
		for(RoutingPathIndex=0;RoutingPathIndex<PduRMaxRoutingPathCount;RoutingPathIndex++)
		{
			RoutingPathPointer = (PduRRoutingPaths*)&PduR_Configuration.PduRRoutingPaths[RoutingPathIndex];
			if(TxPduId == RoutingPathPointer->PduRDestPdu.PduRDestPduHandleId)
			{
				UpperLayerPduId = RoutingPathPointer->PduRSrcPdu.PduRSrcPduHandleId;

				switch(RoutingPathPointer->PduRSrcPdu.PduRSrcModule)
				{
				case PDUR_COM:
				{
					Com_TxConfirmation(UpperLayerPduId);
					break;
				}
				default:
					break;
				}
				break;
			}
		}
	}
}
#endif

void PduR_CanIfRxIndication(PduIdType RxPduId,PduInfoType* PduInfoPtr)
{
	uint8_t RoutingPathIndex;
	PduRRoutingPaths* RoutingPathPointer = NULL_PTR;
	PduIdType UpperLayerPduId;
	if(PDUR_UNINIT == PduRCurrent_State)
	{
#if(PduRDevErrorDetect == STD_ON)
		Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CANIFRXINDICATION_SID, PDUR_E_UNINIT);
#endif
	}
	else if(RxPduId >= PduR_MaxPduId)
	{
#if(PduRDevErrorDetect == STD_ON)
		Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CANIFRXINDICATION_SID, PDUR_E_PDU_ID_INVALID);
#endif
	}
	else
	{
		for(RoutingPathIndex=0;RoutingPathIndex<PduRMaxRoutingPathCount;RoutingPathIndex++)
		{
			RoutingPathPointer = (PduRRoutingPaths*)&PduR_Configuration.PduRRoutingPaths[RoutingPathIndex];
			if(RxPduId == RoutingPathPointer->PduRDestPdu.PduRDestPduHandleId)
			{
				UpperLayerPduId = RoutingPathPointer->PduRSrcPdu.PduRSrcPduHandleId;

				switch(RoutingPathPointer->PduRSrcPdu.PduRSrcModule)
				{
				case PDUR_COM:
				{
					Com_TxConfirmation(UpperLayerPduId);
					break;
				}
				default:
					break;
				}
				break;
			}
		}
	}
}

