/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#ifndef LWIP_TCPECHO_H
#define LWIP_TCPECHO_H
#include <string.h> /* memset */
#include <stdlib.h> /* atoi */
#include "sys_arch.h"

/* USER CODE BEGIN 0 */
#define DEST_IP_ADDR0               192
#define DEST_IP_ADDR1               168
#define DEST_IP_ADDR2               105
#define DEST_IP_ADDR3               150

#define DEST_PORT                  5001

#define UDP_SERVER_PORT            5002   /* define the UDP local connection port */
#define UDP_CLIENT_PORT            5002   /* define the UDP remote connection port */

#define LOCAL_PORT                 5000

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0                    192
#define IP_ADDR1                    168
#define IP_ADDR2                    123
#define IP_ADDR3                    1

/*NETMASK*/
#define NETMASK_ADDR0               255
#define NETMASK_ADDR1               255
#define NETMASK_ADDR2               255
#define NETMASK_ADDR3                 0

/*Gateway Address*/
#define GW_ADDR0                    192
#define GW_ADDR1                    168
#define GW_ADDR2                    123
#define GW_ADDR3                    1
/* USER CODE END 0 */

#define MAX_REC_COMSIZE  30 //前三十个字节固定 
#define MAX_NUM_COM                sizeof(CMD_Assemble)/sizeof(CMD_Type)  //命令总数 被PC_DATAHandler()引用


#ifndef INC_FLASH_PROGRAM_H_
#define INC_FLASH_PROGRAM_H_


#define ADDR_FLASH_SECTOR_0 ((uint32_t)0x08000000)  /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1 ((uint32_t)0x08004000)  /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2 ((uint32_t)0x08008000)  /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3 ((uint32_t)0x0800C000)  /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4 ((uint32_t)0x08010000)  /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5 ((uint32_t)0x08020000)  /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6 ((uint32_t)0x08040000)  /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7 ((uint32_t)0x08060000)  /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8 ((uint32_t)0x08080000)  /* Base @ of Sector 8, 128 Kbytes */


#define ADDR_FLASH_SECTOR_9 ((uint32_t)0x080A0000)  /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10 ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11 ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define AddressSampleVolB ADDR_FLASH_SECTOR_11
#define AddressControlCurrentKB ADDR_FLASH_SECTOR_9
#define AddressSamplingCurrentKB ADDR_FLASH_SECTOR_10


/*Bootloader 16K*/
#define Bootloader_FLASHAddr ((uint32_t)0x08000000) // bootloader起始地址 32K
#define Bootloader_FLASH_End ((uint32_t)0x08008000) // bootloader结束地址

/*16K*/
#define Bootloader_FLASHAddr ((uint32_t)0x08000000) // bootloader起始地址 32K
#define Bootloader_FLASH_End ((uint32_t)0x08008000) // bootloader结束地址

/*app 128K section 6*/
#define APP_FLASHAddr ((uint32_t)0x08040000) // APP1起始地址
#define APP_FLASH_End ((uint32_t)0x0805ffff) // APP1结束地址
/*app 128K section 7 缓存区用于补充SRAM*/
#define APP_FBuff_Addr ((uint32_t)0x08060000) // 缓存起始地址 对应扇区6
#define APP_FBuff_End ((uint32_t)0x0807ffff) // 缓存结束地址
/*备份区 section 5 128K*/
#define Cache_FLASHAddr ((uint32_t)0x08020000) // APP2起始地址
#define Cache_FLASH_End ((uint32_t)0x0803FFFF) // APP2结束地址
/*app 128K section 9 主程序运行*/
#define APP_FBuff_Addr ((uint32_t)0x08060000) // 缓存起始地址 对应扇区6
#define APP_FBuff_End ((uint32_t)0x0807ffff) // 缓存结束地址
/*app 16K section 3 */
#define Flag_Addr ADDR_FLASH_SECTOR_3 
/*KB */
#define KB_ADDR ADDR_FLASH_SECTOR_9


/*GD32更新程序相关*/
/*app 128K section 6*/
#define APP_Main_Sector					CTL_SECTOR_NUMBER_6
#define APP_Buff_Sector					CTL_SECTOR_NUMBER_7
#define Updata_Ctrl_Sector      CTL_SECTOR_NUMBER_5

#define APP_Buff_ADDRESS        ADDR_FLASH_SECTOR_7
#define Updata_Ctrl_ADDRESS     ADDR_FLASH_SECTOR_5

/*KB值相关存储*/
#define KB_Sector					CTL_SECTOR_NUMBER_9
#define KB_ADDRESS        ADDR_FLASH_SECTOR_9
struct APPDownloadCTRL
{
	uint32_t MAX_Byte;//用于记录总字节数
	uint16_t Num; //用于记录已缓存的数据包
	uint16_t MAX_Num;//用于记录总缓存次数
	uint32_t FlashBuff_Length;//flash 缓存的长度
	uint32_t FlashAPP_Length;//flash 主程序已写入长度
	uint8_t  MAX_FlashWE;//flash 写次数总数
	uint8_t  FlashWECounter;//当前flsh 写的次数
	uint32_t Net_Buf;//网络缓存地址
	uint8_t CompeleteFlag;//完成标志
};

#endif
	

union E_DATA_BYTE
{

	uint32_t data;
	struct {
	        uint16_t lbyte;

			uint16_t hbyte;
			}bytes;
};
struct ETH_TYPE_REG
{  
    union E_DATA_BYTE       ETH_ReceiveLength;
    union E_DATA_BYTE       ETH_Length;
    union E_DATA_BYTE       ETH_PackageLength;
    union E_DATA_BYTE       Flash_Legth;
	
    uint16_t       ETH_Flag;
    uint16_t       Data[1536];//缓存大小转化后的数据
};
struct Eth_DATA
{
	int16_t socket_descr;//socket_ID
 //communication
	struct ETH_TYPE_REG Ethernet;
	uint8_t Error_Flag;
	char Rec_Comand[MAX_REC_COMSIZE];  //分离固定头30个字节
	uint16_t ConnectFlag;
};
typedef struct CMD_Type
{
	char* CMD_String;			
	void (*Run_Function)(void *);
}CMD_Type;
extern struct Eth_DATA Eth_Data;

#endif /* LWIP_TCPECHO_H */
