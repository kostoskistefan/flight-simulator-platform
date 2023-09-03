#include "analog_sensor.h"
#include "common.h"
#include "value_remapper.h"
#include "x_plane_interface.h"

void analog_sensor_initialize(analog_sensor_s *analog_sensor, uint8_t pin, uint8_t use_calibration)
{
    analog_sensor->pin = pin;
    analog_sensor->value = 0;
    analog_sensor->data_reference_path = NULL;

    analog_sensor->input_range =
        (range_s) {
            .minimum = 1023 * use_calibration,
            .maximum = 1023 * (1 - use_calibration)
        };

    analog_sensor->output_range =
        (range_s) {
            .minimum = 0,
            .maximum = 1
        };

    pinMode(analog_sensor->pin, INPUT);
}

void analog_sensor_set_data_reference_path(analog_sensor_s *analog_sensor, const char *data_reference_path)
{
    analog_sensor->data_reference_path = data_reference_path;
}

void analog_sensor_set_required_output_range(analog_sensor_s *analog_sensor, range_s output_range)
{
    analog_sensor->output_range = output_range;
}

void analog_sensor_read(analog_sensor_s *analog_sensor)
{
    analog_sensor->value = value_remapper_remap(
        analogRead(analog_sensor->pin),
        analog_sensor->input_range.minimum,
        analog_sensor->input_range.maximum,
        analog_sensor->output_range.minimum,
        analog_sensor->output_range.maximum
    );
}

void analog_sensor_calibrate(analog_sensor_s *analog_sensor)
{
    uint16_t current_sensor_value = analogRead(analog_sensor->pin);

    if (analog_sensor->input_range.minimum > current_sensor_value)
        analog_sensor->input_range.minimum = current_sensor_value;

    if (analog_sensor->input_range.maximum < current_sensor_value)
        analog_sensor->input_range.maximum = current_sensor_value;
}

void analog_sensor_send_update(analog_sensor_s *analog_sensor)
{
    x_plane_interface_send_data_reference_value(
        analog_sensor->data_reference_path,
        analog_sensor->value
    );
}
