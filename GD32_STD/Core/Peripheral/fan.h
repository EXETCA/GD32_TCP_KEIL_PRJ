/********************
 * @Author: ShadowFilter
 * @Date: 2022-05-05 10:33:34
 * @LastEditors: ShadowFilter
 * @LastEditTime: 2022-05-07 10:45:25
 * @FilePath: \2022_03_23\AGE_Light\Use_Headfile\fan.h
 * @Description: PWM的头文件
 *********************/

#ifndef FAN_H
#define FAN_H
#include "main.h"
#define Fan_Port GPIOG
#define Fan_PIN GPIO_PIN_14
#define Fan_AF GPIO_AF15_EVENTOUT
#define TIMERx_CHCTL0_OFFSET 0x18
#define TIMER2_Base 0x40000000
#define TIMER3_Base 0x40000400
#define TIMER4_Base 0x40000800
#define TIMER5_Base 0x40000C00
#define TIMER3_CHCTL0 *((uint32_t *)(TIMER3_Base + TIMERx_CHCTL0_OFFSET))
#define CH0COMCTL_BITS 4
#endif
enum
{
	TIMING_MODE = 0,
	SET_CH_OUT,
	CLEAR_CH_OUT,
	TOGGLE_ON_MATCH,
	FORCE_LOW,
	FORCE_HIGH,
	PWM_MODE_0,
	PWM_MODE_1
};
extern void FAN_SPEED_SET(uint16_t Speed);
extern void FAN_Stop(void);
extern void FAN_Start(void);
static void FAN_REG_CTL(uint8_t CH0COMCTlL);
