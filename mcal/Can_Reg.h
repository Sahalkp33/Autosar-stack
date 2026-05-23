/*
 * Can_Reg.h
 *
 *  Created on: Apr 6, 2026
 *      Author: sahal
 */

#ifndef MCAL_CAN_REG_H_
#define MCAL_CAN_REG_H_

#define SYCTL_RCGC0_REG		(*((volatile uint32_t *)0x400FE100))

/*RCGC0 REG*/
#define CAN0_BIT			(24U)

/*Base Address */
#define CAN0_BASEADDR		0x40040000


/*Offsets for Can Registers */
#define CANCTL_OFFSET		0X000
#define CANSTS_OFFSET		0x004
#define CANBIT_OFFSET		0X00C
#define CANINT_OFFSET		0X010
#define CANBRPE_OFFSET		0x018
#define CANIF1CRQ_OFFSET	0X020
#define CANIF1CMSK_OFFSET	0X024
#define CANIF1MSK1_OFFSET	0X028
#define CANIF1MSK2_OFFSET	0X02C
#define CANIF1ARB1_OFFSET	0X030
#define CANIF1ARB2_OFFSET	0X034
#define CANIF1MCTL_OFFSET	0X038
#define CANIF1DA1_OFFSET	0X03C

/*CANCTL Bits */
#define INIT_BIT			0U
#define CCE_BIT				6U

/*CANSTS Bits*/
#define BOFF_BIT			(7U)
#define TXOK_BIT			(3U)
#define RXOK_BIT			(4U)
#define LEC0_BIT			(0U)
#define LEC1_BIT			(1U)
#define LEC2_BIT			(2U)

/*CANINT Bits*/
#define STATUS_INTERRUPT	(uint32_t)0x8000

/*CANIFNCRQ Bits */
#define MNUM_MASK			0x3f
#define BUSY_BIT			15U

/*CANIFnCMSK bITS */
#define DATAA_BIT			0U
#define DATAB_BIT			1U
#define NEWDATA_BIT_MASK	0X4
#define CLRINTPND_BIT		3U
#define CONTROL_BIT			4U
#define ARB_BIT				5U
#define MASK_BIT			6U
#define WRNRD_BIT			7U

/*CANIFnARB2 Bits */
#define DIR_BIT				13U
#define XTD_BIT				14U
#define MSGVAL_BIT			15U
#define ID_MASK				0x1FFF

/*CANIFnMCTL Bits */
#define EOB_BIT				7U
#define TXRQST_BIT			8U
#define RXIE_BIT			10U
#define TXIE_BIT			11U
#define UMASK_BIT			12U
#define NEWDAT_BIT			15U
#define DLC_MASK			0XF

/*CANIFnMASK2 Bits */
#define MXTD_BIT			15U


#endif /* MCAL_CAN_REG_H_ */
