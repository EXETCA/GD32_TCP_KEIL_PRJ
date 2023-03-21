#ifndef ALARM_H
#define ALARM_H

#include "main.h"
#include "Cpld_Ctrl.h"

#define SYS_POWER_CLOSE_OFF() Ctrl_Cpld_Switch(CTRL_IO6, ON_CTRL)
#define UPS_CLOSE_OFF() Ctrl_Cpld_Switch(CTRL_IO7, ON_CTRL)
#define MCU_GPIO_REN8_ON() Ctrl_Cpld_Switch(CTRL_IO8, ON_CTRL)
#define MCU_REN11_ON() Ctrl_Cpld_Switch(CTRL_PWR8, ON_CTRL)
#define MCU_REN10_ON() Ctrl_Cpld_Switch(CTRL_PWR7, ON_CTRL)
#define MCU_REN9_ON() Ctrl_Cpld_Switch(CTRL_PWR6, ON_CTRL)

#define SYS_POWER_CLOSE_ON() Ctrl_Cpld_Switch(CTRL_IO6, OFF_CTRL)
#define UPS_CLOSE_ON() Ctrl_Cpld_Switch(CTRL_IO7, OFF_CTRL)
#define MCU_GPIO_REN8_OFF() Ctrl_Cpld_Switch(CTRL_IO8, OFF_CTRL)
#define MCU_REN11_OFF() Ctrl_Cpld_Switch(CTRL_PWR8, OFF_CTRL)
#define MCU_REN10_OFF() Ctrl_Cpld_Switch(CTRL_PWR7, OFF_CTRL)
#define MCU_REN9_OFF() Ctrl_Cpld_Switch(CTRL_PWR6, OFF_CTRL)

#define YellowLight_ON() Ctrl_Cpld_Switch(CTRL_IO4, ON_CTRL)
#define GreenLight_ON() Ctrl_Cpld_Switch(CTRL_IO3, ON_CTRL)
#define RedLight_ON() Ctrl_Cpld_Switch(CTRL_IO5, ON_CTRL)
#define Beeper_ON() Ctrl_Cpld_Switch(CTRL_IO2, ON_CTRL)
#define YellowLight_OFF() Ctrl_Cpld_Switch(CTRL_IO4, OFF_CTRL)
#define GreenLight_OFF() Ctrl_Cpld_Switch(CTRL_IO3, OFF_CTRL)
#define RedLight_OFF() Ctrl_Cpld_Switch(CTRL_IO5, OFF_CTRL)
#define Beeper_OFF() Ctrl_Cpld_Switch(CTRL_IO2, OFF_CTRL)

#define C4588A0_ON() Ctrl_Cpld_Switch(CTRL_CS8, ON_CTRL)
#define C4588A1_ON() Ctrl_Cpld_Switch(CTRL_CS4, ON_CTRL)
#define C4588A2_ON() Ctrl_Cpld_Switch(CTRL_CS2, ON_CTRL)
#define C4588EN_ON() Ctrl_Cpld_Switch(CTRL_CS6, ON_CTRL)
#define C4588A0_OFF() Ctrl_Cpld_Switch(CTRL_CS8, OFF_CTRL)
#define C4588A1_OFF() Ctrl_Cpld_Switch(CTRL_CS4, OFF_CTRL)
#define C4588A2_OFF() Ctrl_Cpld_Switch(CTRL_CS2, OFF_CTRL)
#define C4588EN_OFF() Ctrl_Cpld_Switch(CTRL_CS6, OFF_CTRL)


extern void Read_SmokeandWater(void);
extern void Read_PowerLoseGpio(void);
extern void Ctrl_4051INCH(uint16_t ChanNo);
#endif
