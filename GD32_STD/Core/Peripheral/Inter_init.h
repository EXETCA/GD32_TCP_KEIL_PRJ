#ifndef __INTER_INIT_H__
#define __INTER_INIT_H__

#include "main.h"
#include "gd32F4xx.h"

#define LED1_Port GPIOA
#define LED1_Pin GPIO_PIN_0
#define LED2_Port GPIOA
#define LED2_Pin GPIO_PIN_3

#define LED1_ON() gpio_bit_write(LED1_Port, LED1_Pin, RESET)
#define LED1_OFF() gpio_bit_write(LED1_Port, LED1_Pin, SET)
#define LED1_TOGGLE() gpio_bit_toggle(LED1_Port, LED1_Pin)

#define LED2_ON() gpio_bit_write(LED2_Port, LED2_Pin, RESET)
#define LED2_OFF() gpio_bit_write(LED2_Port, LED2_Pin, SET)
#define LED2_TOGGLE() gpio_bit_toggle(LED2_Port, LED2_Pin)

extern void App_LEDGpioCfg(void);
extern void InitInnerPeripheral(void);
extern void InitLedSysParameter(void);
extern void InitLedSysContral(void);
#endif
