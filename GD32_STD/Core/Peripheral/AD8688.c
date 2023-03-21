
#include "AD8688.h"
#include "Myinclude.h"
volatile uint16_t watch;
const unsigned int ADS8688_CH_TABLE[8] = {0xc000, 0xc400, 0xc800, 0xcc00, 0xd000, 0xd400, 0xd800, 0xdc00};

void Init_AD8688Gpio(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = GPIO_AF15_EVENTOUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Init_AD8688(void)
{
  AD_SYNC_H;
  AD_SCLK_H;
  AD_SDI_L;
  ADS8688_Reset();
  //第一路-第八路采样范围
  Write_AD8688(0x0B01);
  Write_AD8688(0x0D01);
  Write_AD8688(0x0F01);
  Write_AD8688(0x1101);
  Write_AD8688(0x1301);
  Write_AD8688(0x1501);
  Write_AD8688(0x1701);
  Write_AD8688(0x1901);
}

void ADS8688_Reset(void)
{
  AD_RST_H;
  DELAY_Ms(5);
  AD_RST_L;
  DELAY_Ms(15);
  AD_RST_H;
  DELAY_Ms(50);
}

void Write_AD8688(uint16_t data)
{
  uint16_t i, j;
  AD_SYNC_H;
  DELAY_Ms(1);
  AD_SYNC_L; //片选信号拉低

  for (i = 0; i < 16; i++) //写入16位命令
  {
    if ((data << i) & 0x8000)
    {
      AD_SDI_H; // din=1
    }
    else
    {
      AD_SDI_L; // din=0
    }

    AD_SCLK_H; // CLK=1
    j = 5;
    while (j--)
      ;
    AD_SCLK_L; // CLK =0
    j = 5;
    while (j--)
      ;
  }
  for (i = 0; i < 16; i++) //后16个时钟为读取数据，写入命令是直接抛掉
  {
    AD_SCLK_H; // CLK=1
    j = 5;
    while (j--)
      ;
    AD_SCLK_L; // CLK =0
    j = 5;
    while (j--)
      ;
  }

  AD_SYNC_H; //片选信号拉高
  AD_SCLK_H;
}

uint16_t Read_AD8688(void)
{
  uint16_t i, Result = 0, j;
  AD_SYNC_H;
  DELAY_Ms(2);
  AD_SYNC_L; //片选信号拉低

  for (i = 0; i < 16; i++) //前16个时钟为写入命令，读取时直接抛掉
  {
    AD_SDI_L;  //确保读取时，写入命令为0；
    AD_SCLK_H; // CLK=1
    j = 5;
    while (j--)
      ;
    AD_SCLK_L; // CLK =0
    j = 5;
    while (j--)
      ;
  }
  for (i = 0; i < 16; i++) //读取十六位数据
  {
    AD_SCLK_H; // CLK =1
    j = 5;
    while (j--)
      ;
    Result <<= 1;
    if (AD_SDO == 1)
    {
      Result++;
    }
    AD_SCLK_L; // CLK =0
    j = 5;
    while (j--)
      ;
  }

  AD_SYNC_H; //片选信号拉高
  AD_SCLK_H;
  return Result;
}

uint16_t ADS8688_READ_Register(uint16_t data)
{
  uint16_t i, j, Result = 0;

  AD_SYNC_H;
  DELAY_Ms(2);
  AD_SYNC_L;               //片选信号拉低
  for (i = 0; i < 16; i++) //写入16位命令
  {
    if ((data << i) & 0x8000)
    {
      AD_SDI_H; // din=1
    }
    else
    {
      AD_SDI_L; // din=0
    }
    AD_SCLK_H; // CLK=1
    j = 5;
    while (j--)
      ;
    AD_SCLK_L; // CLK =0
    j = 5;
    while (j--)
      ;
  }
  for (i = 0; i < 16; i++) //读取十六位数据
  {
    AD_SCLK_H; // CLK =1
    j = 5;
    while (j--)
      ;

    Result <<= 1;
    if (AD_SDO == 1)
    {
      Result++;
    }
    AD_SCLK_L; // CLK =0
    j = 5;
    while (j--)
      ;
  }
  AD_SYNC_H; //片选信号拉高
  AD_SCLK_H;
  return Result;
}

//第二份尝试程序
uint8_t Comu_8688(uint8_t Data, uint8_t Ope)
{
  volatile uint8_t i, Buf;
  if (Ope)
  {
    Buf = Data;
    for (i = 0; i < 8; i++)
    {
      //			Delay_Us(1);
      AD_SCLK_H;
      //			Delay_Us(1);
      if (Buf & 0x80)
        AD_SDI_H;
      else
        AD_SDI_L;
      //			Delay_Us(1);
      AD_SCLK_L;
      Buf <<= 1;
    }
    return 1;
  }
  else
  {
    Buf = 0;
    for (i = 0; i < 8; i++)
    {
      Buf <<= 1;
      if (AD_SDO)
        Buf |= 1;
      //				Delay_Us(1);

      AD_SCLK_H;
      //				Delay_Us(1);
      AD_SCLK_L;
      //				Delay_Us(1);
    }
    return Buf;
  }
}

void Init_Ads8688(void)
{
  uint8_t i, j, Num;
  uint8_t Range_Info[8] = {0X01, 0X01, 0X01, 0X01, 0X01, 0X01, 0X01, 0X01};
  uint16_t itemp;

  for (Num = 0; Num < 5; Num++)
  {
    AD_RST_L;
    itemp = 10000;
    while (itemp--)
      ;
    AD_RST_H;
    itemp = 10000;
    while (itemp--)
      ;
    // Write_Reg_8688(AD_8688_FETURE_SEL,0x2B);
    itemp = 50000;
    while (itemp--)
      ;

    j = 0;
    for (i = AD_8688_CH0_RAGE; i <= AD_8688_CH7_RAGE; i++)
    {
      Write_Reg_8688(i, Range_Info[j]);
      itemp = 500;
      while (itemp--)
        ;
      watch = Read_Reg_8688(i);
      itemp = 500;
      while (itemp--)
        ;
      j++;
    }
    // Write_Reg_8688(AD_8688_CH0_RAGE,0x01);
  }
}
void Write_Reg_8688(uint8_t Add, uint8_t Data)
{
  uint8_t Buf;
  uint16_t i;
  AD_SYNC_L;
  i = 400;
  while (i--)
    ;
  Buf = (Add << 1) + 1;
  Comu_8688(Buf, 1);
  Comu_8688(Data, 1);
  AD_SYNC_H;
}

uint8_t Read_Reg_8688(uint8_t Add)
{
  uint8_t Buf;
  uint16_t i;
  AD_SYNC_L;
  i = 400;
  while (i--)
    ;
  Buf = (Add << 1);
  Comu_8688(Buf, 1);
  Comu_8688(0, 0);
  Buf = Comu_8688(0, 0);
  AD_SYNC_H;
  return Buf;
}

unsigned int Read_8688(unsigned int Samp_Ch)
{
  uint8_t Buf;
  uint16_t Resault, data;
  // Init_Ads8688();
  //	Delay_Us(1);
  AD_SYNC_L;
  //	Delay_Us(1);

  data = (uint16_t)Samp_Ch;
  Buf = (uint8_t)((data >> 8) & 0xff);
  Comu_8688(Buf, 1);
  data = (uint16_t)Samp_Ch;
  Buf = (uint8_t)((data)&0xff);
  // Buf = (uint8_t)Samp_Ch;
  Comu_8688(Buf, 1);
  AD_SYNC_H;
  //	Delay_Us(1);
  AD_SYNC_L;
  //	Delay_Us(1);
  Comu_8688(0X00, 0);
  Comu_8688(0, 0);

  Resault = (Comu_8688(0x00, 0));
  Resault <<= 8;
  Resault |= (Comu_8688(0x00, 0));
  AD_SYNC_H;
  return (uint32_t)Resault;
}
