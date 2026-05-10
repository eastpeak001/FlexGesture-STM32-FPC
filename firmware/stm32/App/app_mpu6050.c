#include "app_mpu6050.h"

#define MPU6050_REG_SMPLRT_DIV    0x19U
#define MPU6050_REG_CONFIG        0x1AU
#define MPU6050_REG_GYRO_CONFIG   0x1BU
#define MPU6050_REG_ACCEL_CONFIG  0x1CU
#define MPU6050_REG_ACCEL_XOUT_H  0x3BU
#define MPU6050_REG_PWR_MGMT_1    0x6BU
#define MPU6050_REG_WHO_AM_I      0x75U

static uint8_t mpu6050_who_am_i = 0U;

static HAL_StatusTypeDef MPU6050_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t value)
{
  return HAL_I2C_Mem_Write(hi2c, MPU6050_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT,
                           &value, 1U, MPU6050_I2C_TIMEOUT_MS);
}

static HAL_StatusTypeDef MPU6050_ReadReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *value)
{
  return HAL_I2C_Mem_Read(hi2c, MPU6050_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT,
                          value, 1U, MPU6050_I2C_TIMEOUT_MS);
}

static int16_t MPU6050_ToInt16(uint8_t high, uint8_t low)
{
  return (int16_t)((uint16_t)high << 8 | low);
}

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c)
{
  HAL_StatusTypeDef status;

  status = MPU6050_ReadReg(hi2c, MPU6050_REG_WHO_AM_I, &mpu6050_who_am_i);
  if (status != HAL_OK)
  {
    return status;
  }

  if (mpu6050_who_am_i != MPU6050_WHO_AM_I_VALUE)
  {
    return HAL_ERROR;
  }

  status = MPU6050_WriteReg(hi2c, MPU6050_REG_PWR_MGMT_1, 0x00U);
  if (status != HAL_OK)
  {
    return status;
  }

  HAL_Delay(100U);

  status = MPU6050_WriteReg(hi2c, MPU6050_REG_SMPLRT_DIV, 0x07U);
  if (status != HAL_OK)
  {
    return status;
  }

  status = MPU6050_WriteReg(hi2c, MPU6050_REG_CONFIG, 0x03U);
  if (status != HAL_OK)
  {
    return status;
  }

  status = MPU6050_WriteReg(hi2c, MPU6050_REG_GYRO_CONFIG, 0x00U);
  if (status != HAL_OK)
  {
    return status;
  }

  return MPU6050_WriteReg(hi2c, MPU6050_REG_ACCEL_CONFIG, 0x00U);
}

HAL_StatusTypeDef MPU6050_ReadRaw(I2C_HandleTypeDef *hi2c, MPU6050_RawData_t *data)
{
  uint8_t buffer[14];
  HAL_StatusTypeDef status;

  if (data == NULL)
  {
    return HAL_ERROR;
  }

  status = HAL_I2C_Mem_Read(hi2c, MPU6050_I2C_ADDR, MPU6050_REG_ACCEL_XOUT_H,
                            I2C_MEMADD_SIZE_8BIT, buffer, sizeof(buffer),
                            MPU6050_I2C_TIMEOUT_MS);
  if (status != HAL_OK)
  {
    return status;
  }

  data->ax = MPU6050_ToInt16(buffer[0], buffer[1]);
  data->ay = MPU6050_ToInt16(buffer[2], buffer[3]);
  data->az = MPU6050_ToInt16(buffer[4], buffer[5]);
  data->gx = MPU6050_ToInt16(buffer[8], buffer[9]);
  data->gy = MPU6050_ToInt16(buffer[10], buffer[11]);
  data->gz = MPU6050_ToInt16(buffer[12], buffer[13]);

  return HAL_OK;
}

uint8_t MPU6050_GetWhoAmI(void)
{
  return mpu6050_who_am_i;
}
