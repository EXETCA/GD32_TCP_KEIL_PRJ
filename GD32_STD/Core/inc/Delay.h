#ifndef __GD32F40X_DELAY_H__

#define __GD32F40X_DELAY_H__
#include "gd32F4xx.h"
extern uint32_t SystemCoreClock;
#define  F_CPU (uint32_t)(SystemCoreClock)

#ifdef __cplusplus

extern "C" {

#endif

  

void __delay_loops(unsigned long n);

void __delay_loops_0(void);



#ifdef __cplusplus

}

#endif



#define _delay_loops(n)    do{if(n>2){__delay_loops((n)-2);}else{__delay_loops_0();}}while(0)



#define  _CYCLES_PER_LOOP  3



#define _delay_us(A) _delay_loops( (uint32_t) (( (double)(F_CPU) *((A)/1000000.0))/_CYCLES_PER_LOOP+0.5))



#define _delay_ms(A) _delay_loops( (uint32_t) (( (double)(F_CPU) *((A)/1000.0))/_CYCLES_PER_LOOP+0.5))



#define _delay_s(A) _delay_loops( (uint32_t) (( (double)(F_CPU) *((A)/1.0))/_CYCLES_PER_LOOP+0.5))

#define Delay_Us(n) _delay_us(n)
#define Delay_Ms(n) _delay_ms(n)
#define DELAY_Ms Delay_Ms
#endif