/*
 * PduR_CanIf.h
 *
 *  Created on: May 10, 2026
 *      Author: sahal
 */

#ifndef PDUR_PDUR_CANIF_H_
#define PDUR_PDUR_CANIF_H_

#include "PduR_Cfg.h"
#include "PduR_Types.h"

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
#define PDUR_CANIFRXINDICATION_SID					(uint8_t)0x42
#define PDUR_CANIFTXCONFIRMATION_SID				(uint8_t)0x48


/************************PduRBswModules Container************************/
/*
 * Specifies if the BSW module supports the TxConfirmation API or not.
 * Value true the API is supported.
 */
#define PduRTxConfirmation                              (STD_ON)


#endif /* PDUR_PDUR_CANIF_H_ */
