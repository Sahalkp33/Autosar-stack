/*
 * Com_Cfg.h
 *
 *  Created on: Apr 13, 2026
 *      Author: sahal
 */

#ifndef COM_COM_CFG_H_
#define COM_COM_CFG_H_


#include "Std_Types.h"

/************************ComGeneral Container************************/

/*The error hook shall contain code to call the Det.
 *If this parameter is configured COM_DEV_ERROR_DETECT shall be set
 *to ON as output of the configuration tool. (as input for the source code).
 */

#define ComConfigurationUseDet					(STD_ON)

#if(ComConfigurationUseDet == STD_ON)
#define COM_DEV_ERROR_DETECT					(STD_ON)
#endif


#define ComTxTimeBase							(float).01

#define ComMaxIpduCount							(uint64_t)3
#define SIGNAL_LEN_IN_BYTES						(uint8_t)1
#define PDU_LEN_IN_BYTES						(uint8_t)8
#define MAX_NUM_OF_SIGNALS						(ComMaxIpduCount*(8/SIGNAL_LEN_IN_BYTES))
#define ComTxIpduUnusedAreasDefault_Value		(uint8_t)0xFF
#define ComTxModeTimePeriod_Value_0				(float).02
#define ComTxModeTimePeriod_Value_1				(float).04
#define ComTxModeTimePeriod_Value_2				(float).06

/*Calculated from lcm of Time period value of pdu0,1,2 */
#define ComTxModeTimePeriod_Max_Value			(float).12

#define ComTimeout_Value						(float).125

#endif /* COM_COM_CFG_H_ */
