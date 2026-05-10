#ifndef APP_GESTURE_H_
#define APP_GESTURE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>

typedef enum
{
  GESTURE_NONE = 0,
  GESTURE_UP,
  GESTURE_DOWN,
  GESTURE_LEFT,
  GESTURE_RIGHT
} Gesture_t;

void Gesture_Init(void);
Gesture_t Gesture_Update(int16_t ax, int16_t ay, int16_t az,
                         int16_t gx, int16_t gy, int16_t gz);
const char *Gesture_ToString(Gesture_t gesture);

#ifdef __cplusplus
}
#endif

#endif /* APP_GESTURE_H_ */
