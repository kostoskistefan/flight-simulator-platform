#ifndef ENGINE_CONTROLS_H
#define ENGINE_CONTROLS_H

#include "analog_sensor.h"

typedef struct engine_controls_s
{
    analog_sensor_s mixture;
    analog_sensor_s throttle;
} engine_controls_s;

void engine_controls_initialize(engine_controls_s *engine_controls, uint8_t use_calibration);
void engine_controls_run(engine_controls_s *engine_controls);
void engine_controls_calibrate_mixture(engine_controls_s *engine_controls);
void engine_controls_calibrate_throttle(engine_controls_s *engine_controls);

#endif // ENGINE_CONTROLS_H