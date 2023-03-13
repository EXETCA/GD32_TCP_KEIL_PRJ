#ifndef _SYSTEM_GLOBALPROTOTYPES_H
#define _SYSTEM_GLOBALPROTOTYPES_H

#include "main.h"
#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************
                                 INCLUDES
*************   **************************************************************/

/*********************************************************
初始化相关控制
*********************************************************/

#define GPIO_SetBits(a, b) HAL_GPIO_WritePin(a, b, GPIO_PIN_SET)
#define GPIO_ResetBits(a, b) HAL_GPIO_WritePin(a, b, GPIO_PIN_RESET)
/*********************************************************
 Ethenet communication
*********************************************************/
extern void EthernetInterfaceHandler(uint16_t len);

extern void Delay_Us(uint32_t Time);
extern void Delay_Ms(uint32_t Delay);
/*********************************************************
 ETH
*********************************************************/

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
