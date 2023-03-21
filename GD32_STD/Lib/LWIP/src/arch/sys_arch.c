#include "debug.h"
 
 #include <lwip/opt.h>
 #include <lwip/arch.h>
 
 #include "tcpip.h"
 #include "lwip/init.h"
 #include "lwip/netif.h"
 #include "lwip/sio.h"
 #include "ethernetif.h"
 
 #if !NO_SYS
 #include "sys_arch.h"
 #endif
 #include <lwip/stats.h>
 #include <lwip/debug.h>
 #include <lwip/sys.h>
 
 #include <string.h>
 
 int errno;
 
 
 u32_t lwip_sys_now;
 
 struct sys_timeouts
 {
     struct sys_timeo *next;
 };
 
 struct timeoutlist
 {
     struct sys_timeouts timeouts;
     xTaskHandle pid;
 };
 
 #define SYS_THREAD_MAX 4
 
 static struct timeoutlist s_timeoutlist[SYS_THREAD_MAX];
 
 static u16_t s_nextthread = 0;
 
 u32_t
 sys_jiffies(void)
 {
     lwip_sys_now = xTaskGetTickCount();
     return lwip_sys_now;
 }
 
 u32_t
 sys_now(void)
 {
     lwip_sys_now = xTaskGetTickCount();
     return lwip_sys_now;
 }
 
 void
 sys_init(void)
 {
     int i;
     // Initialize the the per-thread sys_timeouts structures
     // make sure there are no valid pids in the list
     for (i = 0; i < SYS_THREAD_MAX; i++)
     {
         s_timeoutlist[i].pid = 0;
         s_timeoutlist[i].timeouts.next = NULL;
     }
     // keep track of how many threads have been created
     s_nextthread = 0;
 }
 
 struct sys_timeouts *sys_arch_timeouts(void)
 {
     int i;
     xTaskHandle pid;
     struct timeoutlist *tl;
     pid = xTaskGetCurrentTaskHandle( );
     for (i = 0; i < s_nextthread; i++)
     {
         tl = &(s_timeoutlist[i]);
         if (tl->pid == pid)
         {
             return &(tl->timeouts);
         }
     }
     return NULL;
 }
 
 sys_prot_t sys_arch_protect(void)
 {
     vPortEnterCritical();         //进入临界段
     return 1;
 }
 
 void sys_arch_unprotect(sys_prot_t pval)
 {
     ( void ) pval;
     vPortExitCritical();      //退出临界段
 }
 
 #if !NO_SYS
 
 err_t
 sys_sem_new(sys_sem_t *sem, u8_t count)
 {
     /* 创建 sem */
     if (count <= 1)
     {
         *sem = xSemaphoreCreateBinary();      //创建二值信号量
         if (count == 1)
         {
             sys_sem_signal(*sem);  //新创建的信号量是无效的，需要释放一个信号量
         }
     }
     else
         *sem = xSemaphoreCreateCounting(count,count); //创建计数信号量
 
 #if SYS_STATS
     ++lwip_stats.sys.sem.used;
     if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used)
     {
         lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
     }
 #endif /* SYS_STATS */
 
     if (*sem != SYS_SEM_NULL)
         return ERR_OK;          //创建成功返回ERR_OK
     else
     {
 #if SYS_STATS
         ++lwip_stats.sys.sem.err;
 #endif /* SYS_STATS */
//         SEGGER_RTT_printf(0,"[sys_arch]:new sem fail!\n");
         return ERR_MEM;
     }
 }
 
 void
 sys_sem_free(sys_sem_t *sem)
 {
 #if SYS_STATS
     --lwip_stats.sys.sem.used;
 #endif /* SYS_STATS */
     /* 删除 sem */
     vSemaphoreDelete(*sem);   //删除一个信号量
     *sem = SYS_SEM_NULL;  //删除之后置空
 }
 
 
 int sys_sem_valid(sys_sem_t *sem)
 {
     return (*sem != SYS_SEM_NULL);   //返回信号量是否有效
 }
 
 
 void
 sys_sem_set_invalid(sys_sem_t *sem)
 {
     *sem = SYS_SEM_NULL;    //信号量设置为无效
 }
 
 /*
  如果timeout参数不为零，则返回值为
  等待信号量所花费的毫秒数。如果
  信号量未在指定时间内发出信号，返回值为
  SYS_ARCH_TIMEOUT。如果线程不必等待信号量
  该函数返回零。 */
 u32_t
 sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
 {
     u32_t wait_tick = 0;
     u32_t start_tick = 0 ;
 
     //看看信号量是否有效
     if (*sem == SYS_SEM_NULL)
         return SYS_ARCH_TIMEOUT;
 
     //首先获取开始等待信号量的时钟节拍
     start_tick = xTaskGetTickCount();
 
     //timeout != 0，需要将ms换成系统的时钟节拍
     if (timeout != 0)
     {
         //将ms转换成时钟节拍
         wait_tick = timeout / portTICK_PERIOD_MS;
         if (wait_tick == 0)
             wait_tick = 1;
     }
     else
         wait_tick = portMAX_DELAY;  //一直阻塞
 
     //等待成功，计算等待的时间，否则就表示等待超时
     if (xSemaphoreTake(*sem, wait_tick) == pdTRUE)
         return ((xTaskGetTickCount()-start_tick)*portTICK_RATE_MS);
     else
         return SYS_ARCH_TIMEOUT;
 }
 
 void
 sys_sem_signal(sys_sem_t *sem)
 {
     if (xSemaphoreGive( *sem ) != pdTRUE)   //释放信号量
         SEGGER_RTT_printf(0,"[sys_arch]:sem signal fail!\n");
 }
 
 err_t
 sys_mutex_new(sys_mutex_t *mutex)
 {
     /* 创建 sem */
     *mutex = xSemaphoreCreateMutex(); //创建互斥量
     if (*mutex != SYS_MRTEX_NULL)
         return ERR_OK;      //创建成功返回ERR_OK
     else
     {
         SEGGER_RTT_printf(0,"[sys_arch]:new mutex fail!\n");
         return ERR_MEM;
     }
 }
 
 void
 sys_mutex_free(sys_mutex_t *mutex)
 {
     vSemaphoreDelete(*mutex);   //删除互斥量
 }
 
 void
 sys_mutex_set_invalid(sys_mutex_t *mutex)
 {
     *mutex = SYS_MRTEX_NULL;    //设置互斥量为无效
 }
 
 void
 sys_mutex_lock(sys_mutex_t *mutex)
 {
     xSemaphoreTake(*mutex,/* 互斥量句柄 */
                    portMAX_DELAY); /* 等待时间 */
 }
 
 void
 sys_mutex_unlock(sys_mutex_t *mutex)
 {
     xSemaphoreGive( *mutex );//给出互斥量
 }
 
 sys_thread_t
 sys_thread_new(const char *name, lwip_thread_fn function,
 		void *arg, int stacksize, int prio)
 {
     sys_thread_t handle = NULL;
     BaseType_t xReturn = pdPASS;
     /* 创建一个线程 */
     xReturn = xTaskCreate((TaskFunction_t )function,  /* 线程入口函数 */
                           (const char*    )name,/* 线程名字 */
                           (uint16_t       )stacksize,  /* 线程栈大小 */
                           (void*          )arg,/* 线程入口函数参数 */
                           (UBaseType_t    )prio, /* 线程的优先级 */
                           (TaskHandle_t*  )&handle);/* 线程控制块指针 */
     if (xReturn != pdPASS)
     {
         SEGGER_RTT_printf(0,"[sys_arch]:create task fail!err:%#lx\n",xReturn);
         return NULL;
     }
     return handle;
 }
 
 err_t
 sys_mbox_new(sys_mbox_t *mbox, int size)
 {
     /* 创建一个邮箱 */
     *mbox = xQueueCreate((UBaseType_t ) size,/* 邮箱的长度 */
                          (UBaseType_t ) sizeof(void *));/* 消息的大小 */
 #if SYS_STATS
     ++lwip_stats.sys.mbox.used;
     if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used)
     {
         lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
     }
 #endif /* SYS_STATS */
     if (NULL == *mbox)
         return ERR_MEM;   // 创建成功返回ERR_OK
 
     return ERR_OK;
 }
 
 void
 sys_mbox_free(sys_mbox_t *mbox)
 {
     if ( uxQueueMessagesWaiting( *mbox ) )
     {
         /* Line for breakpoint.  Should never break here! */
         portNOP();
 #if SYS_STATS
         lwip_stats.sys.mbox.err++;
 #endif /* SYS_STATS */
     }
 
     vQueueDelete(*mbox);    //删除一个邮箱
 
 #if SYS_STATS
     --lwip_stats.sys.mbox.used;
 #endif /* SYS_STATS */
 }
 
 int sys_mbox_valid(sys_mbox_t *mbox)
 {
     if (*mbox == SYS_MBOX_NULL)     //判断邮箱是否有效
         return 0;
     else
         return 1;
 }
 
 void
 sys_mbox_set_invalid(sys_mbox_t *mbox)
 {
     *mbox = SYS_MBOX_NULL;  //设置有效为无效状态
 }
 
 void
 sys_mbox_post(sys_mbox_t *q, void *msg)
 {
     while (xQueueSend( *q, /* 邮箱的句柄 */
                        &msg,/* 发送的消息内容 */
                        portMAX_DELAY) != pdTRUE); /* 等待时间 */
 }
 
 err_t
 sys_mbox_trypost(sys_mbox_t *q, void *msg)
 {
     if (xQueueSend(*q,&msg,0) == pdPASS) //尝试发送一个消息，非阻塞发送
         return ERR_OK;
     else
         return ERR_MEM;
 }
 
 err_t
 sys_mbox_trypost_fromisr(sys_mbox_t *q, void *msg)
 {
     uint32_t ulReturn;
     err_t err = ERR_MEM;
     BaseType_t pxHigherPriorityTaskWoken;
 
     /* 进入临界段，临界段可以嵌套 */
     ulReturn = taskENTER_CRITICAL_FROM_ISR();
 
     if (xQueueSendFromISR(*q,&msg,&pxHigherPriorityTaskWoken)==pdPASS)
     {
         err = ERR_OK;
     }
     //如果需要的话进行一次线程切换
     portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
 
     /* 退出临界段 */
     taskEXIT_CRITICAL_FROM_ISR( ulReturn );
 
     return err;
 }
 
 u32_t
 sys_arch_mbox_fetch(sys_mbox_t *q, void **msg, u32_t timeout)
 {
     void *dummyptr;
     u32_t wait_tick = 0;
     u32_t start_tick = 0 ;
 
     if ( msg == NULL )  //看看存储消息的地方是否有效
         msg = &dummyptr;
 
     //首先获取开始等待信号量的时钟节拍
     start_tick = sys_now();
 
     //timeout != 0，需要将ms换成系统的时钟节拍
     if (timeout != 0)
     {
         //将ms转换成时钟节拍
         wait_tick = timeout / portTICK_PERIOD_MS;
         if (wait_tick == 0)
             wait_tick = 1;
     }
     //一直阻塞
     else
         wait_tick = portMAX_DELAY;
 
     //等待成功，计算等待的时间，否则就表示等待超时
     if (xQueueReceive(*q,&(*msg), wait_tick) == pdTRUE)
         return ((sys_now() - start_tick)*portTICK_PERIOD_MS);
     else
     {
         *msg = NULL;
         return SYS_ARCH_TIMEOUT;
     }
 }
 
 u32_t
 sys_arch_mbox_tryfetch(sys_mbox_t *q, void **msg)
 {
     void *dummyptr;
     if ( msg == NULL )
         msg = &dummyptr;
 
     //等待成功，计算等待的时间
     if (xQueueReceive(*q,&(*msg), 0) == pdTRUE)
         return ERR_OK;
     else
         return SYS_MBOX_EMPTY;
 }
 
 #if LWIP_NETCONN_SEM_PER_THREAD
	#error LWIP_NETCONN_SEM_PER_THREAD==1 not supported
 #endif /* LWIP_NETCONN_SEM_PER_THREAD */
 
 #endif /* !NO_SYS */