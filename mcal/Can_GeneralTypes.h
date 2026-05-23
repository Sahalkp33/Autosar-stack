/*
 * Can_GeneralTypes.h
 *
 *  Created on: Apr 20, 2026
 *      Author: sahal
 */

#ifndef MCAL_CAN_GENERALTYPES_H_
#define MCAL_CAN_GENERALTYPES_H_

#include <stdint.h>

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Enumeration to represent States that are used by the several ControllerMode functions*/
typedef enum
{
	CAN_CS_UNINIT,
	CAN_CS_STARTED,
	CAN_CS_STOPPED,
	CAN_CS_SLEEP
}Can_ControllerStateType;

typedef struct
{
	uint32_t CanId;
	uint8_t Hoh;
	uint8_t ControllerId;
}Can_HwType;

typedef struct
{
	uint32_t swPduHandle;
	uint8_t length;
	uint32_t id;
	uint8_t* sdu;
}Can_PduType;


#endif /* MCAL_CAN_GENERALTYPES_H_ */
