/*
 * can.c
 *
 *  Created on: Apr 6, 2026
 *      Author: sahal
 */

#include <stdint.h>
#include "can.h"
#include "common_macros.h"
#include "Can_Reg.h"
#include "Can_Cfg.h"

#include "ComStack_Types.h"

Message_Confirmation Object_Check[CAN_CONTROLLERS_NUM][CAN_HOH_NUM][MAX_HWOBJECT_COUNT];

static register_val registers[CAN_HOH_NUM] = {0};

void can_init(const Can_ConfigType *Config)
{
	/*1.Error Check */

	volatile uint8_t delay = 0;
	SET_BIT(SYCTL_RCGC0_REG,CAN0_BIT);
	delay = SYCTL_RCGC0_REG;

	/*2.Set Init Bit */
	SET_BIT(REG_VAL(CAN0_BASE,CANCTL_OFFSET),INIT_BIT);

	/*3.Set the Baudrate */
	SET_BIT(REG_VAL(CAN0_BASE,CANCTL_OFFSET),CCE_BIT);
	REG_VAL(CAN0_BASE,CANBIT_OFFSET) |= ((Config->CanConfigSet.CanController[CAN0_CONTROLLER_ID].CanControllerBaudRateConfig.PropSeg + Config->CanConfigSet.CanController[CAN0_CONTROLLER_ID].CanControllerBaudRateConfig.PhaseSeg1 - 1 ) << 8 );
	REG_VAL(CAN0_BASE,CANBIT_OFFSET) |= ((Config->CanConfigSet.CanController[CAN0_CONTROLLER_ID].CanControllerBaudRateConfig.PhaseSeg2 - 1) << 12);
	REG_VAL(CAN0_BASE,CANBIT_OFFSET) |= ((Config->CanConfigSet.CanController[CAN0_CONTROLLER_ID].CanControllerBaudRateConfig.SyncJumpWidth - 1) << 6);
	REG_VAL(CAN0_BASE,CANBIT_OFFSET) |= 1 << 0;
	REG_VAL(CAN0_BASE,CANBRPE_OFFSET) |= 0 << 0;

	while(BIT_IS_SET(REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET),BUSY_BIT))
	{
		/*Wait for IF1 to become free */
	}

	/*SET WRNRD Bit to specify write to CANIFnMASK */
	SET_BIT(REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET),WRNRD_BIT);
	/*Set ARB Bit to transfer ID,DIR,XTD AND MSGVAL OF Message object into IF Registers */
	SET_BIT(REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET),ARB_BIT);
	/*Set CNTRL Bit to transfer Control Bits into IF Registers */
	SET_BIT(REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET),CONTROL_BIT);
	/*Set Mask Bit to Transfer IDMASK */
	SET_BIT(REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET),MASK_BIT);

	/*Clear MSGVAL Bit in the arbitration Register */
	/*Clear All the Flags in CANIFnMCTL Register */
	REG_VAL(CAN0_BASE,CANIF1ARB2_OFFSET) = 0;
	REG_VAL(CAN0_BASE,CANIF1MCTL_OFFSET) = 0;

	/*Set CLRINTPND Bit in the CANIFnCMSK to Clear INTPND bit in CANIFnMCTL */
	/*Set NEWDATA Bit in CANIFnCMSK to Clear NEWDATABit in CANWDaN Reg */
	SET_BIT(REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET),NEWDATA_BIT);
	SET_BIT(REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET),CLRINTPND_BIT);

	uint8_t iter = 0;
	uint8_t Object_Count_iter = 0;

	for(iter=0;iter<CAN_HOH_NUM;iter++)
	{
		uint8_t Check_iter = 0;
		for(Check_iter=0;Check_iter<=Config->CanConfigSet.CanHardwareObject[iter].CanHardwareObjectCount - 1;Check_iter++)
		{
			Object_Check[CAN0_CONTROLLER_ID][iter][Check_iter].Check = Confirmed;
			Object_Count_iter++;
			Object_Check[CAN0_CONTROLLER_ID][iter][Check_iter].Mailbox = Object_Count_iter;
		}

		while(BIT_IS_SET(REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET),BUSY_BIT))
		{
			/*Wait for busy bit to Clear */
		}
		uint8_t iter2 = 0;
		for(iter2=0;iter2<=Config->CanConfigSet.CanHardwareObject[iter].CanHardwareObjectCount - 1;iter2++)
		{
			REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET) |= Object_Check[CAN0_CONTROLLER_ID][iter][iter2].Mailbox & MNUM_MASK;
		}
	}

	/*Clear any pending Status Interrupt */
	REG_VAL(CAN0_BASE,CAN_STS_OFFSET);

	for(iter=0;iter<CAN_HOH_NUM;iter++)
	{
		for(uint8_t iter2=0;iter2<=Config->CanConfigSet.CanHardwareObject[iter].CanHardwareObjectCount;iter2++)
		{
			uint16_t u16CmdMaskReg = 0;
			uint16_t u16MaskReg0 = 0,u16MaskReg1 = 0;
			uint16_t u16ArbReg0 = 0,u16ArbReg1 = 0;
			uint16_t u16MsgCntrl = 0;
			uint8_t BusExtendedId = 0;

			while(BIT_IS_SET(REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET),BUSY_BIT))
			{
				/*Wait for busy bit to Clear */
			}

			if(Config->CanConfigSet.CanHardwareObject[iter].CanIdType == ID_EXTENDED)
				BusExtendedId = 1;
			else
				BusExtendedId = 0;

			SET_BIT(u16CmdMaskReg,WRNRD_BIT);
			SET_BIT(u16CmdMaskReg,CONTROL_BIT);
			SET_BIT(u16CmdMaskReg,DATAA_BIT);
			SET_BIT(u16CmdMaskReg,DATAB_BIT);

			/*Configure Transmit HOH */
			if(Config->CanConfigSet.CanHardwareObject[iter].CanObjectType == TRANSMIT)
				SET_BIT(u16ArbReg1,DIR_BIT);
			else if(Config->CanConfigSet.CanHardwareObject[iter].CanObjectType == RECIEVE)
				u16ArbReg1 = 0;
			else
			{
				/*Do Nothing*/
			}

			if(BusExtendedId)
			{
				u16MaskReg0 = (uint16_t)(Config->CanConfigSet.CanHardwareObject[iter].CanHwFilter.CanHwFilterMask & 16);
				u16MaskReg1 = (uint16_t)((Config->CanConfigSet.CanHardwareObject[iter].CanHwFilter.CanHwFilterMask >> 16) & 13 );
			}
			else
			{
				u16MaskReg0 = 0;
				u16ArbReg1 = (uint16_t)((Config->CanConfigSet.CanHardwareObject[iter].CanHwFilter.CanHwFilterMask << 2 )& 13);
			}

			SET_BIT(u16MaskReg1,MXTD_BIT);
			SET_BIT(u16MsgCntrl,UMASK_BIT);
			SET_BIT(u16CmdMaskReg,ARB_BIT);
			SET_BIT(u16CmdMaskReg,MASK_BIT);

			if(BusExtendedId)
			{
				u16ArbReg0 = (uint16_t)(Config->CanConfigSet.CanHardwareObject[iter].CanHwFilter.CanHwFilterMask & 16);
				u16ArbReg1 = (uint16_t)((Config->CanConfigSet.CanHardwareObject[iter].CanHwFilter.CanHwFilterMask >> 16) & 13 );

				SET_BIT(u16ArbReg1,XTD_BIT);
				SET_BIT(u16ArbReg1,MSGVAL_BIT);
			}
			else
			{
				u16ArbReg0 = 0;
				u16ArbReg1 = (uint16_t)((Config->CanConfigSet.CanHardwareObject[iter].CanHwFilter.CanHwFilterMask << 2 )& 13);

				SET_BIT(u16ArbReg1,MSGVAL_BIT);
			}

			if(Config->CanConfigSet.CanHardwareObject[iter].CanObjectType == RECIEVE)
				SET_BIT(u16MsgCntrl,RXIE_BIT);
			else
			{

			}

			if(Config->CanConfigSet.CanHardwareObject[iter].CanObjectType == RECIEVE)
			{
				REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET) |= u16CmdMaskReg;
				REG_VAL(CAN0_BASE,CANIF1MSK1_OFFSET) |= u16MaskReg0;
				REG_VAL(CAN0_BASE,CANIF1MSK2_OFFSET) |= u16MaskReg1;
				REG_VAL(CAN0_BASE,CANIF1ARB1_OFFSET) |= u16ArbReg0;
				REG_VAL(CAN0_BASE,CANIF1ARB2_OFFSET) |= u16ArbReg1;
				REG_VAL(CAN0_BASE,CANIF1MCTL_OFFSET) |= u16MsgCntrl;
			}
			else
			{
				registers[iter].u16CmdMaskReg = u16CmdMaskReg;
				registers[iter].u16MaskReg0 = u16MaskReg0;
				registers[iter].u16MaskReg1 = u16MaskReg1;
				registers[iter].u16ArbReg0 = u16ArbReg0;
				registers[iter].u16ArbReg1 = u16ArbReg1;
				registers[iter].u16MsgCntrl = u16MsgCntrl;
			}
			REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET) |= Object_Check[CAN0_CONTROLLER_ID][iter][iter2].Mailbox & MNUM_MASK;
		}
	}
}

