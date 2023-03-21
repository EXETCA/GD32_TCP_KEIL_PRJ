#include "main.h"
#include "TM1640.h"
void Init_TM1640Gpio(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
	  rcu_periph_clock_enable(RCU_GPIOE);
	  rcu_periph_clock_enable(RCU_GPIOF);
    /*PE11 -> PF11 */
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	  gpio_af_set(GPIOF,GPIO_AF_15,GPIO_PIN_11);
    gpio_bit_reset(GPIOF, GPIO_PIN_11);		
    /*PB2 -> PE3 */
	  gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);
	  gpio_af_set(GPIOB,GPIO_AF_15,GPIO_PIN_0 | GPIO_PIN_1);
    gpio_bit_reset(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);		

	  gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_3);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
	  gpio_af_set(GPIOE,GPIO_AF_15,GPIO_PIN_3);
    gpio_bit_reset(GPIOE, GPIO_PIN_3);		
}

void Init_TM1640(void)
{
    uint16_t itemp;
    // 第一片1640初始化

    TM1640_START(TM1640_1);
    Set_TM1640(TM1640_1, Add_address);
    TM1640_END(TM1640_1);
    TM1640_START(TM1640_1);
    Set_TM1640(TM1640_1, GRID_1);
    for (itemp = 0; itemp < 16; itemp++)
    {
        Set_TM1640(TM1640_1, 0);
    }
    TM1640_END(TM1640_1);
    TM1640_START(TM1640_1);
    Set_TM1640(TM1640_1, Light_off);
    TM1640_END(TM1640_1);

    // 第2片1640初始化
    TM1640_START(TM1640_2);
    Set_TM1640(TM1640_2, Add_address);
    TM1640_END(TM1640_2);

    TM1640_START(TM1640_2);
    Set_TM1640(TM1640_2, GRID_1);
    for (itemp = 0; itemp < 16; itemp++)
    {
        Set_TM1640(TM1640_2, 0);
    }
    TM1640_END(TM1640_2);
    TM1640_START(TM1640_2);
    Set_TM1640(TM1640_2, Light_off);
    TM1640_END(TM1640_2);
}

void BatchLightControl(uint8_t TM1640_num, uint8_t *color, uint8_t light)
{
    uint8_t itemp;
    TM1640_START(TM1640_num);
    Set_TM1640(TM1640_num, Add_address);
    TM1640_END(TM1640_num);
    TM1640_START(TM1640_num);
    Set_TM1640(TM1640_num, GRID_1);
    for (itemp = 0; itemp < 12; itemp++)
    {
        Set_TM1640(TM1640_num, color[itemp]);
    }
    TM1640_END(TM1640_num);
    TM1640_START(TM1640_num);
    Set_TM1640(TM1640_num, light);
    TM1640_END(TM1640_num);
}

void SingleLightControl(uint8_t TM1640_num, uint8_t led_num, uint8_t color, uint8_t light)
{
    uint8_t SelLED_num = 0;

    switch (led_num)
    {
    case 1:
        SelLED_num = GRID_1;
        break;
    case 2:
        SelLED_num = GRID_2;
        break;
    case 3:
        SelLED_num = GRID_3;
        break;
    case 4:
        SelLED_num = GRID_4;
        break;
    case 5:
        SelLED_num = GRID_5;
        break;
    case 6:
        SelLED_num = GRID_6;
        break;
    case 7:
        SelLED_num = GRID_7;
        break;
    case 8:
        SelLED_num = GRID_8;
        break;
    case 9:
        SelLED_num = GRID_9;
        break;
    case 10:
        SelLED_num = GRID_10;
        break;
    case 11:
        SelLED_num = GRID_11;
        break;
    case 12:
        SelLED_num = GRID_12;
        break;
    case 13:
        SelLED_num = GRID_13;
        break;
    case 14:
        SelLED_num = GRID_14;
        break;
    case 15:
        SelLED_num = GRID_15;
        break;
    case 16:
        SelLED_num = GRID_16;
        break;
    default:
        break;
        // 需要后4个控制可以直接添加case
    }

    TM1640_START(TM1640_num);
    Set_TM1640(TM1640_num, Fixed_address);
    TM1640_END(TM1640_num);
    TM1640_START(TM1640_num);
    Set_TM1640(TM1640_num, SelLED_num);
    Set_TM1640(TM1640_num, color);
    TM1640_END(TM1640_num);
    TM1640_START(TM1640_num);
    Set_TM1640(TM1640_num, light);
    TM1640_END(TM1640_num);
}

void Set_TM1640(uint8_t num, uint8_t data)
{
    uint16_t DATA, itemp, i;

    DATA = data;
    if (1 == num)
    {
        for (itemp = 0; itemp < 8; itemp++)
        {
            LedC1_CLK_L;
            i = 100;
            while (i--)
                ;
            if ((DATA & 0x01))
            {
                LedC1_DIN_H;
            }
            else
            {
                LedC1_DIN_L;
            }
            i = 100;
            while (i--)
                ;
            DATA = DATA >> 1;
            LedC1_CLK_H;
        }
        i = 100;
        while (i--)
            ;
        LedC1_CLK_L;
        LedC1_DIN_L;
    }
    else if (2 == num)
    {
        for (itemp = 0; itemp < 8; itemp++)
        {
            LedC2_CLK_L;
            i = 100;
            while (i--)
                ;
            if ((DATA & 0x01))
            {
                LedC2_DIN_H;
            }
            else
            {
                LedC2_DIN_L;
            }
            i = 100;
            while (i--)
                ;
            DATA = DATA >> 1;
            LedC2_CLK_H;
        }
        i = 100;
        while (i--)
            ;
        LedC2_CLK_L;
        LedC2_DIN_L;
    }
}

void TM1640_END(uint8_t flag)
{
    uint16_t i;
    if (TM1640_1 == flag)
    {
        LedC1_CLK_H;
        LedC1_DIN_L;
        i = 100;
        while (i--)
            ;
        LedC1_DIN_H;
    }
    else if (TM1640_2 == flag)
    {
        LedC2_CLK_H;
        LedC2_DIN_L;
        i = 100;
        while (i--)
            ;
        LedC2_DIN_H;
    }
}

void TM1640_START(uint8_t flag)
{
    uint16_t i;
    if (TM1640_1 == flag)
    {
        LedC1_CLK_H;
        LedC1_DIN_H;
        i = 100;
        while (i--)
            ;
        LedC1_DIN_L;
    }
    else if (TM1640_2 == flag)
    {
        LedC2_CLK_H;
        LedC2_DIN_H;
        i = 100;
        while (i--)
            ;
        LedC2_DIN_L;
    }
}
