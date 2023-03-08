/******************************************************** 
 * @file: 
 * @brief: 
 * @details: 
 * @Author: ShadowFilter
 * @Date: 2022-07-12 08:56:29
 * @version: 
 * @LastEditors: ShadowFilter
 * @LastEditTime: 2022-08-08 09:41:27
 * @FilePath: \GD32_STD\Core\src\main.c
 * @Description: 
 **********************************************************/
/*Ethernet Test*/
#include "main.h"
#include "netconf.h"
#include "lwip/tcp.h"
#include "lwip/timeouts.h"
#include "SEGGER_SYSVIEW.h"
#include "eth_bsp_config.h"
#include "Cpld_ctrl.h"
#include "FreeRTOS.h"
#include "task.h"

#include "SEGGER_RTT.h"
#include "semphr.h"
void Ethernet_init(void);
static void CPLD_Init(void);
static void Cpld_GPIO_Init(void);
static void Eth_LoopBackReset(void);
static void Eth_LoopBackSet(void);
void Delay_Us(uint32_t time);
static void AppTask(void * P);
static TaskHandle_t taskCtl;
static TaskHandle_t TCP_Task;
extern void TcpServersTask(void *argument);
extern void lwip_stack_init(void);
const union EthDes_TxStatus * Des0;
const union EthDes_RxStatus * Des1;
const struct ENET_MAC_DBG_REG *Eth_Ddbug;
extern enet_descriptors_struct  txdesc_tab[ENET_TXBUF_NUM]; 
extern enet_descriptors_struct  rxdesc_tab[ENET_RXBUF_NUM]; 
extern xSemaphoreHandle g_rx_semaphore ;

