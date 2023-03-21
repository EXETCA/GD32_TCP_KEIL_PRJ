#include "TM1640.h"
#include "AD8688.h"
#include "interrupt.h"
#include "Cpld_Ctrl.h"
#include "DataSamp.h"
#include "inter_init.h"
#include "alarm.h"
#include "MAX485.h"
#include "AMC7836.h"
#include "Fan.h"
#include "System_Var.h"
#include "tcpecho.h"
extern struct SYS_CTRL SysCtrl;
extern struct SysData SysData;
extern void EXMC_INI(void);
extern uint32_t SystemCoreClock;
extern void Ctrl_OutPutIO(uint8_t flag);
void App_LEDGpioCfg(void)
{
	  rcu_periph_clock_enable(RCU_GPIOA);

    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_3);
	  gpio_af_set(GPIOA,GPIO_AF_15,GPIO_PIN_0 | GPIO_PIN_3);
    gpio_bit_reset(GPIOA, GPIO_PIN_0 | GPIO_PIN_3);		
	
    LED1_OFF();
    LED2_OFF();
}

void InitInnerPeripheral(void)
{
    SysCtrl.ConnectFlag = 0;
    SysCtrl.ConnectCnt = 0;

    App_LEDGpioCfg();

	AMC7836GPIO_Init();
    InitAMC7836();

    MAX485_Init();
    Init_RdStateGpio();

    Read_PowerLoseGpio();	
	
}

void InitAGESysParameter(void)
{
    uint16_t i;

    for (i = 0; i < MAX_READ_NUM; i++)
    {
        SysData.RD[i] = 0;
        SysData.AlarmFlag[i] = 0;
        SysData.AlarmCnt[i] = 0;
    }

	SysData.DelayUPSPowerFlag=0;
	SysData.DelayUPSPower=0;
	
	for(i=0;i<MAX_TEMP_NUM;i++)
	{
	   SysData.Temp_Cnt[i]=0;
	   SysData.LastTemperature[i]=0;
	}
	
    SysData.ReStartflag=0;
	
    SysData.WaterLose = 0;
    SysData.Somke = 0;
    SysData.Gate_State = 0;
	SysData.PowerLose=0;
	SysCtrl.DelayProtectCnt=0;
	
	SysCtrl.SampleWaterCnt=0;
		
	SysData.Aging_Pack=0;
	
	SysCtrl.Temp_Index=0;
}

void InitAGESysContral(void)
{
	uint32_t Flash_Program_Addr = 0;
	uint8_t LIGHT_CHOSE,i;
	uint8_t BEEP_SWITCH;
	
	Flash_Program_Addr = ADDR_FLASH_SECTOR_10;
	LIGHT_CHOSE = *(uint16_t *)(Flash_Program_Addr);
	Flash_Program_Addr=Flash_Program_Addr+4;
	BEEP_SWITCH = *(uint16_t *)(Flash_Program_Addr);
	Flash_Program_Addr=Flash_Program_Addr+4;	
	SysCtrl.SetFanSpeed=*(uint16_t *)(Flash_Program_Addr);
	FAN_SPEED_SET(SysCtrl.SetFanSpeed);
    FAN_Start();   
	
    if (1 == LIGHT_CHOSE)
	{
		if (1 == BEEP_SWITCH)
		{
			Beeper_ON();
			YellowLight_OFF();
			RedLight_ON();
			GreenLight_OFF();
		}
		else
		{
			RedLight_ON();
			Beeper_OFF();
			YellowLight_OFF();
			GreenLight_OFF();
		}
	}
	else if (3 == LIGHT_CHOSE)
	{
		RedLight_OFF();
		Beeper_OFF();
		YellowLight_ON();
		GreenLight_OFF();
	}
	else if (2 == LIGHT_CHOSE)
	{
		RedLight_OFF();
		Beeper_OFF();
		YellowLight_OFF();
		GreenLight_ON();
	}
	else
	{
	    RedLight_OFF();
		Beeper_OFF();
		YellowLight_ON();
		GreenLight_OFF();
	}
	
	SYS_POWER_CLOSE_ON();
  //SYS_POWER_CLOSE_OFF();
    UPS_CLOSE_ON();
  //UPS_CLOSE_OFF();
}
