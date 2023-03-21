#ifndef _FLASH_DATA_H
#define _FLASH_DATA_H

#include "main.h"
#define FLASH_WORD 0x800
#define WFLASH_INDEX 256
#define MAX_FLASH_PAGE 40
#define FLASH_WFLAG 0x5A5A

/*Bootloader 16K*/
#define Bootloader_FLASHAddr ((u32)0x08000000) // bootloader起始地址 140K
#define Bootloader_FLASH_End ((u32)0x08022fff) // bootloader结束地址
/*app 180K*/
#define APP_FLASHAddr ((u32)0x08023000) // APP1起始地址
#define APP_FLASH_End ((u32)0x0804ffff) // APP1结束地址
/*备份区 180K*/
#define Cache_FLASHAddr ((u32)0x08050000) // APP2起始地址
#define Cache_FLASH_End ((u32)0x0807CFFF) // APP2结束地址

#define APP_FLASH_MAX (180 * 1024)

/*标志位存放区 1K*/
#define Symbol_FLASHAddr ((u32)0x0807FC00) // 标志位存放区起始地址
#define Symbol_FLASH_End ((u32)0x0807FFFF) // 标志位存放区结束地址

#define Restart_Flag ((u32)0x0807FC00)

// 2048个字节
#define ADDR_FLASH_SECTOR_1 (u32)0x0807D000
// 2048个字节
#define ADDR_FLASH_SECTOR_2 (u32)(ADDR_FLASH_SECTOR_1 + FLASH_WORD)
// 2048个字节
#define ADDR_FLASH_SECTOR_3 (u32)(ADDR_FLASH_SECTOR_2 + FLASH_WORD)
// 2048个字节
#define ADDR_FLASH_SECTOR_4 (u32)(ADDR_FLASH_SECTOR_3 + FLASH_WORD)

// 2048个字节
#define CONCUR_FLASH_B ADDR_FLASH_SECTOR_1

// 2048个字节
#define SAMCUR_FLASH_B ADDR_FLASH_SECTOR_2

// 2048个字节
#define OUTSAMCUR_FLASH_B ADDR_FLASH_SECTOR_3

// 2048个字节
#define TEMP_FLASH_B ADDR_FLASH_SECTOR_4

// extern void WriteIPValueToFlash(uint16_t *source,uint16_t len);
extern void WriteCONInCurKBToFlash(uint16_t *Data, uint16_t len);
extern void WriteSampleInCurKBToFlash(uint16_t *Data, uint16_t len);
extern void WriteSampleOutCurKBToFlash(uint16_t *Data, uint16_t len);
extern void WriteCONInSCurToFlash(uint16_t *Data, uint16_t len);
// extern void WriteTempValueToFlash(uint16_t *source,uint16_t len);

// extern void ReadTempFlashBlock(uint16_t *pdata, uint16_t Length);
extern void ReadConInCurFlashBlock(void);
extern void ReadSampleInCurFlashBlock(void);
extern void ReadSampleOutCurFlashBlock(void);
extern void ReadCONInSCurFlashBlock(void);

extern FLASH_Status iap_write_cache_appbin(char *appbuf, uint32_t appsize, uint16_t number);
extern FLASH_Status iap_write_APP_appbin(char *appbuf, uint32_t appsize, uint16_t number);
extern ErrorStatus WriteAppData(char *recData);

extern uint8_t recAllDataFlag;
#endif
