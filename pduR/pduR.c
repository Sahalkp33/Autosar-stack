/*
 * pduR.c
 *
 *  Created on: Apr 16, 2026
 *      Author: sahal
 */

#include "PduR_Cfg.h"
#include "PduR_Types.h"
#include "PduR.h"
#include "Det.h"

#include <stdint.h>

PduR_StateType PduR_Current_State = PDUR_UNINIT;

void PduR_Init(const PduR_ConfigType* ConfigPtr)
{
	if(NULL_PTR == ConfigPtr)
	{
#if( PduRDevErrorDetect == STD_ON  )
        Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_INIT_SID, PDUR_E_PARAM_POINTER);
#endif
	}
	else if(ConfigPtr != &PduR_Configuration)
	{
#if( PduRDevErrorDetect == STD_ON  )
        Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_INIT_SID, PDUR_E_UNINIT);
#endif
	}
	else
	{
		PduRCurrent_State = PDUR_ONLINE;
	}
}

Std_ReturnType pduR_ComTransmit(uint16_t TxPduId,PduInfoType* PduInfoPtr)
{
	Std_ReturnType PduR_ComTransmitReturn;

	if(PDUR_UNINIT == PduRCurrent_State)
	{
#if( PduRDevErrorDetect == STD_ON  )
        Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_COMTRANSMIT_SID, PDUR_E_UNINIT);
#endif
	}
	else if(TxPduId >= PduR_MaxPduId)
	{
#if( PduRDevErrorDetect == STD_ON  )
        Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_COMTRANSMIT_SID, PDUR_E_PDU_ID_INVALID);
#endif
	}
	else if(NULL_PTR == PduInfoPtr)
	{
#if( PduRDevErrorDetect == STD_ON  )
        Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_COMTRANSMIT_SID, PDUR_E_PARAM_POINTER);
#endif
	}
	else
	{
		uint8_t iter;
		for(iter=0;iter<PduRMaxRoutingPathCount;iter++)
		{
			if(TxPduId == PduR_Configuration.PduRRoutingPaths[iter].PduRSrcPdu.PduRSrcPduHandleId)
			{
				uint16_t Pdu_ID = PduR_Configuration.PduRRoutingPaths[iter].PduRDestPdu.PduRDestPduRef->CanIfTxPduId;
				PduR_ComTransmitReturn = CanIf_Transmit(Pdu_ID,PduInfoPtr);
			}
		}
	}

	return PduR_ComTransmitReturn;

}