int main(void)
{
//	SystemInit();
			//中断组设置
//	nvic_priority_group_set(NVIC_PRIGROUP_PRE3_SUB1);
	CPLD_Init();
	SEGGER_RTT_Init();
	EXGPIO_Write(FPGAIO_T4A_ADDR,OFF_CTRL);//复位
  Delay_Us(50);
  EXGPIO_Write(FPGAIO_T4A_ADDR,ON_CTRL);
  Delay_Us(200);	
	Eth_Ddbug =(struct ENET_MAC_DBG_REG *)(0x40028000+0x34);

  SEGGER_SYSVIEW_Conf();
	BaseType_t xReturn = pdPASS;
	Des0 = (const union EthDes_TxStatus * )&txdesc_tab[0].status;
	Des1 = (const union EthDes_RxStatus * )&rxdesc_tab[0].status;
  enet_system_setup();
  lwip_stack_init();
//  enet_enable();  
	//SEGGER_RTT_printf(0,"123213");
	/*
	以太网三个主要线程
	tcpip_thread 监听mesbox
	tcpip_input(用于生成mes，并投递)
	ethernetif_input()监听信号量并发送到 以前ethernet_input处理
	*/
	xReturn = xTaskCreate((TaskFunction_t)AppTask,
	                      (const char *)"MainProcess",
	                      (uint16_t) 512,(void *)0,
	                      (UBaseType_t)15,
	                      (TaskHandle_t *)&taskCtl);
  xReturn = xTaskCreate((TaskFunction_t)TcpServersTask,
	                      (const char *)"TCP_Server",
	                      (uint16_t) 512,(void *)0,
	                      (UBaseType_t)15,
	                      (TaskHandle_t *)&TCP_Task);
	if (pdPASS == xReturn)
		vTaskStartScheduler();
	
//	volatile ErrStatus err;
//	{
//		char T_buff[]={0x01,0x00,0x12,0x22,0x33,0x33,0x01,0x00,0x12,0x22,0x44,0x33,0x08,0x06,0x00,0x01,0x08,0x00,0x06,0x04,\
//			0x00,0x01,0x01,0x00,0x12,0x22,0x33,0x33,0xC0,0xA0,0x7B,0x02,0x00,0x00,0x00,0x00,0x00,0x00};
////			while (1)
////			{
////			err =enet_frame_transmit((uint8_t *)T_buff,sizeof(T_buff)) ;}
////			err=enet_debug_status_get(ENET_RX_ASYNCHRONOUS_FIFO_STATE);
////			err=enet_debug_status_get(ENET_MAC_TRANSMITTER_STATUS);
////			err=enet_debug_status_get(ENET_TXFIFO_READ_STATUS);
//		}
//	while(1);
//	uint8_t R_buff[40];
//	err =  enet_rxframe_size_get();
//	while(1);
}
static void AppTask(void * P)
{
	uint32_t i =0;
	while(1)
	{
		i++;
		vTaskDelay(10);
	}
}
void Ethernet_init(void)
{
	uint8_t MacAddr[6]={0x01,0x00,0x12,0x22,0x33,0x33};
	
	//Eth_LoopBackSet();
	
	enet_mac_address_set(ENET_MAC_ADDRESS0,MacAddr);//配置MAC1地址
	enet_descriptors_chain_init(ENET_DMA_TX);//库函中已经分配描述符表以及数据缓存空间了 txdesc_tab tx_buff
	enet_descriptors_chain_init(ENET_DMA_RX);
	    /* enable ethernet Rx interrrupt */ 
    {   int i;
			  extern enet_descriptors_struct  rxdesc_tab[ENET_RXBUF_NUM];
        for(i=0; i<ENET_RXBUF_NUM; i++){ 
            enet_rx_desc_immediate_receive_complete_interrupt(&rxdesc_tab[i]);
        }
    }
#ifdef CHECKSUM_BY_HARDWARE
    /* enable the TCP, UDP and ICMP checksum insertion for the Tx frames */
    for(uint8_t i=0; i < ENET_TXBUF_NUM; i++){
        enet_transmit_checksum_config(&txdesc_tab[i], ENET_CHECKSUM_TCPUDPICMP_FULL);
    }
#endif /* CHECKSUM_BY_HARDWARE */
}
static void CPLD_Init(void)
{
	Cpld_GPIO_Init();
	rcu_periph_clock_enable(RCU_EXMC);
	exmc_norsram_parameter_struct exmc_struct;
  exmc_norsram_timing_parameter_struct nor_timing_init_struct;  
	nor_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A;
  nor_timing_init_struct.syn_data_latency = EXMC_DATALAT_2_CLK;
  nor_timing_init_struct.syn_clk_division = EXMC_SYN_CLOCK_RATIO_DISABLE;
  nor_timing_init_struct.bus_latency = 0;
  nor_timing_init_struct.asyn_data_setuptime = 5;
  nor_timing_init_struct.asyn_address_holdtime = 3;
  nor_timing_init_struct.asyn_address_setuptime = 5;
	
	exmc_struct.norsram_region =  EXMC_BANK0_NORSRAM_REGION2;
  exmc_struct.write_mode = EXMC_ASYN_WRITE;
  exmc_struct.extended_mode = ENABLE;
  exmc_struct.asyn_wait = DISABLE;
  exmc_struct.nwait_signal = DISABLE;
  exmc_struct.memory_write = ENABLE;
  exmc_struct.nwait_config = EXMC_NWAIT_CONFIG_DURING;
  exmc_struct.wrap_burst_mode = DISABLE;
  exmc_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;
  exmc_struct.burst_mode = DISABLE;
  exmc_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_8B;
  exmc_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;
  exmc_struct.address_data_mux = DISABLE;
  exmc_struct.read_write_timing = &nor_timing_init_struct;
  exmc_struct.write_timing = &nor_timing_init_struct;
	exmc_norsram_init(&exmc_struct);
	exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION2);
}
static void Cpld_GPIO_Init(void)
{
	  //GPIO AF12

  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_GPIOC);
  rcu_periph_clock_enable(RCU_GPIOD);
  rcu_periph_clock_enable(RCU_GPIOE);
  rcu_periph_clock_enable(RCU_GPIOF);
  rcu_periph_clock_enable(RCU_GPIOG);

//XD0-15
//PD
#if 1
  gpio_af_set(GPIOD,GPIO_AF_12,GPIO_PIN_0);
  gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_0);
  gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_0);
  
  gpio_af_set(GPIOD,GPIO_AF_12,GPIO_PIN_1);
  gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_1);
  gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_1);
  
  gpio_af_set(GPIOD,GPIO_AF_12,GPIO_PIN_8);
  gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_8);
  gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_8);
  
  gpio_af_set(GPIOD,GPIO_AF_12,GPIO_PIN_9);
  gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_9);
  gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_9);
  
  gpio_af_set(GPIOD,GPIO_AF_12,GPIO_PIN_10);
  gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_10);
  gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_10);
  
  gpio_af_set(GPIOD,GPIO_AF_12,GPIO_PIN_14);
  gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_14);
  gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_14);
  
  gpio_af_set(GPIOD,GPIO_AF_12,GPIO_PIN_15);
  gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_15);
  gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_15);

