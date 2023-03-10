#include "MAX485.h"
#include "delay.h"
#include "System_Const.h"
#include "System_Var.h"
uint8_t Re_Buff[30];
volatile uint8_t Rev_Counter;
static volatile uint8_t Re_Flag;
static volatile uint8_t RT_Flag=0;
extern int SEGGER_RTT_printf(unsigned BufferIndex, const char * sFormat, ...);
#ifdef DMA_Send
void usart_dma_config(void);
#endif
//01 06 00 C8 00 02 89 F5  配置地址 原地址+06命令+寄存器命令200+新地址+校验和
//01 06 00 C8 00 64 09 DF 
//01 06 00 C8 00 65 C8 1F
//01 06 00 C8 00 01 C9 F4
uint8_t Send_Buff[MAX_TEMP_NUM][8] = {
                                    {0x01, 0x04, 0x00, 0x64, 0x00, 0x01, 0x70, 0x15},
									{0x01, 0x04, 0x00, 0x65, 0x00, 0x01, 0x21, 0xD5},
									{0x01, 0x04, 0x00, 0x66, 0x00, 0x01, 0xD1, 0xD5},
									{0x01, 0x04, 0x00, 0x67, 0x00, 0x01, 0x80, 0x15},
									{0x01, 0x04, 0x00, 0x68, 0x00, 0x01, 0xB0, 0x16},
									{0x01, 0x04, 0x00, 0x69, 0x00, 0x01, 0xE1, 0xD6},
									{0x01, 0x04, 0x00, 0x6A, 0x00, 0x01, 0x11, 0xD6},
									{0x01, 0x04, 0x00, 0x6B, 0x00, 0x01, 0x40, 0x16},
									//{0x01, 0x04, 0x00, 0x64, 0x00, 0x08, 0xB0, 0x13},//读8路温度
									
									{0x02, 0x04, 0x00, 0x64, 0x00, 0x01, 0x70, 0x26},
									{0x02, 0x04, 0x00, 0x65, 0x00, 0x01, 0x21, 0xE6},
									{0x02, 0x04, 0x00, 0x66, 0x00, 0x01, 0xD1, 0xE6},
									{0x02, 0x04, 0x00, 0x67, 0x00, 0x01, 0x80, 0x26},
									{0x02, 0x04, 0x00, 0x68, 0x00, 0x01, 0xB0, 0x25},
									{0x02, 0x04, 0x00, 0x69, 0x00, 0x01, 0xE1, 0xE5},
									{0x02, 0x04, 0x00, 0x6A, 0x00, 0x01, 0x11, 0xE5},
									{0x02, 0x04, 0x00, 0x6B, 0x00, 0x01, 0x40, 0x25},//热电偶							// 热电偶
																
									

									{0x64, 0x04, 0x00, 0x64, 0x00, 0x01, 0x79, 0xE0},
									{0x64, 0x04, 0x00, 0x65, 0x00, 0x01, 0x28, 0x20},
									{0x64, 0x04, 0x00, 0x66, 0x00, 0x01, 0xD8, 0x20},
									{0x64, 0x04, 0x00, 0x67, 0x00, 0x01, 0x89, 0xE0},
									{0x64, 0x04, 0x00, 0x68, 0x00, 0x01, 0xB9, 0xE3},
									{0x64, 0x04, 0x00, 0x69, 0x00, 0x01, 0xE8, 0x23},
									
									{0x65, 0x04, 0x00, 0x64, 0x00, 0x01, 0x78, 0x31},
									{0x65, 0x04, 0x00, 0x65, 0x00, 0x01, 0x29, 0xF1},
									{0x65, 0x04, 0x00, 0x66, 0x00, 0x01, 0xD9, 0xF1},
									{0x65, 0x04, 0x00, 0x67, 0x00, 0x01, 0x88, 0x31},
									{0x65, 0x04, 0x00, 0x68, 0x00, 0x01, 0xB8, 0x32},
									{0x65, 0x04, 0x00, 0x69, 0x00, 0x01, 0xE9, 0xF2}
                              }; // 热电阻

