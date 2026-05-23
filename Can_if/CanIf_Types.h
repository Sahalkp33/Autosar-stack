/*
 * CanIf_Types.h
 *
 *  Created on: Apr 17, 2026
 *      Author: sahal
 */

#ifndef CAN_IF_CANIF_TYPES_H_
#define CAN_IF_CANIF_TYPES_H_

#include "stdint.h"

#include "can.h"
#include "Std_Types.h"
#include "CanIf_Cfg.h"

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* The State of the CanIf module  */
typedef enum
{
    CANIF_UNINIT,
    CANIF_READY
} CanIf_State;

typedef enum
{
	CANIF_OFFLINE,
	CANIF_TX_OFFLINE,
	CANIF_TX_OFFLINE_ACTIVE,
	CANIF_ONLINE
}CanIf_PduModeType;

typedef enum
{
	EXTENDED_CAN,
	EXTENDED_FD_CAN,
	STANDARD_CAN,
	STANDARD_FD_CAN
}CanIfTxPduCanIdType;

typedef enum
{
	CAN_NM,
	CAN_TP,
	CAN_TSYN,
	CDD,
	J1939NM,
	J1939TP,
	PDUR,
	XCP
}CanIfUser;

typedef enum
{
	CANIF_NO_NOTIFICATION,
	CANIF_TX_RX_NOTIFICATION
}CanIf_NotifStatusType;

typedef struct
{
	uint8_t CanIfCtrlId;
	CanController* CanIfCtrlCanCtrlRef;
}CanIfCtrlCfg;

typedef struct
{
	CanIfCtrlCfg* CanIfHthCanCtrlIdRef;
	CanHardwareObject* CanIfHthIdSymRef;
}CanIfHthCfg;

typedef struct
{
	uint32_t CanIfHrhRangeRxPduLowerCanId;
	uint32_t CanIfHrhRangeRxPduUpperCanId;

}CanIfHrhRangeCfg;

typedef struct
{
	CanIfCtrlCfg* CanIfHrhCanCtrlIdRef;
	CanHardwareObject* CanIfHrhIdSymRef;
	CanIfHrhRangeCfg CanIfHrhRangeCfg;
	boolean CanIfHrhSoftwareFilter;
}CanIfHrhCfg;

typedef struct
{
	CanIfHthCfg* CanIfBufferHthRef;
}CanIfBufferCfg;

typedef struct
{
	uint32_t CanIfRxPduCanIdMask;
	uint32_t CanIfRxPduCanId;
	CanIfHrhCfg* CanIfRxPduHrhIdRef;
#if(CanIfPublicReadRxPduNotifyStatusApi == STD_ON)
	boolean CanIfRxPduReadNotifyStatus;
#endif
	uint8_t CanIfRxPduDataLength;
#if(CanIfPublicReadRxPduDataApi == STD_ON)
	boolean CanIfRxPduReadData;
#endif
	CanIfUser CanIfRxPduUserRxIndicationName;
}CanIfRxPduCfg;

typedef struct
{
	CanIfBufferCfg* CanIfTxPduBufferRef;
	uint32_t CanIfTxPduId;
	CanIfTxPduCanIdType CanIfTxPduCanIdType;
	uint32_t CanIfTxPduCanId;
#if(CanIfPublicReadTxPduNotifyStatusApi == STD_ON)
	boolean CanIfTxPduReadNotifyStatus;
#endif
	uint8_t CanIfTxPduTruncation;
	CanIfUser CanIfTxPduUserTxConfirmationUL;
}CanIfTxPduCfg;

typedef struct
{
	CanIfHthCfg CanIfHthCfg[HTH_NUMBER];
	CanIfHrhCfg CanIfHrhCfg[HRH_NUMBER];
}CanIfInitHohCfg;

typedef struct
{
	CanIfInitHohCfg CanIfInitHohCfg[CAN_DRIVERS_NUM];
	CanIfBufferCfg CanIfBufferCfg[CANIF_BUFFER_NUMBER];
	CanIfTxPduCfg CanIfTxPduCfg[CanIfMaxTxPduCfg];
	CanIfRxPduCfg CanIfRxPduCfg[CanIfMaxRxPduCfg];
}CanIfInitConfig;

typedef struct
{
	CanIfCtrlCfg CanIfCtrlCfg[CAN_CONTROLLERS_NUM];
}CanIfCtrlDrvCfg;

typedef struct
{
	CanIfCtrlDrvCfg CanIfCtrlDrvCfg;
	CanIfInitConfig CanIfInitConfig;
}CanIf_ConfigType;

#endif /* CAN_IF_CANIF_TYPES_H_ */