void Can_Write(uint8_t Hth,Can_PduType* PduInfoPtr)
{
	if(FULL == Can_Configuration.CanConfigSet.CanHardwareObject[Hth].CanHandleType)
	{
		uint16_t u16MsgCtrl;
		u16MsgCtrl = 0;

		SET_BIT(u16MsgCtrl,TXRQST_BIT);
		u16MsgCtrl |= PduInfoPtr->length & DLC_MASK;
		SET_BIT(u16MsgCtrl,EOB_BIT);

		if(Can_Configuration.CanConfigSet.CanHardwareObject[Hth].CanIdType == ID_EXTENDED)
		{
			registers[Hth].u16ArbReg0 = (uint16_t)(PduInfoPtr->id);
			registers[Hth].u16ArbReg1 = (((uint16_t)(PduInfoPtr->id)>>16)&13);

			SET_BIT(registers[Hth].u16ArbReg1,XTD_BIT);
			SET_BIT(registers[Hth].u16ArbReg1,MSGVAL_BIT);
		}
		else
		{
			registers[Hth].u16ArbReg0 = 0;
			registers[Hth].u16ArbReg1 = (((uint16_t)(PduInfoPtr->id)<<2)&13);

			SET_BIT(registers[Hth].u16MaskReg1,MSGVAL_BIT);
		}

		uint16_t Data_Index,Data_Value;
		uint32_t Data_Reg;
		Data_Reg = (CANIF1DA1_OFFSET);

		for(Data_Index=0;Data_Index<PduInfoPtr->length;)
		{
			Data_Value =(uint8_t)PduInfoPtr->sdu[Data_Index++];
			if(Data_Index<PduInfoPtr->length)
			{
				Data_Value |= (uint8_t)PduInfoPtr->sdu[Data_Index++] << 8;
			}
			REG_VAL(CAN0_BASE,Data_Reg) |= (uint16_t)Data_Value;
			Data_Reg += 4;
		}

		SET_BIT(u16MsgCtrl,TXIE_BIT);
		SET_BIT(u16MsgCtrl,UMASK_BIT);

		registers[Hth].u16MsgCntrl |= u16MsgCtrl;

		REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET) = registers[Hth].u16CmdMaskReg ;
		REG_VAL(CAN0_BASE,CANIF1MSK1_OFFSET) = registers[Hth].u16MaskReg0;
		REG_VAL(CAN0_BASE,CANIF1MSK2_OFFSET) = registers[Hth].u16MaskReg1;
		REG_VAL(CAN0_BASE,CANIF1ARB1_OFFSET) = registers[Hth].u16ArbReg0;
		REG_VAL(CAN0_BASE,CANIF1ARB2_OFFSET) = registers[Hth].u16ArbReg1;
		REG_VAL(CAN0_BASE,CANIF1MCTL_OFFSET) = registers[Hth].u16MsgCntrl;

		SET_BIT(REG_VAL(CAN0_BASE,CANIF1ARB2_OFFSET),DIR_BIT);
		REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET) = Object_Check[CAN0_CONTROLLER_ID][Hth][0].Mailbox & MNUM_MASK;

		while(BIT_IS_SET(REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET),BUSY_BIT))
		{

		}

		Object_Check[CAN0_CONTROLLER_ID][Hth][0].Check = Unconfirmed;
		Object_Check[CAN0_CONTROLLER_ID][Hth][0].PduId = PduInfoPtr->swPduHandle;

	}
	else if(BASIC == Can_Configuration.CanConfigSet.CanHardwareObject[Hth].CanHandleType)
	{
		uint8_t iter = 0;
		for(iter=0;iter<Can_Configuration.CanConfigSet.CanHardwareObject[Hth].CanHardwareObjectCount-1;iter++)
		{
			if(Object_Check[CAN0_CONTROLLER_ID][Hth][iter].Check == Confirmed)
			{
				Object_Check[CAN0_CONTROLLER_ID][Hth][iter].Check = Unconfirmed;

				uint16_t u16MsgCtrl;
				u16MsgCtrl = 0;

				SET_BIT(u16MsgCtrl,TXRQST_BIT);
				u16MsgCtrl |= PduInfoPtr->length & DLC_MASK;
				SET_BIT(u16MsgCtrl,EOB_BIT);

				if(Can_Configuration.CanConfigSet.CanHardwareObject[Hth].CanIdType == ID_EXTENDED)
				{
					registers[Hth].u16ArbReg0 = (uint16_t)(PduInfoPtr->id);
					registers[Hth].u16ArbReg1 = (((uint16_t)(PduInfoPtr->id)>>16)&13);

					SET_BIT(registers[Hth].u16ArbReg1,XTD_BIT);
					SET_BIT(registers[Hth].u16ArbReg1,MSGVAL_BIT);
				}
				else
				{
					registers[Hth].u16ArbReg0 = 0;
					registers[Hth].u16ArbReg1 = (((uint16_t)(PduInfoPtr->id)<<2)&13);

					SET_BIT(registers[Hth].u16MaskReg1,MSGVAL_BIT);
				}

				uint16_t Data_Index,Data_Value;
				uint32_t Data_Reg;
				Data_Reg = (CANIF1DA1_OFFSET);

				for(Data_Index=0;Data_Index<PduInfoPtr->length;)
				{
					Data_Value =(uint8_t)PduInfoPtr->sdu[Data_Index++];
					if(Data_Index<PduInfoPtr->length)
					{
						Data_Value |= (uint8_t)PduInfoPtr->sdu[Data_Index++] << 8;
					}
					REG_VAL(CAN0_BASE,Data_Reg) |= (uint16_t)Data_Value;
					Data_Reg += 4;
				}

				SET_BIT(u16MsgCtrl,TXIE_BIT);
				SET_BIT(u16MsgCtrl,UMASK_BIT);

				registers[Hth].u16MsgCntrl |= u16MsgCtrl;

				REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET) = registers[Hth].u16CmdMaskReg ;
				REG_VAL(CAN0_BASE,CANIF1MSK1_OFFSET) = registers[Hth].u16MaskReg0;
				REG_VAL(CAN0_BASE,CANIF1MSK2_OFFSET) = registers[Hth].u16MaskReg1;
				REG_VAL(CAN0_BASE,CANIF1ARB1_OFFSET) = registers[Hth].u16ArbReg0;
				REG_VAL(CAN0_BASE,CANIF1ARB2_OFFSET) = registers[Hth].u16ArbReg1;
				REG_VAL(CAN0_BASE,CANIF1MCTL_OFFSET) = registers[Hth].u16MsgCntrl;

				SET_BIT(REG_VAL(CAN0_BASE,CANIF1ARB2_OFFSET),DIR_BIT);
				REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET) = Object_Check[CAN0_CONTROLLER_ID][Hth][0].Mailbox & MNUM_MASK;

				while(BIT_IS_SET(REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET),BUSY_BIT))
				{

				}

				Object_Check[CAN0_CONTROLLER_ID][Hth][0].PduId = PduInfoPtr->swPduHandle;

				break;

			}
		}
	}

}

