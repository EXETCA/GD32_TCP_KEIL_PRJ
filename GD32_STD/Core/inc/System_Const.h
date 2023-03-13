#ifndef _SYSTEM_CONST_H
#define _SYSTEM_CONST_H

#ifdef __cplusplus
extern "C"
{
#endif
/**************************************************************************
								 INCLUDES
***************************************************************************/
// 按两个字节为一个长度计算
#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#ifndef Online
#define Online 2
#endif
/* void type */
#ifndef NULL
#define NULL (void *)0
#endif

#ifndef MAX_REC_COMSIZE
#define MAX_REC_COMSIZE  30 //前三十个字节固定 
#endif
/*操控板*/

typedef enum
{
	MAX_WATER_SPEED_NUM=12,
	MAX_WATER_GATE_NUM=12,
	MAX_READ_NUM =10,
	AMC_ADC_NUM =12,
  AMC_DAC_NUM =12,
	MAX_TEMP_NUM = 28,
	COLD_BOARD_TEMP_NUM = 12,
	SHELL_TEMP_NUM = 12,
	ENVI_TEMP_NUM=1 

} TEMP_NUM;
#ifndef CPLD_CTRL_H
typedef enum
{
	ON_CTRL = 0x01,
	OFF_CTRL = 0x00,
	TRUE_CTRL = 0x01,
	FALSE_CTRL = 0x00,

} Ctrl_Out;
#endif

#ifdef __cplusplus
}
#endif

#endif
