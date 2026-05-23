/*
 * ComStack_Types.h
 *
 *  Created on: Apr 13, 2026
 *      Author: sahal
 */

#ifndef COM_COMSTACK_TYPES_H_
#define COM_COMSTACK_TYPES_H_

#include <stdint.h>
#include "Com_Cfg.h"


/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/*Type definition of the ID of the PDU*/
typedef uint32_t PduIdType;

/*Type definition of the Length of the PDU*/
typedef uint32_t 		PduLengthType;

/* The AUTOSAR COM module's signal object identifier. */
typedef uint16_t		Com_SignalIdType;

/*Structure that carries the info of the PDU*/
typedef struct
{
	uint8_t* SduDataPtr;
	uint8_t* MetaDataPtr;
	PduLengthType SduLength;
}PduInfoType;

/* This is a status value returned by the API service Com_GetStatus(). */
typedef enum
{
	COM_UNINIT,
	COM_INIT
}Com_StatusType;



typedef enum
{
	COM_SEND,
	COM_RECIEVE
}ComIpduDirection;

typedef enum
{
	DEFERRED,
	IMMEDIATE
}ComIpduSignalProcessing;

typedef enum
{
	PERIODIC_Tx,
	DIRECT_Tx
}TxModeMode;

typedef enum
{
	TRIGGERED,
	TRIGGERED_WITHOUT_REPITITION,
	PENDING
}ComTransferProperty;

typedef enum
{
	LITTLE_ENDIAN,
	BIG_ENDIAN,
	OPAQUE
}ComSignalEndianness;

typedef struct
{
	uint16_t ComHandleId;
	void(*ComNotification)(void);
	uint32_t ComSignalLength;
	uint8_t ComSignalInitValue;
	ComTransferProperty ComTransferProperty;
	uint8_t ComBitPosition;
	ComSignalEndianness ComSignalEndianness;
}ComSignal;

typedef struct
{
	TxModeMode TxModeMode;
	float TxModeTimePeriod;
}TxMode;

typedef struct
{
	TxMode TxMode;
	uint8_t ComTxIpduUnusedAreasDefault;
}ComTxIpdu;

typedef struct
{
	uint16_t ComIpduHandleId;
	ComIpduSignalProcessing ComIpduSignalProcessing;
	ComTxIpdu ComTxIpdu;
	ComIpduDirection ComIpduDirection;
	ComSignal* ComIpduSignalRef[ComMaxIpduCount];
}ComIpdu;

typedef struct
{
	uint64_t MemSize;
	ComSignal ComSignal[ComMaxIpduCount];
	ComIpdu ComIpdu[MAX_NUM_OF_SIGNALS];
}Com_ConfigType;





#endif /* COM_COMSTACK_TYPES_H_ */