uint8_t Can_Message_Recieve(uint32_t Controller_ID,uint8_t MessageObj_Num,Can_PduType* Message)
{
	uint32_t Controller_Base_Address;
	switch(Controller_ID)
	{
		case CAN0_CONTROLLER_ID:
			Controller_Base_Address = CAN0_BASE;
			break;
		case CAN1_CONTROLLER_ID:
			Controller_Base_Address = CAN1_BASE;
			break;
		default:
			break;
	}

	uint16_t u16CmdMaskReg = 0;
	uint16_t u16ArbReg0 = 0;
	uint16_t u16ArbReg1 = 0;
	uint16_t u16MsgCtrl = 0;
	uint8_t Flag = FLAG_LOW;

	SET_BIT(u16CmdMaskReg,ARB_BIT);
	SET_BIT(u16CmdMaskReg,DATAA_BIT);
	SET_BIT(u16CmdMaskReg,DATAB_BIT);
	SET_BIT(u16CmdMaskReg,MASK_BIT);
	SET_BIT(u16CmdMaskReg,CONTROL_BIT);

	REG_VAL(Controller_Base_Address,CANIF1CMSK_OFFSET) = u16CmdMaskReg;
	REG_VAL(Controller_Base_Address,CANIF1CRQ_OFFSET) = (MessageObj_Num & (MNUM_MASK));

	while(BIT_IS_SET(REG_VAL(Controller_Base_Address,CANIF1CRQ_OFFSET),BUSY_BIT))
	{

	}

	u16ArbReg0 = REG_VAL(Controller_Base_Address,CANIF1ARB1_OFFSET);
	u16ArbReg1 = REG_VAL(Controller_Base_Address,CANIF1ARB2_OFFSET);
	u16MsgCtrl = REG_VAL(Controller_Base_Address,CANIF1MCTL_OFFSET);

	if(BIT_IS_CLEAR(REG_VAL(Controller_Base_Address,CANIF1ARB2_OFFSET),XTD_BIT))
	{
		Message->id = ( u16ArbReg1 & ID_MASK ) >> 2;
	}
	else
	{
		Message->id = (u16ArbReg1 & ID_MASK ) << 16 | u16ArbReg0 ;
	}

	if(BIT_IS_SET(REG_VAL(Controller_Base_Address,CANIF1MCTL_OFFSET),NEWDAT_BIT))
	{
		uint32_t Data_Index;
		uint32_t Data_REG;
		uint32_t Data_Value;

		Flag = FLAG_HIGH;

		Message->length = u16MsgCtrl & DLC_MASK ;
		Data_REG = (CANIF1DA1_OFFSET);
		static uint8_t Data_Ptr[8] = {0};
		for(Data_Index=0;Data_Index<Message->length;)
		{
			Data_Value = REG_VAL(Controller_Base_Address,Data_REG);
			Data_Ptr[Data_Index++] = (uint8_t)Data_Value;

			if(Data_Index<Message->length)
			{
				Data_Ptr[Data_Index++] = (uint8_t)(Data_Value >> 8);
			}
			else
			{

			}
			Data_REG+=4;
		}
		Message->sdu = Data_Ptr;

		REG_VAL(Controller_Base_Address,CANIF1CMSK_OFFSET) = NEWDATA_BIT_MASK;
		REG_VAL(Controller_Base_Address,CANIF1CRQ_OFFSET) = MessageObj_Num & (MNUM_MASK);

		while(BIT_IS_SET(REG_VAL(Controller_Base_Address,CANIF1CRQ_OFFSET),BUSY_BIT))
		{

		}

	}
	else
	{

	}

	return Flag;

}

