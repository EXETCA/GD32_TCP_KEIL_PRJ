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
     vPortEnterCritical();         //�����ٽ��
     return 1;
 }
 
 void sys_arch_unprotect(sys_prot_t pval)
 {
     ( void ) pval;
     vPortExitCritical();      //�˳��ٽ��
 }
 
 #if !NO_SYS
 
 err_t
 sys_sem_new(sys_sem_t *sem, u8_t count)
 {
     /* ���� sem */
     if (count <= 1)
     {
         *sem = xSemaphoreCreateBinary();      //������ֵ�ź���
         if (count == 1)
         {
             sys_sem_signal(*sem);  //�´������ź�������Ч�ģ���Ҫ�ͷ�һ���ź���
         }
     }
     else
         *sem = xSemaphoreCreateCounting(count,count); //���������ź���
 
 #if SYS_STATS
     ++lwip_stats.sys.sem.used;
     if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used)
     {
         lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
     }
 #endif /* SYS_STATS */
 
     if (*sem != SYS_SEM_NULL)
         return ERR_OK;          //�����ɹ�����ERR_OK
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
     /* ɾ�� sem */
     vSemaphoreDelete(*sem);   //ɾ��һ���ź���
     *sem = SYS_SEM_NULL;  //ɾ��֮���ÿ�
 }
 
 
 int sys_sem_valid(sys_sem_t *sem)
 {
     return (*sem != SYS_SEM_NULL);   //�����ź����Ƿ���Ч
 }
 
 
 void
 sys_sem_set_invalid(sys_sem_t *sem)
 {
     *sem = SYS_SEM_NULL;    //�ź�������Ϊ��Ч
 }
 
 /*
  ���timeout������Ϊ�㣬�򷵻�ֵΪ
  �ȴ��ź��������ѵĺ����������
  �ź���δ��ָ��ʱ���ڷ����źţ�����ֵΪ
  SYS_ARCH_TIMEOUT������̲߳��صȴ��ź���
  �ú��������㡣 */
 u32_t
 sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
 {
     u32_t wait_tick = 0;
     u32_t start_tick = 0 ;
 
     //�����ź����Ƿ���Ч
     if (*sem == SYS_SEM_NULL)
         return SYS_ARCH_TIMEOUT;
 
     //���Ȼ�ȡ��ʼ�ȴ��ź�����ʱ�ӽ���
     start_tick = xTaskGetTickCount();
 
     //timeout != 0����Ҫ��ms����ϵͳ��ʱ�ӽ���
     if (timeout != 0)
     {
         //��msת����ʱ�ӽ���
         wait_tick = timeout / portTICK_PERIOD_MS;
         if (wait_tick == 0)
             wait_tick = 1;
     }
     else
         wait_tick = portMAX_DELAY;  //һֱ����
 
     //�ȴ��ɹ�������ȴ���ʱ�䣬����ͱ�ʾ�ȴ���ʱ
     if (xSemaphoreTake(*sem, wait_tick) == pdTRUE)
         return ((xTaskGetTickCount()-start_tick)*portTICK_RATE_MS);
     else
         return SYS_ARCH_TIMEOUT;
 }
 
 void
 sys_sem_signal(sys_sem_t *sem)
 {
     if (xSemaphoreGive( *sem ) != pdTRUE)   //�ͷ��ź���
         SEGGER_RTT_printf(0,"[sys_arch]:sem signal fail!\n");
 }
 
 err_t
 sys_mutex_new(sys_mutex_t *mutex)
 {
     /* ���� sem */
     *mutex = xSemaphoreCreateMutex(); //����������
     if (*mutex != SYS_MRTEX_NULL)
         return ERR_OK;      //�����ɹ�����ERR_OK
     else
     {
         SEGGER_RTT_printf(0,"[sys_arch]:new mutex fail!\n");
         return ERR_MEM;
     }
 }
 
 void
 sys_mutex_free(sys_mutex_t *mutex)
 {
     vSemaphoreDelete(*mutex);   //ɾ��������
 }
 
 void
 sys_mutex_set_invalid(sys_mutex_t *mutex)
 {
     *mutex = SYS_MRTEX_NULL;    //���û�����Ϊ��Ч
 }
 
 void
 sys_mutex_lock(sys_mutex_t *mutex)
 {
     xSemaphoreTake(*mutex,/* ��������� */
                    portMAX_DELAY); /* �ȴ�ʱ�� */
 }
 
 void
 sys_mutex_unlock(sys_mutex_t *mutex)
 {
     xSemaphoreGive( *mutex );//����������
 }
 
 sys_thread_t
 sys_thread_new(const char *name, lwip_thread_fn function,
 		void *arg, int stacksize, int prio)
 {
     sys_thread_t handle = NULL;
     BaseType_t xReturn = pdPASS;
     /* ����һ���߳� */
     xReturn = xTaskCreate((TaskFunction_t )function,  /* �߳���ں��� */
                           (const char*    )name,/* �߳����� */
                           (uint16_t       )stacksize,  /* �߳�ջ��С */
                           (void*          )arg,/* �߳���ں������� */
                           (UBaseType_t    )prio, /* �̵߳����ȼ� */
                           (TaskHandle_t*  )&handle);/* �߳̿��ƿ�ָ�� */
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
     /* ����һ������ */
     *mbox = xQueueCreate((UBaseType_t ) size,/* ����ĳ��� */
                          (UBaseType_t ) sizeof(void *));/* ��Ϣ�Ĵ�С */
 #if SYS_STATS
     ++lwip_stats.sys.mbox.used;
     if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used)
     {
         lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
     }
 #endif /* SYS_STATS */
     if (NULL == *mbox)
         return ERR_MEM;   // �����ɹ�����ERR_OK
 
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
 
     vQueueDelete(*mbox);    //ɾ��һ������
 
 #if SYS_STATS
     --lwip_stats.sys.mbox.used;
 #endif /* SYS_STATS */
 }
 
 int sys_mbox_valid(sys_mbox_t *mbox)
 {
     if (*mbox == SYS_MBOX_NULL)     //�ж������Ƿ���Ч
         return 0;
     else
         return 1;
 }
 
 void
 sys_mbox_set_invalid(sys_mbox_t *mbox)
 {
     *mbox = SYS_MBOX_NULL;  //������ЧΪ��Ч״̬
 }
 
 void
 sys_mbox_post(sys_mbox_t *q, void *msg)
 {
     while (xQueueSend( *q, /* ����ľ�� */
                        &msg,/* ���͵���Ϣ���� */
                        portMAX_DELAY) != pdTRUE); /* �ȴ�ʱ�� */
 }
 
 err_t
 sys_mbox_trypost(sys_mbox_t *q, void *msg)
 {
     if (xQueueSend(*q,&msg,0) == pdPASS) //���Է���һ����Ϣ������������
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
 
     /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
     ulReturn = taskENTER_CRITICAL_FROM_ISR();
 
     if (xQueueSendFromISR(*q,&msg,&pxHigherPriorityTaskWoken)==pdPASS)
     {
         err = ERR_OK;
     }
     //�����Ҫ�Ļ�����һ���߳��л�
     portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
 
     /* �˳��ٽ�� */
     taskEXIT_CRITICAL_FROM_ISR( ulReturn );
 
     return err;
 }
 
 u32_t
 sys_arch_mbox_fetch(sys_mbox_t *q, void **msg, u32_t timeout)
 {
     void *dummyptr;
     u32_t wait_tick = 0;
     u32_t start_tick = 0 ;
 
     if ( msg == NULL )  //�����洢��Ϣ�ĵط��Ƿ���Ч
         msg = &dummyptr;
 
     //���Ȼ�ȡ��ʼ�ȴ��ź�����ʱ�ӽ���
     start_tick = sys_now();
 
     //timeout != 0����Ҫ��ms����ϵͳ��ʱ�ӽ���
     if (timeout != 0)
     {
         //��msת����ʱ�ӽ���
         wait_tick = timeout / portTICK_PERIOD_MS;
         if (wait_tick == 0)
             wait_tick = 1;
     }
     //һֱ����
     else
         wait_tick = portMAX_DELAY;
 
     //�ȴ��ɹ�������ȴ���ʱ�䣬����ͱ�ʾ�ȴ���ʱ
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
 
     //�ȴ��ɹ�������ȴ���ʱ��
     if (xQueueReceive(*q,&(*msg), 0) == pdTRUE)
         return ERR_OK;
     else
         return SYS_MBOX_EMPTY;
 }
 
 #if LWIP_NETCONN_SEM_PER_THREAD
	#error LWIP_NETCONN_SEM_PER_THREAD==1 not supported
 #endif /* LWIP_NETCONN_SEM_PER_THREAD */
 
 #endif /* !NO_SYS */