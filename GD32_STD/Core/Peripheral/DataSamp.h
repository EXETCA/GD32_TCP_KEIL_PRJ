#ifndef INC_DATASAMP_H_
#define INC_DATASAMP_H_

#include "gd32f4xx.h"
#include "Relay_Ctrl.h"
#define RD0_STATE_GPIO_Port GPIOA
#define RD0_STATE_Pin GPIO_PIN_4 //
#define RD1_STATE_GPIO_Port GPIOC
#define RD1_STATE_Pin GPIO_PIN_13
#define RD2_STATE_GPIO_Port GPIOE
#define RD2_STATE_Pin GPIO_PIN_5
#define RD3_STATE_GPIO_Port GPIOE
#define RD3_STATE_Pin GPIO_PIN_1
#define RD4_STATE_GPIO_Port GPIOA
#define RD4_STATE_Pin GPIO_PIN_6
#define RD5_STATE_GPIO_Port GPIOF
#define RD5_STATE_Pin GPIO_PIN_8
#define RD6_STATE_GPIO_Port GPIOF
#define RD6_STATE_Pin GPIO_PIN_6
#define RD7_STATE_GPIO_Port GPIOD
#define RD7_STATE_Pin GPIO_PIN_7
#define RD8_STATE_GPIO_Port GPIOC
#define RD8_STATE_Pin GPIO_PIN_12
#define RD9_STATE_GPIO_Port GPIOB
#define RD9_STATE_Pin GPIO_PIN_7

#define RD0_STATE gpio_output_bit_get(RD0_STATE_GPIO_Port, RD0_STATE_Pin)

#define RD1_STATE gpio_output_bit_get(RD1_STATE_GPIO_Port, RD1_STATE_Pin)
#define RD2_STATE gpio_output_bit_get(RD2_STATE_GPIO_Port, RD2_STATE_Pin)
#define RD3_STATE gpio_output_bit_get(RD3_STATE_GPIO_Port, RD3_STATE_Pin)
#define RD4_STATE gpio_output_bit_get(RD4_STATE_GPIO_Port, RD4_STATE_Pin)
#define RD5_STATE gpio_output_bit_get(RD5_STATE_GPIO_Port, RD5_STATE_Pin)
#define RD6_STATE gpio_output_bit_get(RD6_STATE_GPIO_Port, RD6_STATE_Pin)
#define RD7_STATE gpio_output_bit_get(RD7_STATE_GPIO_Port, RD7_STATE_Pin)
#define RD8_STATE gpio_output_bit_get(RD8_STATE_GPIO_Port, RD8_STATE_Pin)
#define RD9_STATE gpio_output_bit_get(RD9_STATE_GPIO_Port, RD9_STATE_Pin)

#define LED_ON 1
#define LED_OFF 0

/*保护驱动板标志位*/
#define WRITE_FLAG (0x000000) // 1:read,0:write
#define READ_FLAG (0x8000)	  // 1:read,0:write

extern void Init_RdStateGpio(void);
extern uint16_t Read_State(void);
extern void BackUpData(void);
extern void AgingPack(void);
void DataCollect(void);
void Read_AllTemp(void);
void RD_READ(void);
void Alarm_READ(void);
#endif /* INC_DATASAMP_H_ */
