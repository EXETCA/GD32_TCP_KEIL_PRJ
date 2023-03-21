/***************************************************************************
 *
 * File          : AMC7836.h
 *
 * Author        : HuangQiang
 *
 * Date          : 2021.9.22
 *
 * Revision      £º  V1.0
 *
 * Description   :
 *
 ***************************************************************************/
#ifndef _AMC7836_H_
#define _AMC7836_H_

#include "gd32f4xx.h"
/***************************************************************************
***************************************************************************/
#define WRITE_FLAG (0x000000) // 1:read,0:write
#define READ_FLAG (0x8000)    // 1:read,0:write

// Interface Configuration 0,1 Register (address = 0x00) [reset = 0x30]
#define AMC_INFCFG0_ADDRE (0x0000)
#define AMC_INFCFG1_ADDRE (0x0100)

// Device Configuration Register (address = 0x02) [reset = 0x03]
#define AMC_DEVCFG_ADDRE (0x0200)

// ADC Register 16 channel
#define AMC_ADC0_ADDRE_LB (0x0020)
#define AMC_ADC0_ADDRE_HB (0x0021)
#define AMC_ADC1_ADDRE_LB (0x0022)
#define AMC_ADC1_ADDRE_HB (0x0023)
#define AMC_ADC2_ADDRE_LB (0x0024)
#define AMC_ADC2_ADDRE_HB (0x0025)
#define AMC_ADC3_ADDRE_LB (0x0026)
#define AMC_ADC3_ADDRE_HB (0x0027)
#define AMC_ADC4_ADDRE_LB (0x0028)
#define AMC_ADC4_ADDRE_HB (0x0029)
#define AMC_ADC5_ADDRE_LB (0x002A)
#define AMC_ADC5_ADDRE_HB (0x002B)
#define AMC_ADC6_ADDRE_LB (0x002C)
#define AMC_ADC6_ADDRE_HB (0x002D)
#define AMC_ADC7_ADDRE_LB (0x002E)
#define AMC_ADC7_ADDRE_HB (0x002F)
#define AMC_ADC8_ADDRE_LB (0x0030)
#define AMC_ADC8_ADDRE_HB (0x0031)
#define AMC_ADC9_ADDRE_LB (0x0032)
#define AMC_ADC9_ADDRE_HB (0x0033)
#define AMC_ADC10_ADDRE_LB (0x0034)
#define AMC_ADC10_ADDRE_HB (0x0035)
#define AMC_ADC11_ADDRE_LB (0x0036)
#define AMC_ADC11_ADDRE_HB (0x0037)
#define AMC_ADC12_ADDRE_LB (0x0038)
#define AMC_ADC12_ADDRE_HB (0x0039)
#define AMC_ADC13_ADDRE_LB (0x003A)
#define AMC_ADC13_ADDRE_HB (0x003B)
#define AMC_ADC14_ADDRE_LB (0x003C)
#define AMC_ADC14_ADDRE_HB (0x003D)
#define AMC_ADC15_ADDRE_LB (0x003E)
#define AMC_ADC15_ADDRE_HB (0x003F)

// DAC Register 12 channel
#define AMC_DAC0_ADDRE_LB (0x5000)
#define AMC_DAC0_ADDRE_HB (0x5100)

// Device ID
#define AMC_ID_ADDRE_LB (0x0004)
#define AMC_ID_ADDRE_HB (0x0005)

// Power-Down
#define POWER_DOWN0_ADDRE (0xB200)
#define POWER_DOWN1_ADDRE (0xB300)
#define POWER_DOWN2_ADDRE (0xB400)

// POWER-DOWN REGISTER config
// Power-Down 0 Register
#define DAC_PDAC_0TO7_MSK (0x00FF)
#define DAC_PDACA0_MSK (0x0001)
#define DAC_PDACA1_MSK (0x0002)
#define DAC_PDACA2_MSK (0x0004)
#define DAC_PDACA3_MSK (0x0008)
#define DAC_PDACB4_MSK (0x0010)
#define DAC_PDACB5_MSK (0x0020)
#define DAC_PDACB6_MSK (0x0040)
#define DAC_PDACB7_MSK (0x0080)
// Power-Down 1 Register
#define DAC_PDAC_8TO15_MSK (0x00FF)
#define DAC_PDACC8_MSK (0x0001)
#define DAC_PDACC9_MSK (0x0002)
#define DAC_PDACC10_MSK (0x0004)
#define DAC_PDACC11_MSK (0x0008)
#define DAC_PDACD12_MSK (0x0010)
#define DAC_PDACD13_MSK (0x0020)
#define DAC_PDACD14_MSK (0x0040)
#define DAC_PDACD15_MSK (0x0080)
// Power-Down 2 Register
#define ADC_PADC_MSK (0x0001)
#define PREF_MSK (0x0002)

