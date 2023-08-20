#include "engine_controls.h"
#include "common.h"

void engine_controls_initialize(engine_controls_s *engine_controls, uint8_t use_calibration)
{
    
    analog_sensor_initialize(&engine_controls->mixture, ENGINE_CONTROLS_MIXTURE_SENSOR_PIN, use_calibration);
    analog_sensor_set_data_reference_path(&engine_controls->mixture, "sim/flightmodel/engine/ENGN_mixt[0]");
    analog_sensor_set_required_output_range(&engine_controls->mixture, (range_s) {.minimum = 0, .maximum = 1});

    analog_sensor_initialize(&engine_controls->throttle, ENGINE_CONTROLS_THROTTLE_SENSOR_PIN, use_calibration);
    analog_sensor_set_data_reference_path(&engine_controls->throttle, "sim/flightmodel/engine/ENGN_thro[0]");
    analog_sensor_set_required_output_range(&engine_controls->throttle, (range_s) {.minimum = 0, .maximum = 1});
}

void engine_controls_read(engine_controls_s *engine_controls)
{
    analog_sensor_read(&engine_controls->mixture);
    analog_sensor_read(&engine_controls->throttle);
}

void engine_controls_calibrate(engine_controls_s *engine_controls)
{
    analog_sensor_calibrate(&engine_controls->mixture);
    analog_sensor_calibrate(&engine_controls->throttle);
}

void engine_controls_send_update(engine_controls_s *engine_controls)
{
    analog_sensor_send_update(&engine_controls->mixture);
    analog_sensor_send_update(&engine_controls->throttle);
}
