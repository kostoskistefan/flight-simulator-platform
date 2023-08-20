#ifndef YOKE_H
#define YOKE_H

#include "analog_sensor.h"

typedef struct yoke_s
{
    analog_sensor_s roll;
    analog_sensor_s pitch;
} yoke_s;

void yoke_initialize(yoke_s *yoke, uint8_t use_calibration);
void yoke_read(yoke_s *yoke);
void yoke_calibrate(yoke_s *yoke);
void yoke_send_update(yoke_s *yoke);

#endif // YOKE_H