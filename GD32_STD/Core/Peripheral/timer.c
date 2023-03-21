#include "timer.h"
uint32_t ccnumber, readvalue1, readvalue2, count;
float fre;
void PWM_GPIO_Init(void);
void TimerBase_init();
void TimerBase_init()
{
    rcu_periph_clock_enable(RCU_TIMER11); // 注意参数名前面有待RCU。。
    //	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
    timer_deinit(TIMER11);
    timer_internal_clock_config(TIMER11); // 选择内部时钟CK_TIMER(200M)

    /* TIMER1配置为0.1ms定时 */
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);

    timer_initpara.prescaler = 0;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;    // 根据prescaler,clockdivision最终该定时器时钟评率为1M
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 触发方式设置根据边沿决定
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // 计数向上
    timer_initpara.period = 15;                         // 设置为0.1ms触发一次
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER11, &timer_initpara);
    // 定时器八
    rcu_periph_clock_enable(RCU_TIMER8); // 注意参数名前面有待RCU。。
    timer_deinit(TIMER8);
    timer_internal_clock_config(TIMER8); // 选择内部时钟CK_TIMER(200M)

    /* TIMER1配置为0.1ms定时 */

    timer_initpara.prescaler = 0;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;    // 根据prescaler,clockdivision最终该定时器时钟评率为1M
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 触发方式设置根据边沿决定
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // 计数向上
    timer_initpara.period = 15;                         // 设置为0.1ms触发一次
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER8, &timer_initpara);
}
void PWM_GPIO_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_14);

    gpio_bit_set(GPIOB, GPIO_PIN_14);

    rcu_periph_clock_enable(RCU_GPIOE);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    gpio_af_set(GPIOE, GPIO_AF_3, GPIO_PIN_5);
}
void PWM_OUT_INI()
{
    TimerBase_init();
    PWM_GPIO_Init();

    /* TIMER的CH1配置为PWM输出:边触发,这样保证每0.1ms产生一次触发 */
    timer_oc_parameter_struct timer_ocintpara;

    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER11, TIMER_CH_0, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER11, TIMER_CH_0, 4);
    timer_channel_output_mode_config(TIMER11, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER11, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* 使能TIMERx_CAR寄存器的影子寄存器 */
    timer_auto_reload_shadow_enable(TIMER11);
    /* 所有通道输出使能 */
    timer_primary_output_config(TIMER11, ENABLE);

    timer_interrupt_enable(TIMER11, TIMER_INT_UP);
    timer_enable(TIMER11);
    // 定时器八 输入捕获
    nvic_irq_enable(TIMER0_BRK_TIMER8_IRQn, 5, 0);
    timer_ic_parameter_struct timer_icinitpara;
    timer_icinitpara.icpolarity = TIMER_IC_POLARITY_RISING;
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
    timer_icinitpara.icfilter = 0x0;
    timer_input_capture_config(TIMER8, TIMER_CH_0, &timer_icinitpara);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER8);
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER8, TIMER_INT_CH0);
    /* channel 0 interrupt enable */
    timer_interrupt_enable(TIMER8, TIMER_INT_CH0);

    /* TIMER2 counter enable */
    timer_enable(TIMER8);
}

void TIMER0_BRK_TIMER8_IRQHandler(void)
{
    if (SET == timer_interrupt_flag_get(TIMER8, TIMER_INT_CH0))
    {
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER8, TIMER_INT_CH0);

        if (0 == ccnumber)
        {
            /* read channel 0 capture value */
            readvalue1 = timer_channel_capture_value_register_read(TIMER8, TIMER_CH_0) + 1;
            ccnumber = 1;
        }
        else if (1 == ccnumber)
        {
            /* read channel 0 capture value */
            readvalue2 = timer_channel_capture_value_register_read(TIMER8, TIMER_CH_0) + 1;

            if (readvalue2 > readvalue1)
            {
                count = (readvalue2 - readvalue1);
            }
            else
            {
                count = ((0xFFFF - readvalue1) + readvalue2);
            }

            fre = 1000000 / count;

            ccnumber = 0;
        }
    }
}
