/*
 * Std_Types.h
 *
 *  Created on: May 10, 2026
 *      Author: sahal
 */

/*
 *  Describes the standard Return Type Definitions used in the project
 */

#include "stdint.h"
typedef uint8_t  Std_ReturnType;

#define STD_ON          0x01U       /* Standard ON */
#define STD_OFF         0x00U       /* Standard OFF */

#define E_OK            ((Std_ReturnType)0x00U)      /* Function Return OK */
#define E_NOT_OK        ((Std_ReturnType)0x01U)      /* Function Return NOT OK */
