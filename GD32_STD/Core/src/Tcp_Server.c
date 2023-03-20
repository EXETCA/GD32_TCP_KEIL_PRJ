#include "lwip.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tcpecho.h"
#include "System_var.h"
extern uint8_t Read_IP;
/*外部函数*/
extern void EthernetInterfaceHandler(uint16_t len);
uint32_t all_size=0;
struct Eth_DATA Eth_Data;
void TcpServersTask(void *argument)
{
  /* init code for LWIP */

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  int sock = -1, connected;
  char *recv_data = NULL;
  struct sockaddr_in server_addr, client_addr;
  socklen_t sin_size;
  int recv_data_len;
  int timeout = 100; // 用于设置参数
  uint16_t i = 0, j = 0;
  short sock_SetState;
  /* Infinite loop */
  do
  {
    recv_data = (char *)pvPortMalloc(RECV_DATA);
  } while (recv_data == NULL);
Again:
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    SEGGER_RTT_printf(0, "Socket error\n");
    goto __exit;
  }
  while (lwip_setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &timeout, sizeof(timeout)) != 0) // 对socket进行设置  设置连接延迟
  {
    vTaskDelay(1);
    i++;
    if (i >= 200)
      break;
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = 0x017bA8C0;//0xC0A87B01;
//  server_addr.sin_addr.s_addr |=Read_IP << 24;
	server_addr.sin_port = htons(LOCAL_PORT);
  memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
  while (1)
  {
    uint8_t times = 0; // 计数 绑定多少次
    sock_SetState = bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    if (sock_SetState == 0) // 绑定成功则退出循环
      break;
    if (times++ > 20) // 尝试20次以上则关闭套接字
    {
      goto __exit;
    }
    vTaskDelay(100);
  }
  if (sock_SetState == -1)
  {
    SEGGER_RTT_printf(0, 0, "Unable to bind\n");
    goto __exit;
  }

  if (listen(sock, 5) == -1)
  {
    SEGGER_RTT_printf(0, "Listen error\n");
    goto __exit;
  }
  for (;;)
  {
    sin_size = sizeof(struct sockaddr_in);

    connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
    Eth_Data.socket_descr = connected;
    if (ntohs(client_addr.sin_port) == 0)
      goto __flase;
    SEGGER_RTT_printf(0, "new client connected from (%s, %d)\n",
                      inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    {
      int flag = 1;

      setsockopt(connected,
                 IPPROTO_TCP,   /* set option at TCP level */
                 TCP_NODELAY,   /* name of option */
                 (void *)&flag, /* the cast is historical cruft */
                 sizeof(int));  /* length of option value */
    }

    while (1)
    {
      recv_data_len = recv(connected, recv_data, RECV_DATA, 0);

      if (recv_data_len <= 0)
        break;
      SEGGER_RTT_printf(0, "recv %d len data\n", recv_data_len);   
       
      // 前四个字节固定 加20个字节的命令 2个字节空余 4个字节的长度 共30字节的前置
      for (i = 0; i < MAX_REC_COMSIZE; i++)
      {
        Eth_Data.Rec_Comand[i] = recv_data[i];
      }
      // 分离命令长度 注意大小端问题
      Eth_Data.Ethernet.ETH_ReceiveLength.bytes.lbyte = ((Eth_Data.Rec_Comand[(MAX_REC_COMSIZE - 3)] << 8) | (Eth_Data.Rec_Comand[(MAX_REC_COMSIZE - 4)]));
      Eth_Data.Ethernet.ETH_ReceiveLength.bytes.hbyte = ((Eth_Data.Rec_Comand[(MAX_REC_COMSIZE - 1)] << 8) | (Eth_Data.Rec_Comand[(MAX_REC_COMSIZE - 2)]));
      if (recv_data_len == Eth_Data.Ethernet.ETH_ReceiveLength.data)
      {
				
//		  if (SysCtrl.ConnectFlag != 1)
//	      {
//		     SysCtrl.ConnectFlag = 1;
//	      }
        // 大小端转换 并且转成16位
        for (j = 0; j < (recv_data_len / 2); j++)
        {
          Eth_Data.Ethernet.Data[j] = (recv_data[(2 * j + 1)] << 8) | (recv_data[2 * j]);
        }
//        EthernetInterfaceHandler((recv_data_len));
      }
	  vTaskDelay(10);
#ifdef NET_REST
      if (SysCtrl.ConnectFlag == 0)
        SysCtrl.ConnectFlag = 1;
#endif
    }
  __flase:
    if (connected >= 0)
      closesocket(connected);

    connected = -1;
    vTaskDelay(10);
  }
__exit:
  if (sock >= 0)
  {
    uint8_t ii = 0;
    sock_SetState = closesocket(sock);
    while (sock_SetState != 0)
    {
      vTaskDelay(1);
      sock_SetState = lwip_close(sock);
      ii++;
      if (ii >= 20)
        break;
    }
  }
  goto Again;
  if (recv_data)
    free(recv_data);
  /* USER CODE END 5 */
}