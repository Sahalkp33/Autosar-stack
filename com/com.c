/*
 * com.c
 *
 *  Created on: Apr 8, 2026
 *      Author: sahal
 */
#include<stdint.h>
#include "Com_Cfg.h"
#include "ComStack_Types.h"
#include "Com.h"
#include "Compiler.h"
#include "PduR_Com.h"
#include "Det.h"


Com_StatusType Com_CurrentState = COM_UNINIT;

uint8_t g_tick = 0;

uint8_t PDU_INDEX = 0;

PduInfoType PDU[ComMaxIpduCount];
ComSignal SignalBuffer[MAX_NUM_OF_SIGNALS];
uint8_t SignalObject[MAX_NUM_OF_SIGNALS];

uint8_t ComTriggerFlag[ComMaxIpduCount] = {0};

void Com_init(const Com_ConfigType* config)
{
	uint8_t byte_counter,pdu_counter,signal_counter;

	for(pdu_counter=0;pdu_counter<ComMaxIpduCount;pdu_counter++)
	{
		PDU[pdu_counter].SduLength = PDU_LEN_IN_BYTES;
		PDU[pdu_counter].SduDataPtr = (uint8_t*)&(config->ComIpdu[pdu_counter].ComTxIpdu.ComTxIpduUnusedAreasDefault);
		for(byte_counter=0;byte_counter<PDU_LEN_IN_BYTES;byte_counter++)
			PDU[pdu_counter].SduDataPtr[byte_counter] = config->ComIpdu[pdu_counter].ComTxIpdu.ComTxIpduUnusedAreasDefault;

		for(signal_counter=0;signal_counter<MAX_NUM_OF_SIGNALS;signal_counter++)
		{
			SignalBuffer[signal_counter].ComSignalInitValue = config->ComSignal[pdu_counter].ComSignalInitValue;
			SignalObject[signal_counter] = SignalBuffer[signal_counter].ComSignalInitValue;
		}
	}

}

void pdu_concatenate(uint8_t Pduid)
{
	uint8_t base_id = Com.ComIpdu[Pduid].ComIpduSignalRef[Pduid]->ComHandleId;

	for(uint8_t signal_counter=0;signal_counter<PDU_LEN_IN_BYTES;signal_counter++)
	{
		uint8_t signal_id = base_id + signal_counter;
		uint8_t index = Com.ComSignal[signal_id].ComBitPosition / 8;
		if(LITTLE_ENDIAN == Com.ComSignal[signal_id].ComSignalEndianness)
		{
			PDU[Pduid].SduDataPtr[index] = SignalObject[signal_id];
		}
		else
		{
			PDU[Pduid].SduDataPtr[8-index-1] = SignalObject[signal_id];
		}
	}
}

void ComSendSignal(uint16_t SignalId,void* SignalDataPtr)
{
	uint16_t SignalID = Com.ComIpdu[SignalId].ComIpduHandleId;

	for(uint8_t iter=0;iter<Com.ComSignal[SignalID].ComSignalLength;iter++)
		SignalObject[SignalID + iter] = ((uint8_t *)SignalDataPtr)[iter];

	ComTriggerFlag[SignalID/PDU_LEN_IN_BYTES] = 1;
}

uint8_t Com_RecieveSignal(Com_SignalIdType SignalId,void* SignalDataPtr)
{
	uint8_t Com_RecieveSignal_Return;

	if(Com_CurrentState == COM_UNINIT)
	{
#if(COM_DEV_ERROR_DETECT == STD_ON)
		Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID,Com_Recieve_Signal_SID, COM_E_UNINIT);
#endif
	}
	else if(SignalId >= MAX_NUM_OF_SIGNALS)
	{
#if(COM_DEV_ERROR_DETECT == STD_ON)
		Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID,Com_Recieve_Signal_SID, COM_E_PARAM);
#endif
	}
	else if(SignalDataPtr == NULL_PTR)
	{
#if(COM_DEV_ERROR_DETECT == STD_ON)
		Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID,Com_Recieve_Signal_SID, COM_E_PARAM);
#endif
	}
	else
	{
		uint8_t* Signal_Value;
		Signal_Value = &SignalObject[SignalId];

		uint8_t iter;
		for(iter=0;iter<Com.ComSignal[SignalId].ComSignalLength;iter++)
		{
			((uint8_t*)SignalDataPtr)[iter] = Signal_Value[iter];
		}
		Com_RecieveSignal_Return = E_OK;
	}
	return Com_RecieveSignal_Return;
}


void ComMainFunctionTx(void)
{
	uint8_t pdu_counter,signal_counter;
	g_tick++;
	if(g_tick>(ComTxModeTimePeriod_Max_Value*100))
		g_tick = 1;

	for(pdu_counter=0;pdu_counter<ComMaxIpduCount;pdu_counter++)
	{
		if(COM_SEND == Com.ComIpdu[pdu_counter].ComIpduDirection)
		{
			if(PERIODIC_Tx == Com.ComIpdu[pdu_counter].ComTxIpdu.TxMode.TxModeMode)
			{
				if(g_tick*ComTxTimeBase*1000 >= Com.ComIpdu[pdu_counter].ComTxIpdu.TxMode.TxModeTimePeriod)
				{
					if((uint8_t)(g_tick*ComTxTimeBase*1000) % (uint8_t)(Com.ComIpdu[pdu_counter].ComTxIpdu.TxMode.TxModeTimePeriod*1000) == 0 )
					{
						pdu_concatenate(pdu_counter);
						pduR_ComTransmit(Com.ComIpdu[pdu_counter].ComIpduHandleId,&PDU[pdu_counter]);
					}
				}
			}
			else if(DIRECT_Tx == Com.ComIpdu[pdu_counter].ComTxIpdu.TxMode.TxModeMode)
			{
				for(signal_counter=0;signal_counter<PDU_LEN_IN_BYTES;signal_counter++)
				{
					if(PENDING == Com.ComSignal[Com.ComIpdu[pdu_counter].ComIpduSignalRef[pdu_counter]->ComHandleId + signal_counter].ComTransferProperty)
					{

					}
					else
					{
						if(ComTriggerFlag[pdu_counter] == 1)
						{
							ComTriggerFlag[pdu_counter] = 0;
							pdu_concatenate(pdu_counter);
							pduR_ComTransmit(Com.ComIpdu[pdu_counter].ComIpduHandleId,&PDU[pdu_counter]);

							break;
						}
					}
				}
			}
		}
	}


}

void Com_MainFunctionRx(void)
{
	void(*NotificationAddress)(void);
	ComSignal* signal_per_pdu;
	uint8_t iter;
	if(Com_CurrentState == COM_UNINIT)
	{
#if(COM_DEV_ERROR_DETECT == STD_ON)
		Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID,Com_MainFunctionRx_SID, COM_E_UNINIT);
#endif
	}
	else
	{
		if(Check_Flag == 1)
		{
			Check_Flag=0;
			uint8_t signal_index = Com.ComSignal[PDU_INDEX].ComHandleId;
			signal_per_pdu = Com.ComIpdu[PDU_INDEX].ComIpduSignalRef[PDU_INDEX];
			for(iter=0;iter<PDU[PDU_INDEX].SduLength;iter++)
			{
				SignalObject[signal_index + iter] = (PDU[PDU_INDEX].SduDataPtr)[iter];
				NotificationAddress = signal_per_pdu->ComNotification;
				NotificationAddress();
				signal_per_pdu++;

			}
		}
	}
}







