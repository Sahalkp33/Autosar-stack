/*
 * PduR_Com.c
 *
 *  Created on: May 12, 2026
 *      Author: sahal
 */

#include "Com.h"
#include "Com_Cfg.h"
#include "Compiler.h"

uint8_t Check_Flag = 0;

void Com_RxIndication(PduIdType RxPduId,const PduInfoType* PduInfoPtr)
{
	if(Com_CurrentState == COM_UNINIT)
	{
#if(COM_DEV_ERROR_DETECT == STD_ON)
		Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RxIndication_SID, COM_E_UNINIT);
#endif
	}
	else if(RxPduId >= ComMaxIpduCount)
	{
#if(COM_DEV_ERROR_DETECT == STD_ON)
		Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RxIndication_SID, COM_E_PARAM);
#endif
	}
	else if(PduInfoPtr == NULL_PTR)
	{
#if(COM_DEV_ERROR_DETECT == STD_ON)
		Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RxIndication_SID, COM_E_PARAM_POINTER);
#endif
	}
	else
	{
		uint8_t i;
		void (*NotificationAddress)(void);
		ComSignal* signal_per_pdu;

		if(Com.ComIpdu[RxPduId].ComIpduDirection == RECIEVE)
		{
			PDU[RxPduId].SduLength = PduInfoPtr->SduLength;
			PDU[RxPduId].SduDataPtr = PduInfoPtr->SduDataPtr;
			PDU[RxPduId].MetaDataPtr = PduInfoPtr->MetaDataPtr;

			if(Com.ComIpdu[RxPduId].ComIpduSignalProcessing == DEFERRED)
			{
				PDU_INDEX = 1;
				Check_Flag = 1;
			}
			else
			{
				uint8_t signal_index = Com.ComIpdu[RxPduId].ComIpduSignalRef[RxPduId]->ComHandleId;
				signal_per_pdu = Com.ComIpdu[RxPduId].ComIpduSignalRef[RxPduId];

				for(i=0;i<PDU[RxPduId].SduLength;i++)
				{
					SignalObject[signal_index+i] = (PDU[RxPduId].SduDataPtr)[i];
					NotificationAddress = signal_per_pdu->ComNotification;
					NotificationAddress();
					signal_per_pdu++;
				}

			}


		}
		else
		{
            /*Do Nothing*/
		}
	}
}
