#include "main.h"
#include "system_var.h"
#include "DataSamp.h"
#include "Alarm.h"
#include "AMC7836.h"

static uint32_t const RD_STATE_Port[MAX_READ_NUM] = {
    RD0_STATE_GPIO_Port,
    RD1_STATE_GPIO_Port,
    RD2_STATE_GPIO_Port,
    RD3_STATE_GPIO_Port,
    RD4_STATE_GPIO_Port,
    RD5_STATE_GPIO_Port,
    RD6_STATE_GPIO_Port,
    RD7_STATE_GPIO_Port,
    RD8_STATE_GPIO_Port,
    RD9_STATE_GPIO_Port};
const static uint16_t RD_STATE_Pin[MAX_READ_NUM] = {
    RD0_STATE_Pin,
    RD1_STATE_Pin,
    RD2_STATE_Pin,
    RD3_STATE_Pin,
    RD4_STATE_Pin,
    RD5_STATE_Pin,
    RD6_STATE_Pin,
    RD7_STATE_Pin,
    RD8_STATE_Pin,
    RD9_STATE_Pin};

void DataCollect(void)
{
    // 状态回读
	if (SysCtrl.ConnectFlag == 1)
	{
	    SysCtrl.DelayProtectCnt++;
		if(SysCtrl.DelayProtectCnt>=10)
		{
			SysCtrl.DelayProtectCnt=20;
			RD_READ();
			Alarm_READ();
			Read_SmokeandWater();
		}
	}
	else
	{
	   SysData.WaterLose = 0;
	   SysData.Somke = 0;
	   SysData.Gate_State = 0;
	   SysData.PowerLose=0;
	   SysCtrl.DelayProtectCnt=0;
	}
	   
	Read_AllTemp();
	
	AgingPack();
}

void Init_RdStateGpio(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
	  rcu_periph_clock_enable(RCU_GPIOB);
	  rcu_periph_clock_enable(RCU_GPIOC);
	  rcu_periph_clock_enable(RCU_GPIOD);
	  rcu_periph_clock_enable(RCU_GPIOE);
	  rcu_periph_clock_enable(RCU_GPIOF);
	  rcu_periph_clock_enable(RCU_GPIOG);

    for (uint16_t i = 0; i < MAX_READ_NUM; i++)
    {
			 gpio_mode_set(RD_STATE_Port[i], GPIO_MODE_INPUT, GPIO_PUPD_NONE, RD_STATE_Pin[i]);
    }
}