//PE
  gpio_af_set(GPIOE,GPIO_AF_12,GPIO_PIN_7);
  gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_7);
  gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_7);
  
  gpio_af_set(GPIOE,GPIO_AF_12,GPIO_PIN_8);
  gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_8);
  gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_8);
  
  gpio_af_set(GPIOE,GPIO_AF_12,GPIO_PIN_9);
  gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_9);
  gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_9);
  
  gpio_af_set(GPIOE,GPIO_AF_12,GPIO_PIN_10);
  gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_10);
  gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_10);
  
  gpio_af_set(GPIOE,GPIO_AF_12,GPIO_PIN_11);
  gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_11);
  gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_11);
  
  gpio_af_set(GPIOE,GPIO_AF_12,GPIO_PIN_12);
  gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_12);
  gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_12);
  
  gpio_af_set(GPIOE,GPIO_AF_12,GPIO_PIN_13);
  gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_13);
  gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_13);
  
  gpio_af_set(GPIOE,GPIO_AF_12,GPIO_PIN_14);
  gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_14);
  gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_14);
  
  gpio_af_set(GPIOE,GPIO_AF_12,GPIO_PIN_15);
  gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_15);
  gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_15);
  

//PF(XA0-4)
  gpio_af_set(GPIOF,GPIO_AF_12,GPIO_PIN_0);
  gpio_mode_set(GPIOF,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_0);
  gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_0);
  
  gpio_af_set(GPIOF,GPIO_AF_12,GPIO_PIN_1);
  gpio_mode_set(GPIOF,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_1);
  gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_1);
  
  gpio_af_set(GPIOF,GPIO_AF_12,GPIO_PIN_2);
  gpio_mode_set(GPIOF,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_2);
  gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_2);
  
  gpio_af_set(GPIOF,GPIO_AF_12,GPIO_PIN_3);
  gpio_mode_set(GPIOF,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_3);
  gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_3);

  gpio_af_set(GPIOF,GPIO_AF_12,GPIO_PIN_4);
  gpio_mode_set(GPIOF,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_4);
  gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_4);
  


//CTRL
  
  gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_4);
  gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_4);
  gpio_af_set(GPIOD,GPIO_AF_12,GPIO_PIN_4);
  
  gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_5);
  gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_5);
  gpio_af_set(GPIOD,GPIO_AF_12,GPIO_PIN_5);
  
  gpio_mode_set(GPIOG,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_10);
  gpio_output_options_set(GPIOG,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_10);
  gpio_af_set(GPIOG,GPIO_AF_12,GPIO_PIN_10);

#endif
}
void Delay_Us(uint32_t time)
{
	uint32_t i ,j = time;
	do{	
	i = 3000;
	while (i--);
	}
	while(j--);
}

static void Eth_LoopBackSet(void)
{
	ErrStatus err;
	/*PHY*/
	err = enet_phyloopback_enable();
	uint16_t temp_phy;
	 enet_phy_write_read(ENET_PHY_READ, PHY_ADDRESS, 17, &temp_phy);
//	temp_phy |= (0x01<<9);
//	 enet_phy_write_read(ENET_PHY_WRITE, PHY_ADDRESS, 17, &temp_phy);
	 enet_phy_write_read(ENET_PHY_READ, PHY_ADDRESS, 17, &temp_phy);
}
/*!
    \brief      after the netif is fully configured, it will be called to initialize the function of telnet, client and udp
    \param[in]  netif: the struct used for lwIP network interface
    \param[out] none
    \retval     none
*/
void lwip_netif_status_callback(struct netif *netif)
{
    if(((netif->flags & NETIF_FLAG_UP) != 0) && (0 != netif->ip_addr.addr)) {
//        /* initilaize the helloGigadevice module telnet 8000 */
//        hello_gigadevice_init();
//        /* initilaize the tcp client: echo 10260 */
//        tcp_client_init();
//        /* initilaize the udp: echo 1025 */
//        udp_echo_init();
    }
}
static void Eth_LoopBackReset(void)
{
	
}
void ENET_IRQHandler(void)
{
//	enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_RS_CLR);
//	enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_NI_CLR);
//	for (uint8_t i =0;i<5;i++)
//{	
//	rxdesc_tab[i].status = 0x80000000;
//}
	    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* frame received */
    if(SET == enet_interrupt_flag_get(ENET_DMA_INT_FLAG_RS)){ 
        /* give the semaphore to wakeup LwIP task */
        xSemaphoreGiveFromISR(g_rx_semaphore, &xHigherPriorityTaskWoken);
    }

    /* clear the enet DMA Rx interrupt pending bits */
    enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_RS_CLR);
    enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_NI_CLR);

    /* switch tasks if necessary */
    if(pdFALSE != xHigherPriorityTaskWoken){
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
}