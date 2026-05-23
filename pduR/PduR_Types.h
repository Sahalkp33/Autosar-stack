/*
 * PduR_Types.h
 *
 *  Created on: Apr 17, 2026
 *      Author: sahal
 */

#ifndef PDUR_PDUR_TYPES_H_
#define PDUR_PDUR_TYPES_H_

#include "PduR_Cfg.h"
#include "CanIf_Types.h"

typedef enum
{
	PDUR_COM,
	PDUR_DCM
}PduRSrcModule;


typedef struct
{
	uint16_t PduRSrcPduHandleId;
	PduRSrcModule PduRSrcModule
}PduRSrcPdu;

typedef struct
{
	CanIfTxPduCfg* PduRDestPduRef;
	uint16_t PduRDestPduHandleId;
}PduRDestPdu;

typedef struct
{
	PduRSrcPdu PduRSrcPdu;
	PduRDestPdu PduRDestPdu;
}PduRRoutingPaths;

typedef struct
{
	PduRRoutingPaths PduRRoutingPaths[PduRMaxRoutingPathCount];
}PduR_ConfigType;

typedef enum
{
	PDUR_UNINIT,
	PDUR_ONLINE
}PduR_StateType;

#endif /* PDUR_PDUR_TYPES_H_ */
