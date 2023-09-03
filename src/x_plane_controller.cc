#include "x_plane_controller.h"
#include "tft.h"
#include "common.h"
#include "x_plane_interface.h"
#include "flight_instruments.h"

void x_plane_controller_calibrate(x_plane_controller_s *x_plane_controller);

void x_plane_controller_initialize(x_plane_controller_s *x_plane_controller)
{
    analogSetWidth(10);
    analogReadResolution(10);

    pinMode(BOOT_INTO_CALIBRATION_MODE_PIN, INPUT_PULLDOWN);
    uint8_t use_calibration = digitalRead(BOOT_INTO_CALIBRATION_MODE_PIN);

    x_plane_interface_initialize();

    yoke_initialize(&x_plane_controller->yoke, use_calibration);
    com_nav_radio_initialize(&x_plane_controller->com_nav_radio);
    elevator_trim_initialize(&x_plane_controller->elevator_trim);
    engine_controls_initialize(&x_plane_controller->engine_controls, use_calibration);
    flight_instruments_initialize(&x_plane_controller->flight_instruments);

    if (use_calibration)
        x_plane_controller_calibrate(x_plane_controller);
}

void x_plane_controller_run(x_plane_controller_s *x_plane_controller)
{
    yoke_run(&x_plane_controller->yoke);
    com_nav_radio_run(&x_plane_controller->com_nav_radio);
    elevator_trim_run(&x_plane_controller->elevator_trim);
    engine_controls_run(&x_plane_controller->engine_controls);
    flight_instruments_run(&x_plane_controller->flight_instruments);
} 

void x_plane_controller_calibrate(x_plane_controller_s *x_plane_controller)
{
    typedef enum sensor_index_e
    {
        SENSOR_INDEX_YOKE_ROLL,
        SENSOR_INDEX_YOKE_PITCH,
        SENSOR_INDEX_ENGINE_CONTROLS_MIXTURE,
        SENSOR_INDEX_ENGINE_CONTROLS_THROTTLE,
        SENSOR_INDEX_LAST
    } sensor_index_e;

    uint32_t sensor_calibration_timer = 0;
    sensor_index_e sensor_index = SENSOR_INDEX_YOKE_ROLL;

    while (1)
    {
        if (millis() - sensor_calibration_timer >= CALIBRATION_PERIOD_FOR_ONE_SENSOR)
        {
            sensor_index = (sensor_index_e) (sensor_index + 1);

            if (sensor_index == SENSOR_INDEX_LAST)
                break;

            sensor_calibration_timer = millis();
        }

        switch (sensor_index)
        {
            case SENSOR_INDEX_YOKE_ROLL:
                yoke_calibrate_roll(&x_plane_controller->yoke);
                tft_sensor_calibration(
                    x_plane_controller->yoke.roll.value,
                    x_plane_controller->yoke.roll.input_range.minimum,
                    x_plane_controller->yoke.roll.input_range.maximum,
                    "Yoke Roll"
                );
                break;

            case SENSOR_INDEX_YOKE_PITCH:
                yoke_calibrate_pitch(&x_plane_controller->yoke);
                tft_sensor_calibration(
                    x_plane_controller->yoke.pitch.value,
                    x_plane_controller->yoke.pitch.input_range.minimum,
                    x_plane_controller->yoke.pitch.input_range.maximum,
                    "Yoke Pitch"
                );
                break;

            case SENSOR_INDEX_ENGINE_CONTROLS_MIXTURE:
                engine_controls_calibrate_mixture(&x_plane_controller->engine_controls);
                tft_sensor_calibration(
                    x_plane_controller->engine_controls.mixture.value,
                    x_plane_controller->engine_controls.mixture.input_range.minimum,
                    x_plane_controller->engine_controls.mixture.input_range.maximum,
                    "Engine Controls Mixture"
                );
                break;

            case SENSOR_INDEX_ENGINE_CONTROLS_THROTTLE:
                engine_controls_calibrate_throttle(&x_plane_controller->engine_controls);
                tft_sensor_calibration(
                    x_plane_controller->engine_controls.throttle.value,
                    x_plane_controller->engine_controls.throttle.input_range.minimum,
                    x_plane_controller->engine_controls.throttle.input_range.maximum,
                    "Engine Controls Throttle"
                );
                break;
        }
    }
}