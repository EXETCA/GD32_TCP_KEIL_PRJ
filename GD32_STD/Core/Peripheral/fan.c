/********************************************************
 * @file:
 * @brief:
 * @details:
 * @Author: ShadowFilter
 * @Date: 2022-05-05 10:33:34
 * @version:
 * @LastEditors: ShadowFilter
 * @LastEditTime: 2022-05-07 14:01:26
 * @FilePath: \code\2022_03_23\AGE_Light\Use_Source\fan.c
 * @Description:
 **********************************************************/
#include "fan.h"
#include "System_var.h"
// ������ʹ�� PWM����ֹͣ
/********************************************************
 * @brief: PWM IO�ٶ����� 0-100
 * @param {Speed 0-100}
 * @return {void}
 * @see:
 **********************************************************/
extern uint16_t Fan_Speed;
void FAN_SPEED_SET(uint16_t Speed)
{
	Fan_Speed = Speed;
	FAN_REG_CTL(FORCE_HIGH);
	timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,Fan_Speed);
//   timer_primary_output_config(TIMER2, ENABLE);//�߼���ʱ��07����

   timer_enable(TIMER2);

   timer_interrupt_enable(TIMER2,TIMER_INT_UP);    
}

void FAN_Stop(void)
{
	timer_channel_output_mode_config(TIMER2,TIMER_CH_0,TIMER_OC_MODE_HIGH);
	SysCtrl.FAN_State = 0;
}

void FAN_Start(void)
{
	timer_channel_output_mode_config(TIMER2,TIMER_CH_0,TIMER_OC_MODE_PWM0);
	timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,Fan_Speed);
	SysCtrl.FAN_State = Fan_Speed ? 1 : 0;
}
static void FAN_REG_CTL(uint8_t CH0COMCTlL)
{
	uint32_t Reg_CHCTL0;
	CH0COMCTlL &= (0x7);
	Reg_CHCTL0 = TIMER3_CHCTL0;
	Reg_CHCTL0 &= ~(0x7 << CH0COMCTL_BITS);
	Reg_CHCTL0 |= (CH0COMCTlL << CH0COMCTL_BITS);
	TIMER3_CHCTL0 = Reg_CHCTL0;
}

void FAN_Init(void)
{

    timer_deinit(TIMER4);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_TIMER4);

    timer_internal_clock_config(TIMER4);    //ѡ���ڲ�ʱ��CK_TIMER(200M)
	
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_1);

    /* TIMER1����Ϊ0.1ms��ʱ */
    timer_parameter_struct timer_initpara;

    timer_initpara.prescaler         = 4;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;//����prescaler,clockdivision���ոö�ʱ��ʱ������Ϊ1M
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;//������ʽ���ø��ݱ��ؾ���
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;//����Ϊ�����ش���
    timer_initpara.period            = 49;//����Ϊ0.1ms����һ��
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER4, &timer_initpara);
    
    /* TIMER��CH1����ΪPWM���:�ߴ���,������֤ÿ0.1ms����һ�δ��� */
    timer_oc_parameter_struct timer_ocintpara;
    
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER4,TIMER_CH_1,&timer_ocintpara);
    
    timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_1, 25);
    timer_channel_output_mode_config(TIMER4, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER4, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    /* ʹ��TIMERx_CAR�Ĵ�����Ӱ�ӼĴ��� */
    timer_auto_reload_shadow_enable(TIMER4);
    /* ����ͨ�����ʹ�� */
    timer_primary_output_config(TIMER4, ENABLE);

    timer_enable(TIMER4);

    timer_interrupt_enable(TIMER4,TIMER_INT_UP);    

    return;
}
