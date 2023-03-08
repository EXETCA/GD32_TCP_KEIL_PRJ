/******************************************************** 
 * @file: 
 * @brief: 
 * @details: 
 * @Author: ShadowFilter
 * @Date: 2022-07-12 08:57:15
 * @version: 
 * @LastEditors: ShadowFilter
 * @LastEditTime: 2022-08-08 09:13:38
 * @FilePath: \GD32_STD\Core\inc\main.h
 * @Description: 
 **********************************************************/
#ifndef MAIN_H
#define MAIN_H
 #include "gd32f4xx.h"
 #include "core_cm4.h"
 #define RMII_MODE  
 #pragma anon_unions  

union EthDes_RxStatus
{
	uint32_t status;
	struct
	{
		uint8_t PCERR:1;
		uint8_t CERR:1;
		uint8_t DBERR:1;
		uint8_t RERR:1;
		uint8_t RWDT:1;
		uint8_t FRMT:1;
		uint8_t LCO:1;
		uint8_t IPHERR:1;
		uint8_t LDES:1;
		uint8_t FDES:1;
		uint8_t VTAG:1;
		uint8_t OERR:1;
		uint8_t LERR:1;
		uint8_t SAFF:1;
		uint8_t DERR:1;
		uint8_t ERRS:1;
		uint16_t FRML:14;
		uint8_t DAFF:1;
		uint8_t DAV:1;
	};
};
union EthDes_TxStatus
{
	uint32_t status;
	struct
	{
		uint8_t COB:1;
		uint8_t UFE:1;
		uint8_t EXD:1;
		uint8_t COCNT:4;
		uint8_t VFRM:1;
		
		uint8_t ECO:1;
		uint8_t LCO:1;
		uint8_t NCA:1;
		uint8_t LCA:1;
		uint8_t IPPE:1;
		uint8_t FRMF:1;
		uint8_t JT:1;
		uint8_t ES:1;
		
		uint8_t IPHE:1;
		uint8_t TTMSS:1;
		uint8_t RESERVED:2;
		uint8_t TCHM:1;
		uint8_t TERM:1;
		uint8_t CM:2;
		
		uint8_t RESERVED1:1;		
		uint8_t TTSEN:1;
		uint8_t DPAD:1;
		uint8_t DCRC:1;		
		uint8_t FSG:1;
		uint8_t LSG:1;	
		uint8_t INTC:1;		
		uint8_t DAV:1;
	};
};

struct ENET_MAC_DBG_REG
{
	uint8_t MRNI:1;
	uint8_t RXAFS:2;
	uint8_t :1;
	uint8_t RXFW:1;
	uint8_t RXFRS:2;
	uint8_t :1;
	
	uint8_t RXFS:2;
  uint8_t :6;
	
	uint8_t MTNI:1;
	uint8_t SOMT:2;
	uint8_t PCS:1;
	uint8_t TXFRS:2;
	uint8_t TXFW:2;
	uint8_t :1;
	
	uint8_t TXFNE:1;
	uint8_t TXFF:1;
  uint8_t :6;
	
	
		
	
	
	
};

#endif