// ADC Configuration Register (address = 0x10) [reset = 0x00]
#define AMC_ADCCONFIG_ADDRE (0x1000)
#define AMC_ADCCONFIG_CFGDATA (0x0090)
// ADC channel
#define AMC_ADCMUXCONFIG0_CHANNEL_ADDRE (0x1300)
#define AMC_ADCMUXCONFIG1_CHANNEL_ADDRE (0x1400)
#define AMC_ADCMUXCONFIG2_CHANNEL_ADDRE (0x1500)
// ADC CHANNEL REGISTER 0 config ADC channel 0~12
// ADC MUX Configuration 0 Register (address = 0x13) [reset = 0x00]
#define AMC_ADCCONFIG_0TO7_MSK (0x00FF)
#define AMC_ADCCONFIG_CH0_MSK (0x0001)
#define AMC_ADCCONFIG_CH1_MSK (0x0002)
#define AMC_ADCCONFIG_CH2_MSK (0x0004)
#define AMC_ADCCONFIG_CH3_MSK (0x0008)
#define AMC_ADCCONFIG_CH4_MSK (0x0010)
#define AMC_ADCCONFIG_CH5_MSK (0x0020)
#define AMC_ADCCONFIG_CH6_MSK (0x0040)
#define AMC_ADCCONFIG_CH7_MSK (0x0080)
// ADC MUX Configuration 1 Register (address = 0x14) [reset = 0x00]
#define AMC_ADCCONFIG_8TO15_MSK (0x00FF)
#define AMC_ADCCONFIG_CH8_MSK (0x0001)
#define AMC_ADCCONFIG_CH9_MSK (0x0002)
#define AMC_ADCCONFIG_CH10_MSK (0x0004)
#define AMC_ADCCONFIG_CH11_MSK (0x0008)
#define AMC_ADCCONFIG_CH12_MSK (0x0010)
#define AMC_ADCCONFIG_CH13_MSK (0x0020)
#define AMC_ADCCONFIG_CH14_MSK (0x0040)
#define AMC_ADCCONFIG_CH15_MSK (0x0080)
// ADC MUX Configuration 2 Register (address = 0x15) [reset = 0x00]
#define AMC_ADCCONFIG_CH16_MSK (0x0001)
#define AMC_ADCCONFIG_CH17_MSK (0x0002)
#define AMC_ADCCONFIG_CH18_MSK (0x0004)
#define AMC_ADCCONFIG_CH19_MSK (0x0008)
#define AMC_ADCCONFIG_CH20_MSK (0x0010)
#define AMC_ADCCONFIG_CHTEMP_MSK (0x0020)

// Register Update Register (address = 0x0F) [reset = 0x00]
#define AMC_REGUPDATA_ADDRE (0x0F00)

// DAC configuration
#define AMC_DAC_RANGE_ADDRE (0x1E00)
#define AMC_DAC_RANGE1_ADDRE (0x1F00)
#define AMC_DAC_RANGEAB (0x0066) // set DAC_A,DAC_B:0 to 10V
#define AMC_DAC_RANGECD (0x0066) // set DAC_C,DAC_D:0 to 10V

// ADC Trigger Register (address = 0xC0) [reset = 0x00]
#define AMC_TRIGGER_ADDRE (0xC000)

#define AMC7836_CS_GPIO_Port GPIOE
#define AMC7836_CS_Pin GPIO_PIN_0

#define AMC7836_SCLK_GPIO_Port GPIOE
#define AMC7836_SCLK_Pin GPIO_PIN_6

#define AMC7836_SDI_GPIO_Port GPIOE
#define AMC7836_SDI_Pin GPIO_PIN_2

#define AMC7836_SDO_GPIO_Port GPIOA
#define AMC7836_SDO_Pin GPIO_PIN_5

#define AMC7836_CS_LOW gpio_bit_write(AMC7836_CS_GPIO_Port, AMC7836_CS_Pin, RESET)
#define AMC7836_CS_HIGH gpio_bit_write(AMC7836_CS_GPIO_Port, AMC7836_CS_Pin, SET)

#define AMC7836_SCLK_LOW gpio_bit_write(AMC7836_SCLK_GPIO_Port, AMC7836_SCLK_Pin, RESET)
#define AMC7836_SCLK_HIGH gpio_bit_write(AMC7836_SCLK_GPIO_Port, AMC7836_SCLK_Pin, SET)

#define AMC7836_SDI_LOW gpio_bit_write(AMC7836_SDI_GPIO_Port, AMC7836_SDI_Pin, RESET)
#define AMC7836_SDI_HIGH gpio_bit_write(AMC7836_SDI_GPIO_Port, AMC7836_SDI_Pin, SET)

#define AMC7836_SDO_DATA gpio_input_bit_get(AMC7836_SDO_GPIO_Port, AMC7836_SDO_Pin)

#define LAEKWATERTEST 0

extern void AMC7836GPIO_Init(void);
extern void AMC7836Write(uint32_t command);
extern uint16_t AMC7836Read(uint16_t command);
extern uint16_t AMCRead_ADC(uint16_t channel);
extern void AMCWrite_DAC(uint16_t channel, uint16_t data);
extern void InitAMC7836(void);
extern void AMC7836ReadADC(void);
extern void AMC7836SetDAC(uint16_t Set_Vol,uint16_t ChanNo);
/***************************************************************************
***************************************************************************/
#endif /* USERHEADERFILE_AMC7836_H_ */
