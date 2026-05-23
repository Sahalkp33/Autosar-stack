/*
 * CanIf_Cfg.c
 *
 *  Created on: Apr 17, 2026
 *      Author: sahal
 */

#include "CanIf.h"

//#ifndef
//#define CAN_INSTANCE_ID			0
//#endif

CanIf_ConfigType CanIf_Configuration =
{
		.CanIfCtrlDrvCfg = {
					.CanIfCtrlCfg = {
							{
								.CanIfCtrlCanCtrlRef = &Can_Configuration.CanConfigSet.CanController[CAN0_CONTROLLER_ID]
							}
					}
		},
		.CanIfInitConfig = {
				.CanIfTxPduCfg = {
						{
								.CanIfTxPduId = CanIfTxPduId_0,
								.CanIfTxPduBufferRef = (CanIfBufferCfg*)&CanIf_Configuration.CanIfInitConfig.CanIfBufferCfg[HTH0_ID],
								.CanIfTxPduCanIdType = CanIfTxPduCanIdType_0,
								.CanIfTxPduCanId = CanIfTxPduCanId_0,
								.CanIfTxPduTruncation = FALSE
						}
				},
				.CanIfBufferCfg = {
						{
								.CanIfBufferHthRef = (CanIfHthCfg*)&CanIf_Configuration.CanIfInitConfig.CanIfInitHohCfg[CAN_INSTANCE_ID].CanIfHthCfg[HTH0_ID]
						}
				},
				.CanIfInitHohCfg = {
						{
								.CanIfHthCfg = {
										{
												.CanIfHthCanCtrlIdRef = (CanIfCtrlCfg*)&CanIf_Configuration.CanIfCtrlDrvCfg.CanIfCtrlCfg[CAN0_CONTROLLER_ID],
												.CanIfHthIdSymRef = (CanHardwareObject*)&Can_Configuration.CanConfigSet.CanHardwareObject[CAN_HOH_ID_0]
										}
								},
								.CanIfHrhCfg = {
										{
												.CanIfHrhSoftwareFilter = TRUE,
												.CanIfHrhRangeCfg = {
														.CanIfHrhRangeRxPduLowerCanId = CanIfHrhRangeRxPduLowerCanId_0,
														.CanIfHrhRangeRxPduUpperCanId = CanIfHrhRangeRxPduUpperCanId_0
												}

										}

								}
						}
				},
				.CanIfRxPduCfg = {
						{
								.CanIfRxPduCanIdMask = CanIfRxPduCanIdMask_0,
								.CanIfRxPduCanId = CanIfRxPduCanId_0,
								.CanIfRxPduHrhIdRef = &CanIf_Configuration.CanIfInitConfig.CanIfInitHohCfg[CAN_INSTANCE_ID].CanIfHrhCfg[HRH0_ID],
#if(CanIfPublicReadRxPduNotifyStatusApi == STD_ON)
								.CanIfRxPduReadNotifyStatus = TRUE,
#endif
								.CanIfRxPduDataLength = CanIfRxPduCanIdMask_0,
#if(CanIfPublicReadRxPduDataApi == STD_ON)
								.CanIfRxPduReadData = TRUE,
#endif
								.CanIfRxPduUserRxIndicationName = CanIfRxPduUserRxIndicationName_0
						}
				}
		}
};
