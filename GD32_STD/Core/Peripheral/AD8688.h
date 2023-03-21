#ifndef INC_AD8688_H_
#define INC_AD8688_H_

#include "GD32f4xx.h"
#define AD_RST_GPIO_Port GPIOE
#define AD_RST_Pin GPIO_PIN_0

#define AD_SYNC_GPIO_Port GPIOF
#define AD_SYNC_Pin GPIO_PIN_6

#define AD_SCLK_GPIO_Port GPIOF
#define AD_SCLK_Pin GPIO_PIN_8

#define AD_MOSI_GPIO_Port GPIOE
#define AD_MOSI_Pin GPIO_PIN_1

#define AD_MISO_GPIO_Port GPIOC
#define AD_MISO_Pin GPIO_PIN_13

#define AD_RST_H gpio_bit_write(AD_RST_GPIO_Port, AD_RST_Pin, SET)
#define AD_RST_L gpio_bit_write(AD_RST_GPIO_Port, AD_RST_Pin, RESET)

#define AD_SYNC_H gpio_bit_write(AD_SYNC_GPIO_Port, AD_SYNC_Pin, SET)
#define AD_SYNC_L gpio_bit_write(AD_SYNC_GPIO_Port, AD_SYNC_Pin, RESET)

#define AD_SCLK_H gpio_bit_write(AD_SCLK_GPIO_Port, AD_SCLK_Pin, SET)
#define AD_SCLK_L gpio_bit_write(AD_SCLK_GPIO_Port, AD_SCLK_Pin, RESET)

#define AD_SDI_H gpio_bit_write(AD_MOSI_GPIO_Port, AD_MOSI_Pin, SET)
#define AD_SDI_L gpio_bit_write(AD_MOSI_GPIO_Port, AD_MOSI_Pin, RESET)

#define AD_SDO gpio_input_bit_get(AD_MISO_GPIO_Port, AD_MISO_Pin)

#define SAMP_CH_0 0xc000
#define SAMP_CH_1 0xc400
#define SAMP_CH_2 0xc800
#define SAMP_CH_3 0xcc00
#define SAMP_CH_4 0xd000
#define SAMP_CH_5 0xd400
#define SAMP_CH_6 0xd800
#define SAMP_CH_7 0xdc00
#define SAMP_CH_AUX 0xe000

/// REG MAP
#define AD_8688_AUTO_CH_EN 0X01
#define AD_8688_PWD_CTRL 0X02
#define AD_8688_FETURE_SEL 0X03
#define AD_8688_CH0_RAGE 0X05
#define AD_8688_CH1_RAGE 0X06
#define AD_8688_CH2_RAGE 0X07
#define AD_8688_CH3_RAGE 0X08
#define AD_8688_CH4_RAGE 0X09
#define AD_8688_CH5_RAGE 0X0A
#define AD_8688_CH6_RAGE 0X0B
#define AD_8688_CH7_RAGE 0X0C
#define AD_8688_COM_READ 0X3F

#define INPUT_RAGE_2_5_REF_PN 0X00
#define INPUT_RAGE_1_25_REF_PN 0X01
#define INPUT_RAGE_2_5_REF_P 0X05
#define INPUT_RAGE_1_25_REF_P 0X06

extern void Init_AD8688Gpio(void);
extern void Init_AD8688(void);
extern void Start8688(void);
extern void ADS8688_Reset(void);
extern uint16_t Read_AD8688(void);
extern void Write_AD8688(uint16_t data);
extern void Samp8688_8ch(void);
extern uint16_t ADS8688_READ_Register(uint16_t data);

extern void Write_Reg_8688(uint8_t Add, uint8_t Data);
extern void Write_Reg_8688(uint8_t Add, uint8_t Data);
extern uint8_t Read_Reg_8688(uint8_t Add);

extern void Init_Ads8688(void);
extern unsigned int Read_8688(unsigned int Samp_Ch);
#endif /* INC_AD8688_H_ */
