#include "yoke.h"
#include "common.h"

void yoke_initialize(yoke_s *yoke, uint8_t use_calibration)
{
    analog_sensor_initialize(&yoke->roll, YOKE_ROLL_SENSOR_PIN, use_calibration);
    analog_sensor_set_data_reference_path(&yoke->roll, "sim/cockpit2/controls/yoke_roll_ratio");
    analog_sensor_set_required_output_range(&yoke->roll, (range_s) {.minimum = -1, .maximum = 1});

    analog_sensor_initialize(&yoke->pitch, YOKE_PITCH_SENSOR_PIN, use_calibration);
    analog_sensor_set_data_reference_path(&yoke->pitch, "sim/cockpit2/controls/yoke_pitch_ratio");
    analog_sensor_set_required_output_range(&yoke->pitch, (range_s) {.minimum = -1, .maximum = 1});
}

void yoke_read(yoke_s *yoke)
{
    analog_sensor_read(&yoke->roll);
    analog_sensor_read(&yoke->pitch);
}

void yoke_calibrate(yoke_s *yoke)
{
    analog_sensor_calibrate(&yoke->roll);
    analog_sensor_calibrate(&yoke->pitch);
}

void yoke_send_update(yoke_s *yoke)
{
    analog_sensor_send_update(&yoke->roll);
    analog_sensor_send_update(&yoke->pitch);
}
