# MPU6050 + FreeRTOS 采集版（acquisition-only）

本分支是**可在 STM32F103C8（64 KB Flash / 20 KB RAM）直接运行**的采集版本，去掉了 Edge Impulse 推理部分。

完整 AI 版本在 `main` 分支：包含 EI SDK、EON 模型与推理代码，链接后约 88 KB，需要 128 KB 及以上 Flash 的平台或 ESP32。

## 功能

- TIM3 定时中断以 **100 Hz** 读取 MPU6050 三轴加速度
- 中断中通过 `osMessageQueuePut` 入队，`AcquireTask` 用 `osMessageQueueGet` 消费
- 每攒满 **200 个点**（约 2 秒），通过 USART1 输出一段 CSV：
  ```
  ---WINDOW_START---
  x,y,z
  ...（200 行）
  ---WINDOW_END---
  ```
- OLED 显示当前三轴加速度

## 硬件连接

| 外设 | 引脚 | 说明 |
|---|---|---|
| MPU6050 | PB10 / PB11 | I2C2，100 kHz |
| OLED | PB8 / PB9 | 软件 I2C（SCL / SDA） |
| USART1 | PA9 / PA10 | 115200-8-N-1 |

## 编译

- Keil MDK 5.39 + ARM Compiler 6.21
- 打开 `MDK-ARM/OLED.uvprojx`，直接 Build
- 实测资源占用：**Code 21,012 B + RO 496 B，RAM 15,280 B**，64 KB Flash 可正常烧录

## 关键点

本工程使用 **TIM2 作为 HAL 时基**。`main.c` 中的 `HAL_TIM_PeriodElapsedCallback` 必须为 TIM2 调用 `HAL_IncTick()`，否则 `HAL_GetTick()` 恒为 0，`HAL_Delay()` 与所有 HAL 超时会永久卡死（表现为卡在时钟初始化或 `HAL_Delay`）。

## 目录

```
Core/          应用与 HAL 初始化
Middlewares/   FreeRTOS 内核与 CMSIS-RTOS2
Drivers/       STM32F1 HAL 与 CMSIS
MDK-ARM/       Keil 工程
```

## 切回完整版

```bash
git checkout main
```
