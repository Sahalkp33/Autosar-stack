/*
 * CanIf_Cfg.h
 *
 *  Created on: Apr 17, 2026
 *      Author: sahal
 */

#ifndef CAN_IF_CANIF_CFG_H_
#define CAN_IF_CANIF_CFG_H_

/************************CanIfPrivateCfg Container*******************************/
/*Selects whether Data Length Check is supported.*/
#define CanIfPrivateDataLengthCheck         (STD_ON)

/************************CanIfPublicCfg Container*******************************/
/*Switches the development error detection and notification on or off.*/
#define CanIfDevErrorDetect                 (STD_ON)

/*
 * Enables / Disables the API CanIf_ReadRxPduData() for reading
 * received L-SDU data.
 */
#define CanIfPublicReadRxPduDataApi         (STD_ON)

/*
 * Enables and disables the API for reading the notification status of
 * receive L-PDUs.
 */
#define CanIfPublicReadRxPduNotifyStatusApi   (STD_ON)

/*
 * Enables and disables the API for reading the notification status of
 * transmit L-PDUs.
 */
#define CanIfPublicReadTxPduNotifyStatusApi   (STD_ON)


#define CanIfMaxTxPduCfg					(3U)

#define CanIfTxPduId_0						(0U)

#define CanIfTxPduCanId_0					(0x01)
#define CanIfTxPduCanId_1					(0x00)


#define HTH_NUMBER							(3U)
#define HTH0_ID								(0U)
#define HTH1_ID								(1U)
#define HTH2_ID								(2U)

#define CANIF_BUFFER_NUMBER					(HTH_NUMBER)



#define CAN_DRIVERS_NUM						(1U)

#define CanIfTxPduCanIdType_0				(CanIfTxPduCanIdType)STANDARD_CAN
#define CanIfTxPduCanIdType_1				(CanIfTxPduCanIdType)STANDARD_CAN

#define CanIfMaxRxPduCfg					(2U)

#define CanIfRxPduCanId_0       			(0b00000000001)
#define CanIfRxPduCanId_1       			(0b00000000011)

/*Data length of the received CAN L-PDUs used by the CAN Interface.*/
#define CanIfRxPduDataLength_0               (8U)
#define CanIfRxPduDataLength_1               (8U)


/*
 * Identifier mask which denotes relevant bits in the CAN Identifier. This
 * parameter defines a CAN Identifier range in an alternative way to
 * CanIfRxPduCanIdRange. It identifies the bits of the configured CAN
 * Identifier that must match the received CAN Identifier. Range: 11 bits
 * for Standard CAN Identifier, 29 bits for Extended CAN Identifier.
 */
#define CanIfRxPduCanIdMask_0   (0b11111111111)
#define CanIfRxPduCanIdMask_1   (0b11111111111)

#define HRH_NUMBER						(2U)
#define HRH0_ID							(0U)
#define HRH1_ID							(1U)

/*This parameter defines the name of the <User_RxIndication>.*/
#define CanIfRxPduUserRxIndicationName_0          (CanIfUser)(PDUR)
#define CanIfRxPduUserRxIndicationName_1          (CanIfUser)(PDUR)



/************************CanIfHrhCfg Container*******************************/

/*
 * Lower CAN Identifier of a receive CAN L-PDU for identifier range
 * definition, in which all CAN Ids shall pass the software filtering.
 */
#define CanIfHrhRangeRxPduLowerCanId_0              (0b00000000000)
#define CanIfHrhRangeRxPduLowerCanId_1              (0b00000000000)

/*
 * Upper CAN Identifier of a receive CAN L-PDU for identifier range
 * definition, in which all CAN Ids shall pass the software filtering.
 */
#define CanIfHrhRangeRxPduUpperCanId_0              (0b11111111111)
#define CanIfHrhRangeRxPduUpperCanId_1              (0b11111111111)

#endif /* CAN_IF_CANIF_CFG_H_ */
