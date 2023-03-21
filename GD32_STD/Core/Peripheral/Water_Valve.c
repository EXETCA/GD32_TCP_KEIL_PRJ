#include "Water_Valve.h"
#include "Alarm.h"
static uint8_t Read_WaterValve(uint8_t ch);
const uint8_t Water_Index[MAX_WATER_SPEED_NUM] = {0, 2, 4, 6, 8, 10,
								 1, 3, 5, 7, 9, 11};
void Read_Water_Valve(void)
{
	C4588EN_ON();
	Delay_Us(100);
	
	for (uint8_t j = 0; j < 8; j++)
	{
		Ctrl_4051INCH(j);
		Delay_Us(500);
		Capture_TIM[0][0] = 0;
		Capture_TIM[0][1] = 0;
		for (uint8_t i = 0; i < 2; i++)
		{
			if (Read_WaterValve(i) == READ_TIMEOUT)
			{
				switch (i)
				{
				case 0:
					SysData.water_MidSpeed[(2 * j)] = 0;
					break;
				case 1:
					SysData.water_MidSpeed[(2 * j + 1)] = 0;
					break;
				default:
					break;
				}
			}
			else
			{
				switch (i)
				{
				case 0:
					if (Capture_TIM[i][0] < Capture_TIM[i][1])
						SysData.water_MidSpeed[(2 * j)] = (Capture_TIM[i][1] - Capture_TIM[i][0]);
					else
						SysData.water_MidSpeed[(2 * j)] = (Capture_TIM[i][1] + 0xffffffff - Capture_TIM[i][0]);
					break;
				case 1:
					if (Capture_TIM[i][0] < Capture_TIM[i][1])
						SysData.water_MidSpeed[(2 * j + 1)] = (Capture_TIM[i][1] - Capture_TIM[i][0]);
					else
						SysData.water_MidSpeed[(2 * j + 1)] = (Capture_TIM[i][1] + 0xffffffff - Capture_TIM[i][0]);
					break;
				default:
					break;
				}
			}
		}
	}
	
	C4588EN_OFF();
	Cali_Water_Valve();
}
void Cali_Water_Valve(void)
{
	for (uint8_t i = 0; i < MAX_WATER_SPEED_NUM; i++)
	{
		SysData.water_Speed[i] = (1 / (0.000002 * SysData.water_MidSpeed[Water_Index[i]])) / 6.6; //(1/((120M/120/2)*CNT)HZ)/6.6L/min
	}
}
static uint8_t Read_WaterValve(uint8_t ch)
{
	uint32_t timeout = 0;
	Capture_Cnt[ch] = 0;

	switch (ch)
	{
	case 0:
		__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_CC2);
		HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
		break;
	case 1:
		__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_CC3);
		HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
		break;
	default:
		break;
	}

	while (Capture_Cnt[ch] < 2)
	{
		Delay_Ms(100);
		if (timeout++ > 20)
			return READ_TIMEOUT;
	}
	return READ_OK;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (TIM2 == htim->Instance)
	{
		if (HAL_TIM_ACTIVE_CHANNEL_2 == htim->Channel)
		{
			Capture_TIM[0][Capture_Cnt[0]] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			if ((++Capture_Cnt[0]) > 1)
			{
				__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_CC2);
				HAL_TIM_IC_Stop(&htim2, TIM_CHANNEL_2);
			}
		}
		else if (HAL_TIM_ACTIVE_CHANNEL_3 == htim->Channel)
		{
			Capture_TIM[1][Capture_Cnt[1]] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
			if ((++Capture_Cnt[1]) > 1)
			{
				__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_CC3);
				HAL_TIM_IC_Stop(&htim2, TIM_CHANNEL_3);
			}
		}
	}
}
