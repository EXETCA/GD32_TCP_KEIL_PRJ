#include "gd32f4xx.h"
#ifndef INC_RELAY_CTRL_H_
#define INC_RELAY_CTRL_H_

#define RD0_STATE_GPIO_Port GPIOC
#define RD0_STATE_Pin GPIO_PIN_11
#define RD1_STATE_GPIO_Port GPIOC
#define RD1_STATE_Pin GPIO_PIN_12
#define RD2_STATE_GPIO_Port GPIOD
#define RD2_STATE_Pin GPIO_PIN_2
#define RD3_STATE_GPIO_Port GPIOD
#define RD3_STATE_Pin GPIO_PIN_3
#define RD4_STATE_GPIO_Port GPIOD
#define RD4_STATE_Pin GPIO_PIN_7
#define RD5_STATE_GPIO_Port GPIOD
#define RD5_STATE_Pin GPIO_PIN_6
#define RD6_STATE_GPIO_Port GPIOB
#define RD6_STATE_Pin GPIO_PIN_3
#define RD7_STATE_GPIO_Port GPIOB
#define RD7_STATE_Pin GPIO_PIN_4

#define RD0_STATE gpio_input_bit_get(RD0_STATE_GPIO_Port, RD0_STATE_Pin)
#define RD1_STATE gpio_input_bit_get(RD1_STATE_GPIO_Port, RD1_STATE_Pin)
#define RD2_STATE gpio_input_bit_get(RD2_STATE_GPIO_Port, RD2_STATE_Pin)
#define RD3_STATE gpio_input_bit_get(RD3_STATE_GPIO_Port, RD3_STATE_Pin)
#define RD4_STATE gpio_input_bit_get(RD4_STATE_GPIO_Port, RD4_STATE_Pin)
#define RD5_STATE gpio_input_bit_get(RD5_STATE_GPIO_Port, RD5_STATE_Pin)
#define RD6_STATE gpio_input_bit_get(RD6_STATE_GPIO_Port, RD6_STATE_Pin)
#define RD7_STATE gpio_input_bit_get(RD7_STATE_GPIO_Port, RD7_STATE_Pin)

/*PE15->PF10  PE14->PC0 PE13->PC2 PE12->PC3 PE11->PF11 PB2->PE3 */
#define Y1_ON gpio_bit_write(GPIOC, GPIO_PIN_0, SET)
#define Y1_OFF gpio_bit_write(GPIOC, GPIO_PIN_0, RESET)
#define Y2_ON gpio_bit_write(GPIOF, GPIO_PIN_10, SET)
#define Y2_OFF gpio_bit_write(GPIOF, GPIO_PIN_10, RESET)
#define Y3_ON gpio_bit_write(GPIOC, GPIO_PIN_3, SET)
#define Y3_OFF gpio_bit_write(GPIOC, GPIO_PIN_3, RESET)
#define Y4_ON gpio_bit_write(GPIOC, GPIO_PIN_2, SET)
#define Y4_OFF gpio_bit_write(GPIOC, GPIO_PIN_2, RESET)
#define Y4OC_ON gpio_bit_write(GPIOB, GPIO_PIN_6, SET)
#define Y4OC_OFF gpio_bit_write(GPIOB, GPIO_PIN_6, RESET)

#define HC138Y1_NUM 0
#define HC138Y2_NUM 1
#define HC138Y3_NUM 2
#define HC138Y5_NUM 3

//报警灯控制
#define ALRED 0
#define ALYELOW 1
#define ALGREEN 2
#define ALBBEEP 3
//#define FAN_CTRL9   6
//#define FAN_CTRL10  4

//风机控制
#define FAN_CTRL1 0
#define FAN_CTRL2 1
#define FAN_CTRL3 2
#define FAN_CTRL4 3
#define FAN_CTRL5 4
#define FAN_CTRL6 5
#define FAN_CTRL7 6
#define FAN_CTRL8 7

// dianyuan控制
#define POWER_ST 2
#define POWER_STOP 3
#define POWER_SMOCK 4

//...yuliu

#define FLAG_EN 1
#define FLAG_DISEN 0

#define LED_ON 1
#define LED_OFF 0

//预留控制电源
#define I1_ON            ((void)0)
#define I1_OFF           ((void)0)
#define I2_ON            ((void)0)
#define I2_OFF           ((void)0)
#define I3_ON            ((void)0)
#define I3_OFF           ((void)0)
#define I4_ON            ((void)0)
#define I4_OFF           ((void)0)
#define I5_ON            ((void)0)
#define I5_OFF           ((void)0)
#define I6_ON            ((void)0)
#define I6_OFF           ((void)0)
#define I7_ON            ((void)0)
#define I7_OFF           ((void)0)
#define I8_ON            ((void)0)
#define I8_OFF           ((void)0)
/*保护驱动板标志位*/
#define WRITE_FLAG                  (0x000000)  //1:read,0:write
#define READ_FLAG                   (0x8000)    //1:read,0:write
struct ReadState_BITS
{
	uint16_t PIN1 : 1;
	uint16_t PIN2 : 1;
	uint16_t PIN3 : 1;
	uint16_t PIN4 : 1;
	uint16_t PIN5 : 1;
	uint16_t PIN6 : 1;
	uint16_t PIN7 : 1;
	uint16_t PIN8 : 1;

	uint16_t PIN9 : 1;
	uint16_t PIN10 : 1;
	uint16_t PIN11 : 1;
	uint16_t PIN12 : 1;
	uint16_t PIN13 : 1;
	uint16_t PIN14 : 1;
	uint16_t PIN15 : 1;
	uint16_t PIN16 : 1;
};
union ReadState_REG
{
	uint16_t all;
	struct ReadState_BITS bit;
};

extern void Init_RdStateGpio(void);
extern uint16_t Read_State(void);
extern void Iint_Switch(void);
extern void Init_CtrlHC138Gpio(void);
extern void CtrlHC138_Y1(uint16_t flag, uint16_t State);
extern void CtrlHC138_Y2(uint16_t flag, uint16_t State);
extern void CtrlHC138_Y3(uint16_t flag, uint16_t State);
extern void CtrlHC138_Y5(uint16_t flag, uint16_t State);

#endif /* INC_RELAY_CTRL_H_ */
