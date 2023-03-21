#ifndef CPLD_CTRL_H
#define CPLD_CTRL_H
#include "gd32F4xx.h"

#define OUTPUT 1
#define INPUT 2


#define EN_DBUS_ADDR (uint8_t)(0x0001)
#define CS_DBUS_ADDR (uint8_t)(0x0002)
#define DATA_DBUS_ADDR (uint8_t)(0x0003)
#define PWR_CTLBUS_ADDR (uint8_t)(0x0004)
#define IO_CTLBUS_ADDR (uint8_t)(0x0005)
#define FPGAIO_T4A_ADDR (uint8_t)(0x0006)

#define FPGAIO_T4B_ADDR (uint8_t)(0x0009)
#define FPGAIO_T6A_ADDR (uint8_t)(0x000A)
#define FPGAIO_T6B_ADDR (uint8_t)(0x000B)
#define FPGAIO_T11B_ADDR (uint8_t)(0x000C)
#define FPGAIO_T12A_ADDR (uint8_t)(0x000D)
#define FPGAIO_T17A_ADDR (uint8_t)(0x000E)
#define FPGAIO_T18B_ADDR (uint8_t)(0x000F)

#define READSTATE_ADDR (uint8_t)(0x0008)

typedef enum
{
    ADDR_EN_DBUS = 0,
    ADDR_CS_DBUS = 1,
    ADDR_DATA_DBUS = 2,
    ADDR_PWR_DBUS = 3,
    ADDR_IO_DBUS = 4,
} ADDR_Ctrl;

typedef enum
{
    CTRL_EN1 = 0,
    CTRL_EN2 = 1,
    CTRL_EN3 = 2,
    CTRL_EN4 = 3,
    CTRL_EN5 = 4,
    CTRL_EN6 = 5,
    CTRL_EN7 = 6,
    CTRL_EN8 = 7,

    CTRL_CS1 = 8,
    CTRL_CS2 = 9,
    CTRL_CS3 = 10,
    CTRL_CS4 = 11,
    CTRL_CS5 = 12,
    CTRL_CS6 = 13,
    CTRL_CS7 = 14,
    CTRL_CS8 = 15,
    CTRL_DATA1 = 16,
    CTRL_DATA2 = 17,
    CTRL_DATA3 = 18,
    CTRL_DATA4 = 19,
    CTRL_DATA5 = 20,
    CTRL_DATA6 = 21,
    CTRL_DATA7 = 22,
    CTRL_DATA8 = 23,
    CTRL_PWR1 = 24,
    CTRL_PWR2 = 25,
    CTRL_PWR3 = 26,
    CTRL_PWR4 = 27,
    CTRL_PWR5 = 28,
    CTRL_PWR6 = 29,
    CTRL_PWR7 = 30,
    CTRL_PWR8 = 31,
    CTRL_IO1 = 32,
    CTRL_IO2 = 33,
    CTRL_IO3 = 34,
    CTRL_IO4 = 35,
    CTRL_IO5 = 36,
    CTRL_IO6 = 37,
    CTRL_IO7 = 38,
    CTRL_IO8 = 39,

} CPLD_CTRL;
#ifndef _SYSTEM_CONST_H
typedef enum
{
	ON_CTRL=0x01,
	OFF_CTRL=0x00,
	TRUE_CTRL=0x01,
	FALSE_CTRL=0x00,
}Ctrl_Out;
#endif
struct READ_BITS
{
    uint8_t PIN0 : 1;
    uint8_t PIN1 : 1;
    uint8_t PIN2 : 1;
    uint8_t PIN3 : 1;
    uint8_t PIN4 : 1;
    uint8_t PIN5 : 1;
    uint8_t PIN6 : 1;
    uint8_t PIN7 : 1;
};

union READ_REGS
{
    uint8_t all;
    struct READ_BITS bit;
};
extern void EXGPIO_Write(uint8_t adr, uint8_t data);
extern uint8_t EXGPIO_Read(uint8_t adr);
extern void Ctrl_Cpld_Switch(uint16_t ChanNo, uint16_t flag);
extern void Ctrl_OutPutIO(uint8_t flag);
#endif