void Can_MainFunction_Write(void)
{

#if(POLLING == CAN0_TX_PROCESSING  || POLLING == CAN1_TX_PROCESSING)

	if(Can_Status == CAN_UNINIT)
	{
#if(STD_ON == CAN_DEV_ERROR_DETECT)
		Det_ReportError(MODULE_ID,CAN_INSTANCE_ID,CAN_MAINFUNCTION_WRITE_SID,CAN_E_UNINIT);
#endif
	}
	else
	{
#if(STD_ON == CAN0_CONTROLLER_ACTIVATION)
#if(POLLING == CAN0_TX_PROCESSING)

		uint8_t HOH_Index=0;
		uint8_t Mailbox_Index=0;
		for(HOH_Index=0;HOH_Index<CAN_HOH_NUM;HOH_Index++)
		{
			if(TRANSMIT == Can_Configuration.CanConfigSet.CanHardwareObject[HOH_Index].CanObjectType)
			{
				if(Can_Configuration.CanConfigSet.CanHardwareObject[HOH_Index].CanControllerRef == &Can_Configuration.CanConfigSet.CanController[CAN0_CONTROLLER_ID])
				{
					for(Mailbox_Index=0;Mailbox_Index<Can_Configuration.CanConfigSet.CanHardwareObject[HOH_Index].CanHardwareObjectCount -1;Mailbox_Index++)
					{
						if(Object_Check[CAN0_CONTROLLER_ID][HOH_Index][Mailbox_Index].Check == Unconfirmed)
						{
							uint16_t u16CmdMaskReg=0;
							SET_BIT(u16CmdMaskReg,CONTROL_BIT);
							CLEAR_BIT(u16CmdMaskReg,WRNRD_BIT);
							REG_VAL(CAN0_BASE,CANIF1CMSK_OFFSET) = u16CmdMaskReg;
							REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET) = Object_Check[CAN0_CONTROLLER_ID][HOH_Index][Mailbox_Index].Mailbox & MNUM_MASK;

							while(BIT_IS_SET(REG_VAL(CAN0_BASE,CANIF1CRQ_OFFSET),BUSY_BIT))
							{

							}

							/*Check if Tx is completed */
							if(BIT_IS_CLEAR(REG_VAL(CAN0_BASE,CANIF1MCTL_OFFSET),TXRQST_BIT))
							{
								Object_Check[CAN0_CONTROLLER_ID][HOH_Index][Mailbox_Index].Check == Confirmed;
								CanIf_TxConfirmation(Object_Check[CAN0_CONTROLLER_ID][HOH_Index][Mailbox_Index].PduId);
							}
							else
							{

							}
						}
						else
						{

						}
					}
				}
				else
				{

				}

			}
			else
			{

			}
		}
#endif
#endif
	}
