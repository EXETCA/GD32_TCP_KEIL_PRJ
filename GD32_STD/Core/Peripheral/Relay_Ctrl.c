#include "System_Var.h"
#include "Relay_Ctrl.h"
#include "Cpld_Ctrl.h"
#include "Delay.h"
static uint8_t SwitchUnit[5];

/******************************************************************
函数名称：Init_CtrlHC138
函数功能：初始化Init_CtrlHC138相关引脚
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void Iint_Switch(void)
{
    uint8_t i;

    for (i = 0; i < 5; i++)
    {
        SwitchUnit[i] = 0;
    }
}
/******************************************************************
函数名称：Init_CtrlHC138
函数功能：初始化Init_CtrlHC138相关引脚
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void Init_RdStateGpio(void)
{
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_GPIOC);

    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4 | GPIO_PIN_3);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_3);
	  gpio_af_set(GPIOB,GPIO_AF_15,GPIO_PIN_4 | GPIO_PIN_3);
    gpio_bit_reset(GPIOB, GPIO_PIN_4 | GPIO_PIN_3);		
	
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_3);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_3);
	  gpio_af_set(GPIOD,GPIO_AF_15,GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_3);
    gpio_bit_reset(GPIOD, GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_3);		
		
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_12 | GPIO_PIN_11);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12 | GPIO_PIN_11);
	  gpio_af_set(GPIOC,GPIO_AF_15,GPIO_PIN_12 | GPIO_PIN_11);
    gpio_bit_reset(GPIOC, GPIO_PIN_12 | GPIO_PIN_11);		
}

/******************************************************** 
 * @brief 前八位分别对饮J6 J7 ---J11 其中 J6 J10为3引脚 
 * @return {uint16_t} 返回状态位，此程序 前八位有效;
 * @warning 当J*引脚与地短接、光耦发光、三极管导通、电平拉低
 **********************************************************/
uint16_t Read_State(void)
{
    uint16_t i;
    union ReadState_REG State;

    State.all = 0;

    Y4OC_ON;
    Y4_OFF;
    DELAY_Ms(15);
    Y4OC_OFF;
    Y4_ON;

    for (i = 0; i < 8; i++)
    {
        switch (i)
        {
        case 0:
            (RD0_STATE == 1) ? (State.bit.PIN1 = 1) : (State.bit.PIN1 = 0);
            break;
        case 1:
            (RD1_STATE == 1) ? (State.bit.PIN2 = 1) : (State.bit.PIN2 = 0);
            break;
        case 2:
            (RD2_STATE == 1) ? (State.bit.PIN3 = 1) : (State.bit.PIN3 = 0);
            break;
        case 3:
            (RD3_STATE == 1) ? (State.bit.PIN4 = 1) : (State.bit.PIN4 = 0);
            break;
        case 4:
            (RD4_STATE == 1) ? (State.bit.PIN5 = 1) : (State.bit.PIN5 = 0);
            break;
        case 5:
            (RD5_STATE == 1) ? (State.bit.PIN6 = 1) : (State.bit.PIN6 = 0);
            break;
        case 6:
            (RD6_STATE == 1) ? (State.bit.PIN7 = 1) : (State.bit.PIN7 = 0);
            break;
        case 7:
            (RD7_STATE == 1) ? (State.bit.PIN8 = 1) : (State.bit.PIN8 = 0);
            break;
        default:
            break;
        }
    }
    Y4OC_ON;
    Y4_OFF;
    return (State.all);
}

/******************************************************************
函数名称：Init_CtrlHC138
函数功能：初始化Init_CtrlHC138相关引脚
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void Init_CtrlHC138Gpio(void)
{
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_GPIOF);

	   gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);
	  gpio_af_set(GPIOC,GPIO_AF_15,GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);
    gpio_bit_reset(GPIOC, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);		

    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	  gpio_af_set(GPIOB,GPIO_AF_15,GPIO_PIN_6);
    gpio_bit_reset(GPIOB, GPIO_PIN_6);		
	

    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	  gpio_af_set(GPIOF,GPIO_AF_15,GPIO_PIN_10);
    gpio_bit_reset(GPIOF, GPIO_PIN_10);		
	
    Y1_OFF;
    Y2_OFF;
    Y3_OFF;
    Y4_OFF;
    Y4OC_ON;
}
/******************************************************************
函数名称：REALY_Ctrl
函数功能：继电器控制
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void CtrlHC138_Y1(uint16_t flag, uint16_t State)
{
    uint16_t mode = 0;

    mode = State;
    Y1_OFF;
    Y2_OFF;
    Y3_OFF;

    if (flag == 0)
    {
        SwitchUnit[HC138Y1_NUM] &= ~(0x01 << mode);
    }
    else
    {
        SwitchUnit[HC138Y1_NUM] |= (0x01 << mode);
    }
    DELAY_Ms(1);
    EXGPIO_Write(EN_DBUS_ADDR, SwitchUnit[HC138Y1_NUM]);
    DELAY_Ms(5);
    Y1_ON;
    DELAY_Ms(5);
    Y1_OFF;
    Y2_OFF;
    Y3_OFF;
}

/******************************************************************
函数名称：REALY_Ctrl
函数功能：继电器控制
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void CtrlHC138_Y2(uint16_t flag, uint16_t State)
{
    uint16_t mode = 0;

    mode = State;

    Y1_OFF;
    Y2_OFF;
    Y3_OFF;

    if (flag == 0)
    {
        SwitchUnit[HC138Y2_NUM] &= ~(0x01 << mode);
    }
    else
    {
        SwitchUnit[HC138Y2_NUM] |= (0x01 << mode);
    }
    DELAY_Ms(1);
    EXGPIO_Write(EN_DBUS_ADDR, SwitchUnit[HC138Y2_NUM]);
    DELAY_Ms(5);
    Y2_ON;
    DELAY_Ms(5);
    Y1_OFF;
    Y2_OFF;
    Y3_OFF;
}
/******************************************************************
函数名称：REALY_Ctrl
函数功能：继电器控制
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void CtrlHC138_Y3(uint16_t flag, uint16_t State)
{
    uint16_t mode = 0;

    mode = State;

    Y1_OFF;
    Y2_OFF;
    Y3_OFF;

    if (flag == 0)
    {
        SwitchUnit[HC138Y3_NUM] &= ~(0x01 << mode);
    }
    else
    {
        SwitchUnit[HC138Y3_NUM] |= (0x01 << mode);
    }
    DELAY_Ms(1);
    EXGPIO_Write(EN_DBUS_ADDR, SwitchUnit[HC138Y3_NUM]);
    DELAY_Ms(5);
    Y3_ON;
    DELAY_Ms(5);
    Y1_OFF;
    Y2_OFF;
    Y3_OFF;
}
