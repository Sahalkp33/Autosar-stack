/*
 * Can_IRQ.c
 *
 *  Created on: May 18, 2026
 *      Author: sahal
 */


#include "can.h"

volatile boolean MSG_Object_INT_Flag=0;
volatile uint8_t MSG_Number_INT[32]={0};
volatile boolean Error_Flag=0;
volatile uint8_t Error_Status=0;
volatile uint32_t Recieve_Count=0;
volatile uint32_t Transmit_Count=0;

Can_PduType Temp_Buffer;



void CAN0_Handler(void)
{
	uint8_t HOH;
	Can_HwType CanIfRx_Mailbox;
	PduInfoType CanIfRx_PduInfoPtr;

	MSG_Object_INT_Flag=0;
	uint32_t status=REG_VAL(CAN0_BASE,CANINT_OFFSET);

	if(STATUS_INTERRUPT == status)
	{
		Error_Status = REG_VAL(CAN0_BASE,CANSTS_OFFSET);
		CLEAR_BIT(REG_VAL(CAN0_BASE,CANSTS_OFFSET),RXOK_BIT);
		CLEAR_BIT(REG_VAL(CAN0_BASE,CANSTS_OFFSET),TXOK_BIT);
		CLEAR_BIT(REG_VAL(CAN0_BASE,CANSTS_OFFSET),LEC0_BIT);
		CLEAR_BIT(REG_VAL(CAN0_BASE,CANSTS_OFFSET),LEC1_BIT);
		CLEAR_BIT(REG_VAL(CAN0_BASE,CANSTS_OFFSET),LEC2_BIT);

		Error_Flag=1;
	}
	else
	{
		MSG_Number_INT[status-1]=status;
		HOH=0;
		uint8_t Mailbox_Index;
		uint8_t iter=Can_Configuration.CanConfigSet.CanHardwareObject[HOH].CanHardwareObjectCount;
		while(MSG_Number_INT[status-1]>iter)
		{
			HOH++;
			iter+=Can_Configuration.CanConfigSet.CanHardwareObject[HOH].CanHardwareObjectCount;
		}
		for(iter=0;iter<Can_Configuration.CanConfigSet.CanHardwareObject[HOH].CanHardwareObjectCount;iter++)
		{
			if(Object_Check[CAN0_CONTROLLER_ID][HOH][iter].Mailbox == MSG_Number_INT[status-1])
			{
				Mailbox_Index=iter;
			}
			else
			{
				/*Misra C:Do nothing*/
			}
		}

		while(BIT_IS_SET(REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET),BUSY_BIT))
		{

		}

		/*Clear Intpnd bit*/
		REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET) = 0x8;
		REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET) = MSG_Number_INT[status-1] & MNUM_MASK;

		while(BIT_IS_SET(REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET),BUSY_BIT))
		{

		}

		if(Can_Configuration.CanConfigSet.CanHardwareObject[HOH].CanObjectType  == TRANSMIT)
		{
			Object_Check[CAN0_CONTROLLER_ID][HOH][Mailbox_Index].Check = Confirmed;
			CanIf_TxConfirmation(Object_Check[CAN0_CONTROLLER_ID][HOH][Mailbox_Index].PduId);
			Transmit_Count++;
		}
		else if(Can_Configuration.CanConfigSet.CanHardwareObject[HOH].CanObjectType == RECIEVE)
		{
			if(Object_Check[CAN0_CONTROLLER_ID][HOH][Mailbox_Index].Check == Confirmed)
			{
				Object_Check[CAN0_CONTROLLER_ID][HOH][Mailbox_Index].Check = Unconfirmed;
				Can_Message_Recieve(CAN0_CONTROLLER_ID, status, &Temp_Buffer);
				CanIfRx_Mailbox.CanId = Temp_Buffer.id;
				CanIfRx_Mailbox.ControllerId = CAN0_CONTROLLER_ID;
				CanIfRx_Mailbox.Hoh = Can_Configuration.CanConfigSet.CanHardwareObject[HOH].CanObjectId;

				CanIfRx_PduInfoPtr.SduDataPtr = Temp_Buffer.sdu;
				CanIfRx_PduInfoPtr.SduLength = Temp_Buffer.length;

				CanIf_RxIndication(&CanIfRx_Mailbox, &CanIfRx_PduInfoPtr);

				Object_Check[CAN0_CONTROLLER_ID][HOH][Mailbox_Index].Check = Confirmed;
			}
			else
			{
				/*Misra C:Do nothing*/
			}

			Recieve_Count++;
		}

		MSG_Object_INT_Flag = 1;
		Error_Flag = 0;


	}


}
