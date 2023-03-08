#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

#define MAC_ADDR0  00
#define MAC_ADDR1  0x80
#define MAC_ADDR2  0xE1
#define MAC_ADDR3  00
#define MAC_ADDR4  00
#define MAC_ADDR5  01
err_t ethernetif_init(struct netif *netif);
void ethernetif_input( void * pvParameters );

#endif 
