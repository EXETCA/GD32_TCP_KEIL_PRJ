#include "AMC7836.h"
#include "System_Var.h"
/**************************************************************************
函数名称:AMC7836Write(Uint32 command)
函数功能:AMC7836写入一个24位数据（1位R/W + 15位地址 + 8位数据）
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2021年9月22日10:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
void AMC7836GPIO_Init()
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOE);
    
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_6);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_6);
	  gpio_af_set(GPIOE,GPIO_AF_15,GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_6);
    gpio_bit_reset(GPIOE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_6);	


    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
	  gpio_af_set(GPIOA,GPIO_AF_15,GPIO_PIN_5);
}
/**************************************************************************
函数名称:AMC7836Write(Uint32 command)
函数功能:AMC7836写入一个24位数据（1位R/W + 15位地址 + 8位数据）
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2021年9月22日10:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
void AMC7836Write(uint32_t command)
{
    uint16_t i;

    AMC7836_CS_LOW; // CS = 0
    Delay_Us(10);
    for (i = 0; i < 24; i++)
    {
        if (command & 0x800000)
        {
            AMC7836_SDI_HIGH; // din = 1
        }
        else
        {
            AMC7836_SDI_LOW; // din = 0
        }
        Delay_Us(5);
        AMC7836_SCLK_HIGH; // CLK = 1
        Delay_Us(5);
        AMC7836_SCLK_LOW; // CLK = 0
        command <<= 1;
    }
    AMC7836_CS_HIGH; // CS = 1
    AMC7836_SCLK_LOW;
}
/**************************************************************************
函数名称:AMC7836Read(uint16_t command)
函数功能:AMC7836读出一个字节的数据
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2021年9月22日10:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
uint16_t AMC7836Read(uint16_t command)
{
    uint16_t i;
    uint16_t read_data = 0;

    AMC7836_CS_LOW; // CS = 0
    Delay_Us(5);
    for (i = 0; i < 16; i++)
    {
        if (command & 0x8000)
        {
            AMC7836_SDI_HIGH; // din = 1
        }
        else
        {
            AMC7836_SDI_LOW; // din = 0
        }

        AMC7836_SCLK_LOW; // CLK = 0
        Delay_Us(3);
        AMC7836_SCLK_HIGH; // CLK = 1
        Delay_Us(3);
        command <<= 1;
    }
    AMC7836_SDI_LOW;
    Delay_Us(5);
    for (i = 0; i < 8; i++)
    {
        read_data <<= 1;
        AMC7836_SCLK_HIGH; // CLK =1
        Delay_Us(5);
        AMC7836_SCLK_LOW; // CLK =0
        Delay_Us(5);
        if (AMC7836_SDO_DATA == 1) // 输出数据锁存在SCLK的下降沿
        {
            read_data |= 1;
        }
    }
    AMC7836_CS_HIGH; // CS = 1
    AMC7836_SDI_HIGH;
    AMC7836_SCLK_LOW;

    return read_data;
}
/**************************************************************************
函数名称:AMCRead_ADC(uint16_t channel)
函数功能:读取ADC数据，12位ADC
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2021年9月22日10:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
uint16_t AMCRead_ADC(uint16_t channel)
{
    uint16_t data_lb = 0, data_hb = 0, adc_data = 0;

    if (channel > AMC_ADC_NUM)
    {
        return 0;
    }
    channel <<= 1;
    data_lb = AMC7836Read(0x8000 + AMC_ADC0_ADDRE_LB + channel);
    data_hb = AMC7836Read(0x8000 + AMC_ADC0_ADDRE_HB + channel);

    adc_data = (data_hb << 8) | data_lb;

    return adc_data;
}
/**************************************************************************
函数名称:AMCWrite_DAC(uint16_t channel,uint16_t data)
函数功能:写DAC，12位DAC
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2021年9月22日10:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
void AMCWrite_DAC(uint16_t channel, uint16_t data)
{
    if (channel < (AMC_DAC_NUM + 1))
    {
        channel <<= 1; // 乘以2
        channel <<= 8; // 左移8位
        AMC7836Write(WRITE_FLAG | (AMC_DAC0_ADDRE_LB + channel) | (data & 0x00FF));
        AMC7836Write(WRITE_FLAG | (AMC_DAC0_ADDRE_HB + channel) | ((data >> 8) & 0x000F));
    }
}
/**************************************************************************
函数名称:InitAMC7836(void)
函数功能:初始化AMC7836芯片
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2021年9月22日10:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
void InitAMC7836(void)
{
    uint16_t data_lb = 0, data_hb = 0, amc_ID = 0;

    AMC7836Write(WRITE_FLAG | AMC_INFCFG0_ADDRE | 0x0030);
    AMC7836Write(WRITE_FLAG | AMC_INFCFG1_ADDRE | 0x0000); // Interface Configuration 1 Register (address = 0x01) [reset = 0x00]
    AMC7836Write(WRITE_FLAG | AMC_DEVCFG_ADDRE | 0x0003);  // Device Configuration Register (address = 0x02) [reset = 0x03]

    //    //read device ID, Default = 0x0C36h
    data_lb = AMC7836Read(READ_FLAG | AMC_ID_ADDRE_LB);
    data_hb = AMC7836Read(READ_FLAG | AMC_ID_ADDRE_HB);
    amc_ID = (data_hb << 8) | data_lb;
    amc_ID = amc_ID;
    Delay_Us(500);

    // set POWER-DOWN REGISTER, ADC is in normal operating mode
    // the reference buffer amplifier is powered on, DACn is in normal operating mode
    AMC7836Write(WRITE_FLAG | (POWER_DOWN2_ADDRE) | ADC_PADC_MSK | PREF_MSK);
    AMC7836Write(WRITE_FLAG | (POWER_DOWN1_ADDRE) | DAC_PDAC_8TO15_MSK); // 8~15
    AMC7836Write(WRITE_FLAG | (POWER_DOWN0_ADDRE) | DAC_PDAC_0TO7_MSK);  // 0~7
    Delay_Us(500);

    // set ADC convert channel, Default = 0000h
    AMC7836Write(WRITE_FLAG | AMC_ADCMUXCONFIG0_CHANNEL_ADDRE | AMC_ADCCONFIG_0TO7_MSK);  // 0~7
    AMC7836Write(WRITE_FLAG | AMC_ADCMUXCONFIG1_CHANNEL_ADDRE | AMC_ADCCONFIG_8TO15_MSK); // 8~15
    AMC7836Write(WRITE_FLAG | AMC_ADCCONFIG_ADDRE | AMC_ADCCONFIG_CFGDATA);
    Delay_Us(500);

    //    AMC7836Write(WRITE_FLAG|0xB000|0x00FF);
    //    AMC7836Write(WRITE_FLAG|0xB100|0x00FF);
    //
    //    AMC7836Write(WRITE_FLAG|0x1800|0x00FF);
    //    AMC7836Write(WRITE_FLAG|0x1900|0x00FF);
    //
    //    AMC7836Write(WRITE_FLAG|0xB000|0x0000);
    //    AMC7836Write(WRITE_FLAG|0xB100|0x0000);
    //
    //    AMC7836Write(WRITE_FLAG|0x1800|0x0000);
    //    AMC7836Write(WRITE_FLAG|0x1900|0x0000);

    AMC7836Write(WRITE_FLAG | AMC_DAC_RANGE_ADDRE | AMC_DAC_RANGEAB);
    AMC7836Write(WRITE_FLAG | AMC_DAC_RANGE1_ADDRE | AMC_DAC_RANGECD);
    //    AMC7836Write(WRITE_FLAG|0x0F00|0x0010);
    // set ADC convert gain, Default = ffffh
    // 1:input range 0~2*Vref, 0:input range 0~1*Vref
    //    amc7812_spi_write_read(WRITE_FLAG|AMC_ADCONFIG2_GAIN_ADDRE|0xffff);

    data_lb = AMC7836Read(READ_FLAG | AMC_ID_ADDRE_LB);
    data_hb = AMC7836Read(READ_FLAG | AMC_ID_ADDRE_HB);
    SysCtrl.AMC_CHIPID = (data_hb << 8) | data_lb;


    for(uint16_t i=0;i<16;i++)
	{
	   AMCWrite_DAC(i, 4095); // 写DAC设置的电压数据
       AMC7836Write(WRITE_FLAG | AMC_REGUPDATA_ADDRE | 0x0001);                // 先写，再更新。写更新DAC寄存器的数据
	   Delay_Us(1000);
	}
    
    //    //set DAC synchronous or asynchronous mode, Default = 0000h
    //    amc7812_spi_write_read(((0x01<<23)|AMC_DAC_CONFIG_MODE_ADDRE) & (~AMC_DACONFIG_MODE0_MSK));
}
/**************************************************************************
函数名称:AMC7836ReadADC(void)
函数功能:读取ADC数据处理函数
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2021年9月22日10:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
void AMC7836ReadADC(void)
{
    uint16_t data_lb = 0, data_hb = 0;

    data_lb = AMC7836Read(READ_FLAG | AMC_ID_ADDRE_LB);
    data_hb = AMC7836Read(READ_FLAG | AMC_ID_ADDRE_HB);
    SysCtrl.AMC_CHIPID = (data_hb << 8) | data_lb;

    AMC7836Write(WRITE_FLAG | AMC_REGUPDATA_ADDRE | 0x0010);
    AMC7836Write(WRITE_FLAG | AMC_TRIGGER_ADDRE | 0x0001);

    for (uint8_t i = 0; i < AMC_ADC_NUM; i++)
    {
        SysCtrl.AMCRead_AdcVoltageCode[i] = AMCRead_ADC(i);
        SysCtrl.AMC_AdcVoltageValue[i] = 5 * ((SysCtrl.AMCRead_AdcVoltageCode[i] * 5) / 4096 - 2.5);
		if(SysCtrl.AMC_AdcVoltageValue[i]>=10)
		{
		  SysCtrl.AMC_AdcVoltageValue[i]=10;
		}
        Delay_Us(1000);
    }
    /* SysCtrl.AMCRead_AdcVoltageCode[SysCtrl.AMC_AdcCnt] = AMCRead_ADC(SysCtrl.AMC_AdcCnt);
     SysCtrl.AMC_AdcVoltageValue[SysCtrl.AMC_AdcCnt] = 5 * ((SysCtrl.AMCRead_AdcVoltageCode[SysCtrl.AMC_AdcCnt] * 5) / 4096 - 2.5);
     SysCtrl.AMC_AdcCnt++;
     if (SysCtrl.AMC_AdcCnt >= AMC_ADC_SUM)
     {
         SysCtrl.AMC_AdcCnt = 0;
     }*/
}
/**************************************************************************
函数名称:AMC7836SetDAC(void)
函数功能:设置DAC数据处理函数
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2021年9月22日10:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
void AMC7836SetDAC(uint16_t Set_Vol,uint16_t ChanNo)
{
    uint16_t data_lb = 0, data_hb = 0;
    volatile uint16_t DAC_SetValue;

    data_lb = AMC7836Read(READ_FLAG | AMC_ID_ADDRE_LB);
    data_hb = AMC7836Read(READ_FLAG | AMC_ID_ADDRE_HB);
    SysCtrl.AMC_CHIPID = (data_hb << 8) | data_lb;

    DAC_SetValue = (uint16_t)((Set_Vol / 100) / 0.00244);
    if (DAC_SetValue > 4095)
    {
        DAC_SetValue = 4095;
    }
    AMCWrite_DAC(ChanNo, DAC_SetValue); // 写DAC设置的电压数据
    AMC7836Write(WRITE_FLAG | AMC_REGUPDATA_ADDRE | 0x0001);                // 先写，再更新。写更新DAC寄存器的数据
    
}
