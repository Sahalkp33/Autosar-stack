/*
 * Can_Cfg.h
 *
 *  Created on: Apr 6, 2026
 *      Author: sahal
 */

#include "Can_Reg.h"
#include "can.h"

#ifndef MCAL_CAN_CFG_H_
#define MCAL_CAN_CFG_H_

#define CAN_CONTROLLERS_NUM		(uint8_t)2

#define CAN_HOH_NUM				(uint8_t)5

#define MAX_HWOBJECT_COUNT		(uint8_t)3

#define CAN0_BASE				CAN0_BASEADDR

#define CAN0_CONTROLLER_ID		0U
#define CAN1_CONTROLLER_ID		1U

#define CAN_HOH_TYPE_0			(CanObjectType)RECIEVE
#define CAN_HOH_TYPE_1			(CanObjectType)TRANSMIT
#define CAN_HOH_TYPE_2			(CanObjectType)RECIEVE
#define CAN_HOH_TYPE_3			(CanObjectType)TRANSMIT
#define CAN_HOH_TYPE_4			(CanObjectType)TRANSMIT

#define CAN_HOH_ID_0			(uint32_t)0
#define CAN_HOH_ID_1			(uint32_t)1
#define CAN_HOH_ID_2			(uint32_t)2
#define CAN_HOH_ID_3			(uint32_t)3
#define CAN_HOH_ID_4			(uint32_t)4

/*
 * Reference to CAN Controller to which the HOH is associated to.
 */
#define CAN_HOH_REF_0                           CAN0_CONTROLLER_ID
#define CAN_HOH_REF_1                           CAN0_CONTROLLER_ID
#define CAN_HOH_REF_2                           CAN0_CONTROLLER_ID
#define CAN_HOH_REF_3                           CAN0_CONTROLLER_ID
#define CAN_HOH_REF_4                           CAN0_CONTROLLER_ID


#endif /* MCAL_CAN_CFG_H_ */
