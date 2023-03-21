#ifndef __Interrupt_H__
#define __Interrupt_H__

#include "gd32f4xx.h"
extern void Tim4_IRQHandler(void);
extern void TIM4_UPCount_Init(uint16_t arr, uint16_t psc);
#endif