void MAX485_Init(void)
{
	nvic_irq_enable(USART5_IRQn, 5, 0);
	/*使能端口*/
	//    rcu_periph_clock_enable(RCU_GPIOA);
	//	  MAX485EN_L();
	//	  gpio_mode_set(MAX485EN_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,MAX485EN_Pin);
	//    gpio_output_options_set(MAX485EN_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MAX485EN_Pin);
	//	  gpio_af_set(MAX485EN_Port,GPIO_AF_15,MAX485EN_Pin);

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART5);

	/* enable GPIO clock */
	rcu_periph_clock_enable(GPIOA);

	/* connect port to USARTx_Tx */
	gpio_af_set(GPIOA, GPIO_AF_8, GPIO_PIN_11);

	/* connect port to USARTx_Rx */
	gpio_af_set(GPIOA, GPIO_AF_8, GPIO_PIN_12);

	/* configure USART Tx as alternate function push-pull */
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

	/* configure USART Rx as alternate function push-pull */
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_12);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);

	/*串口初始化*/
	/* USART configure */
	usart_deinit(USART5);
	usart_baudrate_set(USART5, 9600U);
	usart_parity_config(USART5, USART_PM_NONE);
	usart_word_length_set(USART5, USART_WL_8BIT);
	usart_stop_bit_set(USART5, USART_STB_1BIT);

#ifdef DMA_Send
	usart_interrupt_enable(USART5, USART_INT_RBNE);
	usart_interrupt_enable(USART5, USART_INT_ERR);
	usart_dma_receive_config(USART5, USART_DENR_ENABLE);
	usart_dma_transmit_config(USART5, USART_DENR_DISABLE);
#endif
#ifdef Manual_Send
	usart_dma_receive_config(USART5, USART_RECEIVE_DISABLE);
	usart_dma_transmit_config(USART5, USART_TRANSMIT_DISABLE);
	usart_interrupt_disable(USART5, USART_INT_TBE);
	//    usart_interrupt_enable(USART5,USART_INT_IDLE);
	usart_interrupt_enable(USART5, USART_INT_RBNE); // 开启中断需要在串口使能之前
#endif
	/* USART DMA enable for transmission and reception */
#ifdef DMA_Send
	usart_dma_config();
	/*DMA初始化*/
#endif
	/*串口使能*/
	usart_receive_config(USART5, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART5, USART_TRANSMIT_ENABLE);
	usart_receiver_timeout_threshold_config(USART5, 9600); // 获取完整数据包的两种方法，计数和超时
	usart_receiver_timeout_enable(USART5);
	usart_interrupt_enable(USART5, USART_INT_RT);
	usart_enable(USART5);
}
void MAX485_SendData(uint8_t *p, uint32_t len)
{
	if (len < 1)
		return;
	RT_Flag = 0;
	MAX485EN_H();
	usart_flag_clear(USART5, USART_FLAG_TC);
#ifdef Manual_Send
	for (uint32_t i = 0; i < (len - 1) / 4 + 1; i++)
	{
		usart_data_transmit(USART5, ((uint32_t *)p)[i]);
		SEGGER_RTT_printf(0, "%d", *(uint32_t *)0x40011400);
		while (!usart_flag_get(USART5, USART_FLAG_TBE))
			;
	}
#endif
#ifdef DMA_Send
	dma_channel_enable(DMA1, DMA_CH7);
	usart_dma_receive_config(USART5, USART_DENR_DISABLE);
	usart_dma_transmit_config(USART5, USART_DENT_ENABLE);
	dma_memory_address_config(DMA1, DMA_CH7, DMA_MEMORY_0, (uint32_t)p);
	dma_transfer_number_config(DMA1, DMA_CH7, len);

#endif
	while (!usart_flag_get(USART5, USART_FLAG_TC));
	MAX485EN_L();
#ifdef DMA_Send
	usart_dma_transmit_config(USART5, USART_DENT_DISABLE);
	dma_channel_enable(DMA1, DMA_CH1);
	usart_dma_receive_config(USART5, USART_DENR_ENABLE);
#endif
	while (SET != RT_Flag);

	if ((Re_Buff[0] == 1) || (Re_Buff[0] == 2) || (Re_Buff[0] == 100) || (Re_Buff[0] == 101))
	{
		SysData.Temperature[SysCtrl.Temp_Index] = Re_Buff[3] << 8 | Re_Buff[4];
		if (SysData.Temperature[SysCtrl.Temp_Index] >= 3000)
		{
			SysData.Temperature[SysCtrl.Temp_Index] = 0;
		}
		Re_Flag = 1;
	}
}
uint8_t ReadTemp(void)
{
	uint32_t timeout = 0;

	if (0 == SysData.Aging_Pack)
	{
		SysCtrl.Temp_Index = 0;
		do
		{
			timeout = 0;
			Re_Flag = 0;
			MAX485_SendData(Send_Buff[SysCtrl.Temp_Index], 8);
			while (!Re_Flag)
			{
				Delay_Ms(50);
				if (timeout++ > 20)
				{
					break;
				}
			}
			SysCtrl.Temp_Index++;
		} while (SysCtrl.Temp_Index < 14);
	}
	else if (1 == SysData.Aging_Pack)
	{
		SysCtrl.Temp_Index = 14;
		do
		{
			timeout = 0;
			Re_Flag = 0;
			MAX485_SendData(Send_Buff[SysCtrl.Temp_Index], 8);
			while (!Re_Flag)
			{
				Delay_Ms(50);
				if (timeout++ > 20)
				{
					break;
				}
			}
			SysCtrl.Temp_Index++;
		} while (SysCtrl.Temp_Index < MAX_TEMP_NUM);
	}

	return READ_OK;
}


