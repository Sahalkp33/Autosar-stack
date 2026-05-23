/*
 * can.h
 *
 *  Created on: Apr 6, 2026
 *      Author: sahal
 */

#ifndef MCAL_CAN_H_
#define MCAL_CAN_H_

#include "Can_Reg.h"
#include "Can_Cfg.h"
#include "common_macros.h"
#include "Can_GeneralTypes.h"
#include "Platform_Types.h"
#include "ComStack_Types.h"

#include <stdint.h>

#define CAN_INSTANCE_ID			0

typedef enum Can_ReturnType
{
	CAN_OK,
	//CAN_NOT_OK,
	//CAN_BUSY
}Can_ReturnType;

typedef enum
{
	BASIC,
	FULL
}CanHandleType;

typedef enum
{
	ID_STANDARD,
	ID_EXTENDED,
	//ID_MIXED
}CanIdType;

typedef enum
{
	TRANSMIT,
	RECIEVE
}CanObjectType;

typedef struct CanBaudRate
{
	uint16_t BaudRate;
	uint8_t PropSeg;
	uint8_t PhaseSeg1;
	uint8_t PhaseSeg2;
	uint8_t SyncJumpWidth;
}CanControllerBaudRateConfig;

typedef struct CanController
{
	uint8_t CanControllerId;
	CanControllerBaudRateConfig CanControllerBaudRateConfig;
}CanController;

typedef struct Can_HwFilterConfig
{
	uint32_t CanHwFilterCode;
	uint32_t CanHwFilterMask;
}CanHwFilter;

typedef struct CanHwObject
{
	CanIdType CanIdType;
	CanObjectType CanObjectType;
	CanHandleType CanHandleType;
	uint32_t CanHardwareObjectCount;
	uint32_t CanObjectId;
	CanHwFilter CanHwFilter;
	CanController* CanControllerRef;
}CanHardwareObject;

typedef struct CanConfigSet
{
	CanController CanController[CAN_CONTROLLERS_NUM];
	CanHardwareObject CanHardwareObject[CAN_HOH_NUM];
}CanConfigSet;

typedef struct
{
	CanConfigSet CanConfigSet;
}Can_ConfigType;

extern Can_ConfigType Can_Configuration;

typedef enum
{
	Confirmed,
	Unconfirmed
}Confirmation_Check;



typedef struct
{
	Confirmation_Check Check;
	uint8_t Mailbox;
	PduIdType PduId;
}Message_Confirmation;

typedef struct
{
	uint16_t u16CmdMaskReg;
	uint16_t u16MaskReg0,u16MaskReg1;
	uint16_t u16ArbReg0,u16ArbReg1;
	uint16_t u16MsgCntrl;
	uint8_t BusExtendedId;
}register_val;

void Can_Write(uint8_t Hth,Can_PduType* PduInfoPtr);


#define FLAG_LOW		(0U)
#define FLAG_HIGH		(1U)


extern Message_Confirmation Object_Check[CAN_CONTROLLERS_NUM][CAN_HOH_NUM][MAX_HWOBJECT_COUNT];




#endif /* MCAL_CAN_H_ */
