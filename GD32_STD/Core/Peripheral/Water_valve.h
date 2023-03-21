#ifndef WATER_VALVE_H
#define WATER_VALVE_H
#include "main.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim12;
extern uint32_t Capture_TIM[8][2];
extern volatile uint8_t Capture_Cnt[8];
extern void Read_Water_Valve(void);
extern void Cali_Water_Valve(void);
#endif
