#include "OLED_hal.h"
#include "OLED_hal_font.h"
#include <math.h>

static void I2C_Delay(void)
{
    uint32_t t;
    for(t = 0; t < 20; t++);
}

static void I2C_Start(void)
{
    HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, GPIO_PIN_RESET);
    I2C_Delay();
    HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_RESET);
    I2C_Delay();
}

static void I2C_Stop(void)
{
    HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, GPIO_PIN_SET);
    I2C_Delay();
}

static void I2C_SendByte(uint8_t dat)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, (dat & 0x80U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        dat <<= 1U;
        I2C_Delay();
        HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_SET);
        I2C_Delay();
        HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_RESET);
        I2C_Delay();
    }
    //Ó¦´ð
    HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_RESET);
    I2C_Delay();
}

static void OLED_Write_Cmd(uint8_t cmd)
{
    I2C_Start();
    I2C_SendByte(0x78U);
    I2C_SendByte(0x00U);
    I2C_SendByte(cmd);
    I2C_Stop();
}

static void OLED_Write_Data(uint8_t data)
{
    I2C_Start();
    I2C_SendByte(0x78U);
    I2C_SendByte(0x40U);
    I2C_SendByte(data);
    I2C_Stop();
}

void OLED_Init(void)
{
    OLED_Write_Cmd(0xAE);
    OLED_Write_Cmd(0xD5);
    OLED_Write_Cmd(0x80);
    OLED_Write_Cmd(0xA8);
    OLED_Write_Cmd(0x3F);
    OLED_Write_Cmd(0xD3);
    OLED_Write_Cmd(0x00);
    OLED_Write_Cmd(0x40);
    OLED_Write_Cmd(0x8D);
    OLED_Write_Cmd(0x14);
    OLED_Write_Cmd(0x20);
    OLED_Write_Cmd(0x00);
    OLED_Write_Cmd(0xA1);
    OLED_Write_Cmd(0xC8);
    OLED_Write_Cmd(0xDA);
    OLED_Write_Cmd(0x12);
    OLED_Write_Cmd(0x81);
    OLED_Write_Cmd(0xCF);
    OLED_Write_Cmd(0xD9);
    OLED_Write_Cmd(0xF1);
    OLED_Write_Cmd(0xDB);
    OLED_Write_Cmd(0x30);
    OLED_Write_Cmd(0xA4);
    OLED_Write_Cmd(0xA6);
    OLED_Write_Cmd(0xAF);

    OLED_Clear();
}

void OLED_Clear(void)
{
    uint8_t page, i;
    for(page = 0U; page < 8U; page++)
    {
        OLED_Write_Cmd(0xB0U + page);
        OLED_Write_Cmd(0x00U);
        OLED_Write_Cmd(0x10U);
        for(i = 0U; i < 128U; i++)
        {
            OLED_Write_Data(0x00U);
        }
    }
}

static void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr)
{
    uint8_t c = 0, i = 0;
    c = chr - ' ';
    if(c > 95) return;
    OLED_Write_Cmd(0xB0 + y);
    OLED_Write_Cmd(((x & 0xF0) >> 4) | 0x10);
    OLED_Write_Cmd(x & 0x0F);
    for(i = 0; i < 16; i++)
    {
        OLED_Write_Data(F8X16[c][i]);
    }
}

void OLED_ShowString(uint8_t x, uint8_t y, char *str)
{
    while(*str != '\0')
    {
        OLED_ShowChar(x, y, (uint8_t)*str);
        x += 8;
        if(x > 120)
        {
            x = 0;
            y++;
        }
        str++;
    }
}

void OLED_ShowNum(uint8_t x, uint8_t y, int32_t num, uint8_t len)
{
    uint8_t i;
    uint32_t abs_num;
    uint8_t offset = 0;

    if(num < 0)
    {
        OLED_ShowChar(x, y, '-');
        offset = 1;
        if(num == (-2147483647 - 1))
        {
            abs_num = 0x80000000U;
        }
        else
        {
            abs_num = (uint32_t)(-num);
        }
    }
    else
    {
        abs_num = (uint32_t)num;
        offset = 0;
    }

    uint8_t num_len = len - offset;
    if(num_len == 0) return;

    uint32_t div = 1;
    for(i = 0; i < num_len - 1; i++)
    {
        div *= 10;
    }

    uint8_t enshow = 0;
    for(i = 0; i < num_len; i++)
    {
        uint8_t d = abs_num / div % 10;
        div /= 10;
        if(enshow == 0)
        {
            if(d == 0 && (i != num_len - 1))
            {
                OLED_ShowChar(x + (offset + i) * 8, y, ' ');
                continue;
            }
            enshow = 1;
        }
        OLED_ShowChar(x + (offset + i) * 8, y, d + '0');
    }
}