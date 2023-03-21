#ifndef _SYSTEM_VAR_H
#define _SYSTEM_VAR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "System_Const.h"
#include "stdbool.h"
#include "delay.h"
/**************************************************************************
								 INCLUDES
***************************************************************************/
enum OK_TYPE
{
	READ_OK = 0,
	READ_TIMEOUT = 1
};

union DATA_BYTE
{
	uint32_t data;
	struct
	{
		uint16_t lbyte;
		uint16_t hbyte;
	} bytes;
};

struct Send_Data
{
	uint32_t Water_Speed[MAX_WATER_SPEED_NUM];
	uint32_t Cold_Board_Temp[COLD_BOARD_TEMP_NUM];
	uint32_t Shell_Temp[SHELL_TEMP_NUM];
	uint32_t Water_Gate[MAX_WATER_GATE_NUM];
	uint32_t Envi_Temp[ENVI_TEMP_NUM];
	uint16_t Leak_Water_State;
	uint16_t Smoke_State;
	uint16_t Door_State;
	uint16_t Power_State;
	uint16_t WaterPower_State;
} __attribute__((packed));

struct Back_Data
{
	uint16_t BLight_Choose;
	uint16_t BBeep_Switch;
	uint16_t BSetFanSpeed;	
} __attribute__((packed));

/*系统状态参数设定

修改日期:2022-3-23*/
struct SYS_CTRL
{
	union DATA_BYTE TEMP; // 上传数据长度

	// communication
	int16_t socket_descr;

	char Rec_Comand[MAX_REC_COMSIZE];

	uint16_t Command;

	uint8_t ConnectFlag;
	uint8_t SampleWaterCnt;
	uint8_t ConnectCnt;
	
	uint8_t DelayProtectCnt;
	
	uint16_t SetFanSpeed;
    uint16_t SetLight_Choose;
	uint16_t SetBeep_Switch;
	
	uint16_t AMC_CHIPID;					   // AMC7836片ID
	float AMCRead_AdcVoltageCode[AMC_ADC_NUM]; // ADC读到的AMC电压码段
	float AMC_AdcVoltageValue[AMC_ADC_NUM];	   // ADC读到的AMC电压值
	uint16_t AMC_AdcCnt;					   // ADC采样次数计数

	uint16_t FAN_State;
	uint16_t Temp_Index;
};

struct SysData
{
	uint16_t RD[MAX_READ_NUM];
	uint16_t Temperature[MAX_TEMP_NUM];
    uint16_t Cold_Board_Temp[COLD_BOARD_TEMP_NUM];
	uint16_t Shell_Temp[SHELL_TEMP_NUM];
	uint16_t Envi_Temp[ENVI_TEMP_NUM];
	
	uint16_t Temp_Cnt[MAX_TEMP_NUM];
	uint16_t LastTemperature[MAX_TEMP_NUM];
	uint16_t ReStartflag;
	
	float water_MidSpeed[16];
	float water_Speed[MAX_WATER_SPEED_NUM];

	uint8_t Somke;
	uint8_t WaterLose;
	uint8_t Gate_State;
	uint8_t PowerLose;
	uint8_t WaterPower_State;

	uint16_t AlarmCnt[MAX_READ_NUM];
	uint16_t AlarmFlag[MAX_READ_NUM];
	uint16_t ConnectFlag;
	uint16_t ConnectCnt;
	
	uint16_t DelayUPSPowerFlag;
	uint16_t DelayUPSPower;
	
	uint16_t Aging_Pack;
	
	struct Send_Data Aging_Pack_DataA;
	struct Send_Data Aging_Pack_DataB;
	
	struct Back_Data  Back_ImData;
};

///////////////////////////////////////////////////////////////////////////////
// 校准KB值数据
///////////////////////////////////////////////////////////////////////////////

struct KB_VALUE
{
	union DATA_BYTE KVal;
	union DATA_BYTE BVal;
};

typedef struct CUR_CORRECTION
{
	union DATA_BYTE KVal;
	union DATA_BYTE BVal;
} KB_TYPE;

extern struct SYS_CTRL SysCtrl;
extern struct SysData SysData;

//extern RTC_HandleTypeDef hrtc;
//extern TIM_HandleTypeDef htim3;
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

//===========================================================================
// End of file.
//===========================================================================
