#ifndef INC_TM1640_H_
#define INC_TM1640_H_
#include "gd32f4xx.h"

#define TM1640_1 1
#define TM1640_2 2

#define Add_address 0x40   //地址�?加模�?
#define Fixed_address 0x44 //固定地址模式
#define Test_Mode 0x48     //测试模式

//对应16�?不同的控�?
#define GRID_1 0xC0
#define GRID_2 0xC1
#define GRID_3 0xC2
#define GRID_4 0xC3
#define GRID_5 0xC4
#define GRID_6 0xC5
#define GRID_7 0xC6
#define GRID_8 0xC7
#define GRID_9 0xC8
#define GRID_10 0xC9
#define GRID_11 0xCA
#define GRID_12 0xCB
#define GRID_13 0xCC
#define GRID_14 0xCD
#define GRID_15 0xCE
#define GRID_16 0xCF

//控制下显示的�?度以�?
#define Light_1 0x88
#define Light_2 0x89
#define Light_3 0x8A
#define Light_4 0x8B
#define Light_5 0x8C
#define Light_6 0x8D
#define Light_7 0x8E
#define Light_8 0x8F
#define Light_off 0x80
#define Light_on 0x88
/*PE11 -> PF11 */
#define DIN1_GPIO_Port GPIOF
#define DIN1_Pin GPIO_PIN_11
/*PB2 -> PE3 */
#define SCLK1_GPIO_Port GPIOE
#define SCLK1_Pin GPIO_PIN_3

#define DIN2_GPIO_Port GPIOB
#define DIN2_Pin GPIO_PIN_1

#define SCLK2_GPIO_Port GPIOB
#define SCLK2_Pin GPIO_PIN_0

#define LedC1_DIN_H gpio_bit_write(DIN1_GPIO_Port, DIN1_Pin, SET);
#define LedC1_DIN_L gpio_bit_write(DIN1_GPIO_Port, DIN1_Pin, RESET);

#define LedC1_CLK_H gpio_bit_write(SCLK1_GPIO_Port, SCLK1_Pin, SET);
#define LedC1_CLK_L gpio_bit_write(SCLK1_GPIO_Port, SCLK1_Pin, RESET);

#define LedC2_DIN_H gpio_bit_write(DIN2_GPIO_Port, DIN2_Pin, SET);
#define LedC2_DIN_L gpio_bit_write(DIN2_GPIO_Port, DIN2_Pin, RESET);

#define LedC2_CLK_H gpio_bit_write(SCLK2_GPIO_Port, SCLK2_Pin, SET);
#define LedC2_CLK_L gpio_bit_write(SCLK2_GPIO_Port, SCLK2_Pin, RESET);

void BatchLightControl(uint8_t TM1640_num, uint8_t *color, uint8_t light);
void SingleLightControl(uint8_t TM1640_num, uint8_t led_num, uint8_t color, uint8_t light);
void Set_TM1640(uint8_t num, uint8_t data);
void TM1640_END(uint8_t flag);
void TM1640_START(uint8_t flag);
void Init_TM1640(void);
void Init_TM1640Gpio(void);
#endif /* INC_TM1640_H_ */