uint16_t Read_State(void)
{
    uint16_t i;
    union ReadState_REG State;

    State.all = 0;

    for (i = 0; i < MAX_READ_NUM; i++)
    {
        switch (i)
        {
        case 0:
            (RD0_STATE == 1) ? (State.bit.PIN0 = 1) : (State.bit.PIN0 = 0);
		    //State.bit.PIN0 = 1;
            break;
        case 1:
           // (RD1_STATE == 1) ? (State.bit.PIN1 = 1) : (State.bit.PIN1 = 0);
		    State.bit.PIN1 = 1;
            break;
        case 2:
            (RD2_STATE == 1) ? (State.bit.PIN2 = 1) : (State.bit.PIN2 = 0);
            break;
        case 3:
            //(RD3_STATE == 1) ? (State.bit.PIN3 = 1) : (State.bit.PIN3 = 0);
		State.bit.PIN3 = 1;
            break;
        case 4:
            //(RD4_STATE == 1) ? (State.bit.PIN4 = 1) : (State.bit.PIN4 = 0);
		State.bit.PIN4 = 1;
            break;
        case 5:
            (RD5_STATE == 1) ? (State.bit.PIN5 = 1) : (State.bit.PIN5 = 0);
		//State.bit.PIN5 = 1;
            break;
        case 6:
            //(RD6_STATE == 1) ? (State.bit.PIN6 = 1) : (State.bit.PIN6 = 0);
		State.bit.PIN6 = 1;
            break;
        case 7:
            (RD7_STATE == 1) ? (State.bit.PIN7 = 1) : (State.bit.PIN7 = 0);
            break;
        case 8:
            //(RD8_STATE == 1) ? (State.bit.PIN8 = 1) : (State.bit.PIN8 = 0);
		  State.bit.PIN8 = 1;
            break;
        case 9:
            (RD9_STATE == 1) ? (State.bit.PIN9 = 1) : (State.bit.PIN9 = 0);
		   //State.bit.PIN9 = 1;
            break;

        default:
            break;
        }
    }

    return (State.all);
}
void BackUpData()
{
   uint16_t i;
   SysData.Back_ImData.BLight_Choose=SysCtrl.SetLight_Choose;
   SysData.Back_ImData.BBeep_Switch=SysCtrl.SetBeep_Switch;
   SysData.Back_ImData.BSetFanSpeed=SysCtrl.SetFanSpeed;	
}
void AgingPack(void)
{
	uint16_t i, j;
	union DATA_BYTE temp;

	if (0 == SysData.Aging_Pack)
	{
		for (j = 0; j < MAX_WATER_SPEED_NUM; j++)
		{
			SysData.Aging_Pack_DataA.Water_Speed[j] = SysData.water_Speed[j] * 10;
		}

		for (j = 0; j < COLD_BOARD_TEMP_NUM; j++)
		{
			SysData.Aging_Pack_DataA.Cold_Board_Temp[j] = SysData.Cold_Board_Temp[j];
		}

		for (j = 0; j < SHELL_TEMP_NUM; j++)
		{
			SysData.Aging_Pack_DataA.Shell_Temp[j] = SysData.Shell_Temp[j];
		}

		for (j = 0; j < MAX_WATER_GATE_NUM; j++)
		{
			SysData.Aging_Pack_DataA.Water_Gate[j] = SysCtrl.AMC_AdcVoltageValue[j] * 100;
		}

		for (j = 0; j < ENVI_TEMP_NUM; j++)
		{
			SysData.Aging_Pack_DataA.Envi_Temp[j] = SysData.Envi_Temp[j];
		}

		SysData.Aging_Pack_DataA.Leak_Water_State = SysData.WaterLose;
		SysData.Aging_Pack_DataA.Smoke_State = SysData.Somke;
		SysData.Aging_Pack_DataA.Door_State = SysData.Gate_State;
		SysData.Aging_Pack_DataA.Power_State = SysData.PowerLose;
		SysData.Aging_Pack_DataA.WaterPower_State =SysData.WaterPower_State;
		SysData.Aging_Pack = 1;
	}
	else if (1 == SysData.Aging_Pack)
	{
		for (j = 0; j < MAX_WATER_SPEED_NUM; j++)
		{
			SysData.Aging_Pack_DataB.Water_Speed[j] = SysData.water_Speed[j] * 10;
		}

		for (j = 0; j < COLD_BOARD_TEMP_NUM; j++)
		{
			SysData.Aging_Pack_DataB.Cold_Board_Temp[j] = SysData.Cold_Board_Temp[j];
		}

		for (j = 0; j < SHELL_TEMP_NUM; j++)
		{
			SysData.Aging_Pack_DataB.Shell_Temp[j] = SysData.Shell_Temp[j];
		}

		for (j = 0; j < MAX_WATER_GATE_NUM; j++)
		{
			SysData.Aging_Pack_DataB.Water_Gate[j] = SysCtrl.AMC_AdcVoltageValue[j] * 100;
		}

		for (j = 0; j < ENVI_TEMP_NUM; j++)
		{
			SysData.Aging_Pack_DataB.Envi_Temp[j] = SysData.Envi_Temp[j];
		}

		SysData.Aging_Pack_DataB.Leak_Water_State = SysData.WaterLose;
		SysData.Aging_Pack_DataB.Smoke_State = SysData.Somke;
		SysData.Aging_Pack_DataB.Door_State = SysData.Gate_State;
		SysData.Aging_Pack_DataB.Power_State = SysData.PowerLose;
		SysData.Aging_Pack_DataB.WaterPower_State =SysData.WaterPower_State;
		SysData.Aging_Pack = 0;
	}
} 
