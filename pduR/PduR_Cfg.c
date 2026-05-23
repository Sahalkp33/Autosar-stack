/*
 * PduR_Cfg.c
 *
 *  Created on: Apr 17, 2026
 *      Author: sahal
 */

#include "PduR_CanIf.h"
#include "PduR.h"


PduR_ConfigType PduR_Configuration =
{
		.PduRRoutingPaths[0] = {
						.PduRSrcPdu = {
								.PduRSrcPduHandleId = PduRSrcPduHandleId_0,
								.PduRSrcModule = PDUR_COM
						},
						.PduRDestPdu = {
								.PduRDestPduHandleId = PduRDestPduHandleId_0,
						}
		}
};