/*!
		\brief      configure USART DMA
		\param[in]  none
		\param[out] none
		\retval     none
*/
#ifdef DMA_Send

void usart_dma_config(void)
{
	nvic_irq_enable(DMA1_Channel7_IRQn, 5, 0);
	nvic_irq_enable(DMA1_Channel1_IRQn, 5, 0);

	dma_single_data_parameter_struct dma_init_struct;
	/* enable DMA1 */
	rcu_periph_clock_enable(RCU_DMA1);
	/* deinitialize DMA channel7(USART0 tx) */
	dma_deinit(DMA1, DMA_CH7);
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
	dma_init_struct.memory0_addr = (uint32_t)Send_Buff;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
	dma_init_struct.number = 8;
	dma_init_struct.periph_addr = DATA_ADDRESS(USART5); // USART_DATA(USART5);//0x40011404;//DATA_ADDRESS(USART5);
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);
	/* configure DMA mode */
	dma_circulation_disable(DMA1, DMA_CH7);
	dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI5);
	dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);

	dma_deinit(DMA1, DMA_CH1);
	dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
	dma_init_struct.memory0_addr = (uint32_t)Re_Buff;
	dma_single_data_mode_init(DMA1, DMA_CH1, &dma_init_struct);
	/* configure DMA mode */
	dma_circulation_disable(DMA1, DMA_CH1);
	dma_channel_subperipheral_select(DMA1, DMA_CH1, DMA_SUBPERI5);
	dma_interrupt_enable(DMA1, DMA_CH1, DMA_CHXCTL_FTFIE);

	dma_channel_enable(DMA1, DMA_CH7); // 完成一次传输后通道使能自动关闭
	dma_channel_enable(DMA1, DMA_CH1);
}
void DMA1_Channel7_IRQHandler(void)
{
	if (dma_interrupt_flag_get(DMA1, DMA_CH7, DMA_INT_FLAG_FTF))
	{
		dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INT_FLAG_FTF);
		//        usart_dma_receive_config(USART5,USART_DENR_DISABLE);
	}
}

/*!
		\brief      this function handles DMA0_Channel4_IRQHandler interrupt
		\param[in]  none
		\param[out] none
		\retval     none
*/
void DMA1_Channel1_IRQHandler(void)
{
	if (dma_interrupt_flag_get(DMA1, DMA_CH1, DMA_INT_FLAG_FTF))
	{
		dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_FLAG_FTF);
		RT_Flag = SET;
	}
}
#endif
void USART5_IRQHandler(void)
{
	if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_RBNE))
	{
#ifdef Manual_Send
		Re_Buff[Rev_Counter++] = usart_data_receive(USART5);
#endif
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_RT))
	{
#ifdef Manual_Send
		Rev_Counter = 0;
#endif
#ifdef DMA_Send
		usart_dma_receive_config(USART5, USART_DENR_DISABLE);
#endif
		usart_interrupt_flag_clear(USART5, USART_INT_FLAG_RT);
		RT_Flag = SET;
		SEGGER_RTT_printf(0, "USART_INT_FLAG_RT\n");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_ERR_ORERR))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_ERR_ORERR\n");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_IDLE))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_IDLE");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_PERR))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_PERR");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_TC))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_TC");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_EB))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_EB");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_CTS))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_CTS1\n");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_TBE))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_TBE\n");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_LBD))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_LBD\n");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_ERR_ORERR))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_ERR_ORERR\n");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_ERR_NERR))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_ERR_NERR\n");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_CTS))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_CTS\n");
	}
	else if (SET == usart_interrupt_flag_get(USART5, USART_INT_FLAG_ERR_FERR))
	{
		SEGGER_RTT_printf(0, "USART_INT_FLAG_ERR_FERR\n");
	}
}