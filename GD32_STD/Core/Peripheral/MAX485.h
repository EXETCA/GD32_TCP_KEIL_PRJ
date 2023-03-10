#ifndef MAX485_H
#define MAX485_H
#include "main.h"
#define DMA_Send  //or Manual_Send
//#define Manual_Send
#define MAX485EN_Port GPIOA
#define MAX485EN_Pin GPIO_PIN_10

#define MAX485EN_H() gpio_bit_write(MAX485EN_Port, MAX485EN_Pin, SET)
#define MAX485EN_L() gpio_bit_write(MAX485EN_Port, MAX485EN_Pin, RESET)
#define DATA_ADDRESS(USARTx)    ((uint32_t)USARTx +0x4)
extern void MAX485_Init(void);
extern uint8_t ReadTemp(void);
extern void MAX485_SendData(uint8_t *p, uint32_t len);

#endif
