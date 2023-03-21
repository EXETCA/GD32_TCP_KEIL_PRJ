#include "System_var.h"
#include "Alarm.h"
#include "DataSamp.h"

const uint8_t RD_Comparative_Basis[] = {0, 1, 0, 1, 1, 0, 1, 0, 1, 1}; // 回读比较基准，与基准相同则清除报警
const uint8_t RD_ImComparative_Basis[] = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0}; // 回读比较基准，与基准相同则清除报警
const uint8_t COLD_BOARD[] = {16, 17, 18, 19, 20, 21, 22, 23,24,25,26,27};
const uint8_t SHELL[] = {0, 1, 2, 3, 4, 5, 6, 7,8,9,10,11};
const uint8_t ENVI[] = {12};


void Read_PowerLoseGpio()
{
	
	  rcu_periph_clock_enable(RCU_GPIOA);
	
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_4);

}
void Read_SmokeandWater(void)
{
	if ( SysData.AlarmFlag[9] == 1)
	{
		SysData.WaterLose = 1;
	}
	else
	{
		SysData.WaterLose = 0;
	}

	if (SysData.AlarmFlag[7] == 1)
	{
		SysData.Somke = 1;
	}
	else
	{
		SysData.Somke = 0;
	}

	if (SysData.AlarmFlag[2] == 1)
	{
		SysData.Gate_State = 1;
	}
	else
	{
		SysData.Gate_State = 0;
	}
	
	if (SysData.AlarmFlag[5] == 1)
	{
		SysData.WaterPower_State = 1;
	}
	else
	{
		SysData.WaterPower_State = 0;
	}
	
	if((SysData.WaterLose==0)&&(SysData.Somke == 0))
	{
	    if(SysData.AlarmFlag[0] == 1)
		{
		   SysData.PowerLose=1;
		}
		else
		{
		   SysData.PowerLose=0;
		}
	}
	
	
	if((SysData.Somke==1)||(SysData.WaterLose==1))
	{
		 SYS_POWER_CLOSE_OFF();		
	}
	else if((SysData.Somke==0)&&(SysData.WaterLose==0))
	{
	     SYS_POWER_CLOSE_ON();
	}
	
}
void Read_AllTemp(void)
{
     uint16_t i,j,cnt;
	
	 for(i=0;i<MAX_TEMP_NUM;i++)
     {
		if(SysData.ReStartflag==0)
		{
		   if(SysData.LastTemperature[i]==SysData.Temperature[i])
		   {
		       SysData.Temp_Cnt[i]++;
			   if(SysData.Temp_Cnt[i]>=10)
			   {
			       SysData.Temp_Cnt[i]=10;				   
			   }
			   if(SysData.Temperature[i]==512)//温度是51.2度则用上一个温度给当前温度
			   {
			       SysData.Temperature[i]=SysData.LastTemperature[i];
			   }
		   }
		   else
		   {
		      SysData.Temp_Cnt[i]=0;
		   }
		}
	    SysData.LastTemperature[i]=SysData.Temperature[i];
	 }
	 cnt=0;
	 for(j=0;j<MAX_TEMP_NUM;j++)
     {
		 if(SysData.Temp_Cnt[j]>=10)
	     {
			 cnt++;		     				   
	     }
	 } 
	 if(cnt>=MAX_TEMP_NUM)
	 {
	    SysData.ReStartflag=1;
//	    RCC->CSR |= RCC_CSR_RMVF;
	    NVIC_SystemReset();
	 }
	 else
     {
	    
	 }
	    	 
	 for(i=0;i<COLD_BOARD_TEMP_NUM;i++)
	 {
	    SysData.Cold_Board_Temp[i]=SysData.Temperature[COLD_BOARD[i]];
	 }
	 
	 for(i=0;i<SHELL_TEMP_NUM;i++)
	 {
	    SysData.Shell_Temp[i]=SysData.Temperature[SHELL[i]];
	 }
	 for(i=0;i<ENVI_TEMP_NUM;i++)
	 {
	    SysData.Envi_Temp[i]=SysData.Temperature[ENVI[i]];
	 }
}

void RD_READ(void)
{
	uint16_t itemp;
	uint16_t Temp;
	Temp = Read_State();
	for (itemp = 0; itemp < MAX_READ_NUM; itemp++)
	{
		SysData.RD[itemp] = ((Temp >> itemp) & 0x1); // 取出各个位的值
	}
}

void Alarm_READ(void)
{
	uint16_t itemp, i;

	for (i = 0; i < MAX_READ_NUM; i++)
	{
		if (SysData.RD[i] == RD_ImComparative_Basis[i])
		{
			if(SysData.AlarmFlag[i] == 0)
			{
			    SysData.AlarmCnt[i]++;			
				if (SysData.AlarmCnt[i] >= 3) 
				{
					SysData.AlarmCnt[i]=0;
					SysData.AlarmFlag[i] = 1;
				}
			}			
		}
		else if (SysData.RD[i] == RD_Comparative_Basis[i])		
		{
			SysData.AlarmFlag[i] = 0;
			SysData.AlarmCnt[i] = 0;
		}
	}
}

void Ctrl_4051INCH(uint16_t ChanNo)
{
	switch (ChanNo)
	{
	case 0:
		C4588A0_OFF();
		C4588A1_OFF();
		C4588A2_OFF();
		break;
	case 1:
		C4588A0_ON();
		C4588A1_OFF();
		C4588A2_OFF();
		break;
	case 2:
		C4588A0_OFF();
		C4588A1_ON();
		C4588A2_OFF();
		break;
	case 3:
		C4588A0_ON();
		C4588A1_ON();
		C4588A2_OFF();
		break;
	case 4:
		C4588A0_OFF();
		C4588A1_OFF();
		C4588A2_ON();
		break;
	case 5:
		C4588A0_ON();
		C4588A1_OFF();
		C4588A2_ON();
		break;
	case 6:
		C4588A0_OFF();
		C4588A1_ON();
		C4588A2_ON();
		break;
	case 7:
		C4588A0_ON();
		C4588A1_ON();
		C4588A2_ON();
		break;
	default:
		C4588A0_ON();
		C4588A1_ON();
		C4588A2_ON();
		break;
	}
}
