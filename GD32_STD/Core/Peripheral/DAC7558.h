#ifndef DAC7588
#include "MyInclude.h"
#include "Cpld_Ctrl.h"

#define DAC7588 1
#define Write_Buffer 0x000000
#define DAC_REF 4.5 // DAC的参考电压
#define WriteLoad_Single 0x100000
#define WriteLoad_OtherAll 0x200000
#define Load_A_B 0x300000
#define Load_A_D 0x320000
#define Load_A_f 0x340000
#define Load_A_H 0x360000
#define WriteLoad_A_B 0x380000
#define WriteLoad_A_D 0x3A0000
#define WriteLoad_A_F 0x3C0000
#define WriteLoad_A_H 0x3E0000
//电流控制板1对应DAC1 对应cs1 ，DAC2 对应cs 2 SDI SCL公用
#define DAC7558CS1_H Ctrl_Cpld_Switch(CTRL_DATA8, 1);
#define DAC7558CS1_L Ctrl_Cpld_Switch(CTRL_DATA8, 0);

#define DAC7558CS2_H Ctrl_Cpld_Switch(CTRL_DATA7, 1);
#define DAC7558CS2_L Ctrl_Cpld_Switch(CTRL_DATA7, 0);

// SDI PC13  SCL PC2
#define DAC7558SDI_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
#define DAC7558SDI_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

#define DAC7558CLK_H HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
#define DAC7558CLK_L HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);

extern void Main_DA_Send(float voltage, uint16_t ch, uint32_t command);
void DA_Send(float voltage, uint16_t ch, uint32_t command, uint8_t cs);
extern void DA_GPIO_INI(void);
extern void DA_SING_SEND(float voltage, uint8_t ch);
#endif
