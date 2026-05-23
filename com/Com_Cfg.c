/*
 * Com_Cfg.c
 *
 *  Created on: Apr 14, 2026
 *      Author: sahal
 */

#include "Com_Cfg.h"
#include "Com_Cbk.h"
#include "ComStack_Types.h"

Com_ConfigType Com = {
		.ComIpdu[0] = {
				.ComIpduHandleId = 0,
				.ComIpduSignalProcessing = IMMEDIATE,
				.ComTxIpdu = {
						.ComTxIpduUnusedAreasDefault = ComTxIpduUnusedAreasDefault_Value,
						.TxMode = {
								.TxModeMode = PERIODIC_Tx ,
								.TxModeTimePeriod = ComTxModeTimePeriod_Value_0
						}
				},
				.ComIpduDirection = COM_SEND,
				.ComIpduSignalRef[0] = (ComSignal*)&Com.ComSignal[0]
		},
		.ComSignal[0] = {
				.ComHandleId = 0,
				.ComSignalLength = 1,
				.ComSignalInitValue = 0,
				.ComNotification = Com_CbkRxAck_0,
				.ComTransferProperty = TRIGGERED,
				.ComBitPosition = 0,
				.ComSignalEndianness = LITTLE_ENDIAN
		}
};
