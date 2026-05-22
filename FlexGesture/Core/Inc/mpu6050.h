#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define MPU6050_I2C_ADDR        (0x68U << 1)
#define MPU6050_I2C_TIMEOUT_MS  100U

typedef struct
{
  int16_t ax;
  int16_t ay;
  int16_t az;
  int16_t gx;
  int16_t gy;
  int16_t gz;
} MPU6050_RawData_t;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_ReadRaw(I2C_HandleTypeDef *hi2c, MPU6050_RawData_t *data);

#ifdef __cplusplus
}
#endif

#endif /* INC_MPU6050_H_ */
