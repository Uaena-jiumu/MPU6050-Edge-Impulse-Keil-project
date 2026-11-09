#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f1xx_hal.h"


/* MPU6050 基础定义 */
#define MPU6050_ADDR        0xD0    // AD0拉低，器件地址
#define MPU6050_PWR_MGMT_1  0x6B    // 电源管理寄存器
#define MPU6050_ACCEL_XOUT_H 0x3B   // 加速度起始寄存器

/* 存储MPU6050数据结构体 */
typedef struct
{
    int16_t AccX;
    int16_t AccY;
    int16_t AccZ;
    int16_t GyroX;
    int16_t GyroY;
    int16_t GyroZ;
    int16_t Temp;
}MPU6050_DataTypeDef;

extern MPU6050_DataTypeDef MPU6050_Data;

/* 函数声明 */
uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c);
uint8_t MPU6050_ReadReg(I2C_HandleTypeDef *hi2c, uint8_t regAddr, uint8_t *pData, uint16_t len);
uint8_t MPU6050_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t regAddr, uint8_t data);
void MPU6050_ReadData(I2C_HandleTypeDef *hi2c);

#endif