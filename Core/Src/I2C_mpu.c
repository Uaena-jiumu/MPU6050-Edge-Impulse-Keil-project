#include "I2C_mpu.h"

MPU6050_DataTypeDef MPU6050_Data;

/**
 * @brief  MPU6050写单个寄存器
 * @param  hi2c: I2C句柄指针
 * @param  regAddr: 要写的寄存器地址
 * @param  data: 写入的数据
 * @retval 0成功，1失败
 */
uint8_t MPU6050_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t regAddr, uint8_t data)
{
    if(HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100) == HAL_OK)
    {
        return 0;
    }
    return 1;
}

/**
 * @brief  MPU6050读取多个寄存器
 * @param  hi2c: I2C句柄指针
 * @param  regAddr: 起始寄存器地址
 * @param  pData: 接收数据缓存
 * @param  len: 读取字节长度
 * @retval 0成功，1失败
 */
uint8_t MPU6050_ReadReg(I2C_HandleTypeDef *hi2c, uint8_t regAddr, uint8_t *pData, uint16_t len)
{
    if(HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, pData, len, 100) == HAL_OK)
    {
        return 0;
    }
    return 1;
}

/**
 * @brief  MPU6050初始化，唤醒芯片
 * @param  hi2c: I2C句柄指针
 * @retval 0成功，1失败
 */
uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t res;
    // 1. 电源管理，唤醒，选择陀螺仪时钟源
    res = MPU6050_WriteReg(hi2c, MPU6050_PWR_MGMT_1, 0x01);
    HAL_Delay(100);
	
    // 2. 采样率分频 125Hz
    MPU6050_WriteReg(hi2c, 0x19, 0x07);
    HAL_Delay(20);
	
    // 3. 低通滤波，抑制噪声
    MPU6050_WriteReg(hi2c, 0x1A, 0x03);
    HAL_Delay(20);
	
    // 4. 加速度量程 ±2g
    MPU6050_WriteReg(hi2c, 0x1C, 0x00);
    HAL_Delay(20);
	
    // 5. 陀螺仪量程 ±250°/s
    MPU6050_WriteReg(hi2c, 0x1B, 0x00);
    HAL_Delay(20);
	
    return res;
}
/**
 * @brief  读取MPU6050加速度、陀螺仪、温度原始数据
 * @param  hi2c: I2C句柄指针
 * @note   从0x3B连续读取14字节数据
 */
void MPU6050_ReadData(I2C_HandleTypeDef *hi2c)
{
    uint8_t buf[14];
    // 先判断读取是否成功
    if(MPU6050_ReadReg(hi2c, MPU6050_ACCEL_XOUT_H, buf, 14) == 0)
    {
        // 读取成功才拼接数据
        MPU6050_Data.AccX  = (int16_t)((buf[0] << 8) | buf[1]);
        MPU6050_Data.AccY  = (int16_t)((buf[2] << 8) | buf[3]);
        MPU6050_Data.AccZ  = (int16_t)((buf[4] << 8) | buf[5]);
        MPU6050_Data.Temp  = (int16_t)((buf[6] << 8) | buf[7]);
        MPU6050_Data.GyroX = (int16_t)((buf[8] << 8) | buf[9]);
        MPU6050_Data.GyroY = (int16_t)((buf[10]<< 8) | buf[11]);
        MPU6050_Data.GyroZ = (int16_t)((buf[12]<< 8) | buf[13]);
    }
    //读取失败：直接跳过，保留上一次的有效数据
}




