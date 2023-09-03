#ifndef TFT_H
#define TFT_H

#include "common.h"

void tft_initialize(void);
void tft_timer_handler(void);
void tft_sensor_calibration(float value, int32_t minimum, int32_t maximum, const char *sensor_name);

#endif // TFT_H
