/*
 * common_macros.h
 *
 *  Created on: Apr 6, 2026
 *      Author: sahal
 */

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

#define SET_BIT(REG,BIT)				(REG |= (1 << BIT))
#define CLEAR_BIT(REG,BIT)				(REG &= ~(1 << BIT))

#define REG_VAL(BASE,OFFSET)			(*(volatile uint32_t*)((volatile uint8_t*)BASE + OFFSET))

#define BIT_IS_SET(REG,BIT)				(REG & (1 << BIT))
#define BIT_IS_CLEAR(REG,BIT)			(!(REG & (1 << BIT)))



#endif /* COMMON_MACROS_H_ */
