#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include "range.h"

typedef struct analog_sensor_s
{
    uint8_t pin;
    uint16_t raw_value;
    range_s input_range;
    range_s output_range;
    float value;
    const char *data_reference_path;
} analog_sensor_s;

void analog_sensor_initialize(analog_sensor_s *analog_sensor, uint8_t pin, uint8_t use_calibration);
void analog_sensor_set_data_reference_path(analog_sensor_s *analog_sensor, const char *data_reference_path);
void analog_sensor_set_required_output_range(analog_sensor_s *analog_sensor, range_s output_range);
void analog_sensor_read(analog_sensor_s *analog_sensor);
void analog_sensor_calibrate(analog_sensor_s *analog_sensor);
void analog_sensor_send_update(analog_sensor_s *analog_sensor);

#endif // ANALOG_SENSOR_H