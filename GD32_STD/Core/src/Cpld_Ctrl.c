#include "Cpld_Ctrl.h"

static uint16_t Cpld_Data[5];


/******************************************************** 
 * @brief 写
 * @param {uint8_t} adr
 * @param {uint8_t} data
 * @return {void}
 * @warning 
 **********************************************************/
void EXGPIO_Write(uint8_t adr, uint8_t data)
{
  uint8_t * p;
	p = (uint8_t *)(0x68000000+adr);
	*p = data;
}

/******************************************************** 
 * @brief 读
 * @param {uint8_t} adr
 * @return {void}
 * @warning 
 **********************************************************/
uint8_t EXGPIO_Read(uint8_t adr)
{
	uint8_t * p;
	p = (uint8_t *)(0x68000000+adr);
	return *p;
}

/******************************************************** 
 * @brief 全写
 * @param {uint8_t} flag
 * @return {void}
 * @warning 
 **********************************************************/
void Ctrl_OutPutIO(uint8_t flag)
{
	uint8_t Read_Data;
	switch (flag)
	{
		case OFF_CTRL:
			EXGPIO_Write(EN_DBUS_ADDR, 0x00);
			EXGPIO_Write(CS_DBUS_ADDR, 0x00);
			EXGPIO_Write(DATA_DBUS_ADDR, 0x00);
			EXGPIO_Write(PWR_CTLBUS_ADDR, 0x00);
			EXGPIO_Write(IO_CTLBUS_ADDR, 0x00);

			EXGPIO_Write(FPGAIO_T4B_ADDR, 0x00);
			EXGPIO_Write(FPGAIO_T6A_ADDR, 0x00);
			EXGPIO_Write(FPGAIO_T6B_ADDR, 0x00);
			EXGPIO_Write(FPGAIO_T11B_ADDR, 0x00);
			EXGPIO_Write(FPGAIO_T12A_ADDR, 0x00);
			EXGPIO_Write(FPGAIO_T17A_ADDR, 0x00);
			EXGPIO_Write(FPGAIO_T18B_ADDR, 0x00);
			break;

		case ON_CTRL:
			EXGPIO_Write(EN_DBUS_ADDR, 0xff);
			EXGPIO_Write(CS_DBUS_ADDR, 0xff);
			EXGPIO_Write(DATA_DBUS_ADDR, 0xff);
			EXGPIO_Write(PWR_CTLBUS_ADDR, 0xff);
			EXGPIO_Write(IO_CTLBUS_ADDR, 0xff);

			EXGPIO_Write(FPGAIO_T4B_ADDR, 0x01);
			EXGPIO_Write(FPGAIO_T6A_ADDR, 0x01);
			EXGPIO_Write(FPGAIO_T6B_ADDR, 0x01);
			EXGPIO_Write(FPGAIO_T11B_ADDR, 0x01);
			EXGPIO_Write(FPGAIO_T12A_ADDR, 0x01);
			EXGPIO_Write(FPGAIO_T17A_ADDR, 0x01);
			EXGPIO_Write(FPGAIO_T18B_ADDR, 0x01);
			break;
		default:
			Read_Data = EXGPIO_Read(READSTATE_ADDR);
			break;
	}

	for (uint16_t i = 0; i < 5; i++)
	{
		Cpld_Data[i] = 0;
	}
}
/******************************************************** 
 * @brief 切单个位
 * @param {uint16_t} ChanNo
 * @param {uint16_t} flag
 * @return {void}
 * @warning 
 **********************************************************/
void Ctrl_Cpld_Switch(uint16_t ChanNo, uint16_t flag)
{
	volatile uint16_t bNo, bit;
	volatile uint16_t csbNo = 0;

	bNo = ChanNo / 8;
	bit = ChanNo % 8;

	if (flag == 1)
	{
		Cpld_Data[bNo] |= (0x01 << bit);
	}
	else
	{
		Cpld_Data[bNo] &= ~(0x01 << bit);
	}

	switch (bNo)
	{
		case ADDR_EN_DBUS:
			EXGPIO_Write(EN_DBUS_ADDR, Cpld_Data[bNo]);
			break;
		case ADDR_CS_DBUS:
			EXGPIO_Write(CS_DBUS_ADDR, Cpld_Data[bNo]);
			break;
		case ADDR_DATA_DBUS:
			EXGPIO_Write(DATA_DBUS_ADDR, Cpld_Data[bNo]);
			break;
		case ADDR_PWR_DBUS:
			EXGPIO_Write(PWR_CTLBUS_ADDR, Cpld_Data[bNo]);
			break;
		case ADDR_IO_DBUS:
			EXGPIO_Write(IO_CTLBUS_ADDR, Cpld_Data[bNo]);
			break;
		default:
			break;
	}
}
