#include "RTC.h"
rtc_parameter_struct   rtc_initpara;
uint32_t BKP_VALUE,prescaler_s,prescaler_a;

#define RTC_CLOCK_SOURCE_LXTAL 
void rtc_show_time(void);
void rtc_setup(void);
void rtc_pre_config(void);
void RTC_Init(void)
{
    /* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);
    /* enable the access of the RTC registers */
    pmu_backup_write_enable();
    
    rtc_pre_config();
  
    /* check if RTC has aready been configured */
    if(BKP_VALUE != RTC_BKP0){
        rtc_setup(); 
    }else{
        /* detect the reset source */
        if(RESET != rcu_flag_get(RCU_FLAG_PORRST)){
//            printf("power on reset occurred....\n\r");
        }else if(RESET != rcu_flag_get(RCU_FLAG_EPRST)){
//            printf("external reset occurred....\n\r");
        }
//        printf("no need to configure RTC....\n\r");
        rtc_show_time();
    }

    rcu_all_reset_flag_clear();
//    while (1);
}
void rtc_show_time(void)
{
    rtc_current_time_get(&rtc_initpara);  
//    printf("Current time: %0.2x:%0.2x:%0.2x\n\r", \
//          rtc_initpara.hour, rtc_initpara.minute, rtc_initpara.second);
}

void rtc_setup(void)
{
    /* setup RTC time value */
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = 0x16;
    rtc_initpara.day_of_week = RTC_SATURDAY;
    rtc_initpara.month = RTC_APR;
    rtc_initpara.date = 0x30;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;

    /* RTC current time configuration */
    if(ERROR == rtc_init(&rtc_initpara)){
        
    }else{
//        
//        rtc_show_time();
        RTC_BKP0 = BKP_VALUE;
    }
}
void rtc_pre_config(void)
{
    #if defined (RTC_CLOCK_SOURCE_IRC32K) 
          rcu_osci_on(RCU_IRC32K);
          rcu_osci_stab_wait(RCU_IRC32K);
          rcu_rtc_clock_config(RCU_RTCSRC_IRC32K);
  
          prescaler_s = 0x13F;
          prescaler_a = 0x63;
    #elif defined (RTC_CLOCK_SOURCE_LXTAL)
          rcu_osci_on(RCU_LXTAL);
          rcu_osci_stab_wait(RCU_LXTAL);
          rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
    
          prescaler_s = 0xFF;
          prescaler_a = 0x7F;
    #else
    #error RTC clock source should be defined.
    #endif /* RTC_CLOCK_SOURCE_IRC32K */

    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
}