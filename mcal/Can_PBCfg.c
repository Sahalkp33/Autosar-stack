/*
 * Can_PBCfg.c
 *
 *  Created on: Apr 18, 2026
 *      Author: sahal
 */

#include "can.h"

Can_ConfigType Can_Configuration = {
		.CanConfigSet = {
				.CanHardwareObject = {
						{
								.CanObjectType = CAN_HOH_TYPE_0,
								.CanObjectId = CAN_HOH_ID_0,
								.CanControllerRef = &Can_Configuration.CanConfigSet.CanController[CAN_HOH_REF_0]
						},
						{
								.CanObjectType = CAN_HOH_TYPE_1,
								.CanObjectId = CAN_HOH_ID_1,
								.CanControllerRef = &Can_Configuration.CanConfigSet.CanController[CAN_HOH_REF_1]
						}
				},
				.CanController = {
						{
								.CanControllerId = CAN0_CONTROLLER_ID
						}
				}
		}
};
