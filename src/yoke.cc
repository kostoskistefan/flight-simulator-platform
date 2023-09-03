#include "yoke.h"
#include "common.h"

void yoke_initialize(yoke_s *yoke, uint8_t use_calibration)
{
    analog_sensor_initialize(&yoke->roll, YOKE_ROLL_SENSOR_PIN, use_calibration);
    analog_sensor_set_data_reference_path(&yoke->roll, "sim/cockpit2/controls/yoke_roll_ratio");
    analog_sensor_set_required_output_range(&yoke->roll, (range_s) { .minimum = -1, .maximum = 1 });

    analog_sensor_initialize(&yoke->pitch, YOKE_PITCH_SENSOR_PIN, use_calibration);
    analog_sensor_set_data_reference_path(&yoke->pitch, "sim/cockpit2/controls/yoke_pitch_ratio");
    analog_sensor_set_required_output_range(&yoke->pitch, (range_s) { .minimum = -1, .maximum = 1 });
}

void yoke_run(yoke_s *yoke)
{
    static uint32_t update_timer = 0;

    if (millis() - update_timer < UPDATE_SEND_PERIOD)
        return;

    analog_sensor_read(&yoke->roll);
    analog_sensor_read(&yoke->pitch);
    analog_sensor_send_update(&yoke->roll);
    analog_sensor_send_update(&yoke->pitch);

    update_timer = millis();
}

void yoke_calibrate_roll(yoke_s *yoke)
{
    analog_sensor_calibrate(&yoke->roll);
}

void yoke_calibrate_pitch(yoke_s *yoke)
{
    analog_sensor_calibrate(&yoke->pitch);
}