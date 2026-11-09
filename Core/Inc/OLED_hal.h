#ifndef __OLED_H
#define __OLED_H 

#include "stdint.h"
#include "stdlib.h"	
#include <math.h>
#include "stm32f1xx_hal.h"

// Èí¼þI2CÒý½Å PB8 SCL  PB9 SDA
#define OLED_SCL_PORT  GPIOB
#define OLED_SCL_PIN   GPIO_PIN_8
#define OLED_SDA_PORT  GPIOB
#define OLED_SDA_PIN   GPIO_PIN_9

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowString(uint8_t x, uint8_t y, char *str);
void OLED_ShowNum(uint8_t x, uint8_t y, int32_t num, uint8_t len);

#endif