#endif
}

void Can_MainFunction_Read(void)
{

#if(POLLING == CAN0_RX_PROCESSING  || POLLING == CAN1_RX_PROCESSING)

	if(Can_Status == CAN_UNINIT)
	{
#if(STD_ON == CAN_DEV_ERROR_DETECT)
		Det_ReportError(MODULE_ID,CAN_INSTANCE_ID,CAN_MAINFUNCTION_READ_SID,CAN_E_UNINIT);
#endif
	}
	else
	{
#if(STD_ON == CAN0_CONTROLLER_ACTIVATION)
#if(POLLING == CAN0_RX_PROCESSING)

		Can_HwType MSG_Object;
		Can_PduType Can_Msg_Recieved;
		PduInfoType RecieverPduInfo;

		uint8_t HOH_Index=0;
		uint8_t Mailbox_Index=0;
		for(HOH_Index=0;HOH_Index<CAN_HOH_NUM;HOH_Index++)
		{
			if(RECIEVE == Can_Configuration.CanConfigSet.CanHardwareObject[HOH_Index].CanObjectType)
			{
				if(Can_Configuration.CanConfigSet.CanHardwareObject[HOH_Index].CanControllerRef == &Can_Configuration.CanConfigSet.CanController[CAN0_CONTROLLER_ID])
				{
					for(Mailbox_Index=0;Mailbox_Index<Can_Configuration.CanConfigSet.CanHardwareObject[HOH_Index].CanHardwareObjectCount -1;Mailbox_Index++)
					{
						uint8_t NEW_DATA_UPDATE;
						NEW_DATA_UPDATE=Can_Message_Recieve(CAN0_CONTROLLER_ID, Object_Check[CAN0_CONTROLLER_ID][HOH_Index][Mailbox_Index].Mailbox, &Can_Msg_Recieved);

						if(NEW_DATA_UPDATE == FLAG_HIGH)
						{
							Object_Check[CAN0_CONTROLLER_ID][HOH_Index][Mailbox_Index].Check = Unconfirmed;

							MSG_Object.CanId = Can_Msg_Recieved.id;
							MSG_Object.ControllerId = Can_Configuration.CanConfigSet.CanHardwareObject[HOH_Index].CanControllerRef->CanControllerId;
							MSG_Object.Hoh = HOH_Index;

							RecieverPduInfo.SduLength = Can_Msg_Recieved.length;
							RecieverPduInfo.SduDataPtr = Can_Msg_Recieved.sdu;

							CanIf_RxIndication(&MSG_Object,&RecieverPduInfo);

							Object_Check[CAN0_CONTROLLER_ID][HOH_Index][Mailbox_Index].Check = Confirmed;

						}
						else
						{

						}
					}
				}
				else
				{

				}

			}
			else
			{

			}
		}
#endif
#endif
	}
#endif
}


