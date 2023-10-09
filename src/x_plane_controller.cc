#include "x_plane_controller.h"
#include "tft.h"
#include "common.h"
#include "x_plane_interface.h"
#include "flight_instruments.h"

void x_plane_controller_calibrate(x_plane_controller_s *x_plane_controller);

void x_plane_controller_initialize(x_plane_controller_s *x_plane_controller)
{
    analogReadResolution(10);

    button_initialize(
        &x_plane_controller->button_calibration_mode, 
        COM_NAV_RADIO_COM_FREQUENCY_TRANSFER_BUTTON_PIN
    );

    uint8_t boot_into_calibration_mode = !digitalRead(x_plane_controller->button_calibration_mode.pin);
    
    tft_initialize();
    tft_wait_for_connection();

    x_plane_interface_initialize();
    
    uint8_t packet_array_length = sizeof(x_plane_controller->received_packets) / sizeof(x_plane_controller->received_packets[0]);

    for (uint8_t i = 0; i < packet_array_length; ++i)
    {
        x_plane_controller->received_packets[i].index = -1;
        x_plane_controller->received_packets[i].value = -1;
    }

    tft_clear_screen();

    yoke_initialize(&x_plane_controller->yoke, boot_into_calibration_mode);
    com_nav_radio_initialize(&x_plane_controller->com_nav_radio);
    elevator_trim_initialize(&x_plane_controller->elevator_trim);
    engine_controls_initialize(&x_plane_controller->engine_controls, boot_into_calibration_mode);

    if (boot_into_calibration_mode)
        x_plane_controller_calibrate(x_plane_controller);

    flight_instruments_initialize(&x_plane_controller->flight_instruments);
}

void x_plane_controller_run(x_plane_controller_s *x_plane_controller)
{
    x_plane_interface_poll_for_packet(x_plane_controller->received_packets);

    yoke_run(&x_plane_controller->yoke);
    com_nav_radio_run(&x_plane_controller->com_nav_radio, x_plane_controller->received_packets);
    elevator_trim_run(&x_plane_controller->elevator_trim);
    engine_controls_run(&x_plane_controller->engine_controls);
    flight_instruments_run(&x_plane_controller->flight_instruments, x_plane_controller->received_packets);
} 

void x_plane_controller_calibrate(x_plane_controller_s *x_plane_controller)
{
    typedef enum sensor_index_e
    {
        SENSOR_INDEX_YOKE_ROLL,
        SENSOR_INDEX_YOKE_PITCH,
        SENSOR_INDEX_ENGINE_CONTROLS_THROTTLE,
        SENSOR_INDEX_ENGINE_CONTROLS_MIXTURE,
        SENSOR_INDEX_LAST
    } sensor_index_e;

    sensor_index_e sensor_index = SENSOR_INDEX_YOKE_ROLL;

    tft_clear_screen();

    while (1)
    {
        button_read(&x_plane_controller->button_calibration_mode);

        if (button_is_pressed(&x_plane_controller->button_calibration_mode))
        {
            sensor_index = (sensor_index_e) (sensor_index + 1);

            if (sensor_index == SENSOR_INDEX_LAST)
                break;
        }

        switch (sensor_index)
        {
            case SENSOR_INDEX_YOKE_ROLL:
                yoke_calibrate_roll(&x_plane_controller->yoke);
                tft_sensor_calibration(
                    x_plane_controller->yoke.roll.raw_value,
                    x_plane_controller->yoke.roll.input_range.minimum,
                    x_plane_controller->yoke.roll.input_range.maximum,
                    "Yoke Roll"
                );
                break;

            case SENSOR_INDEX_YOKE_PITCH:
                yoke_calibrate_pitch(&x_plane_controller->yoke);
                tft_sensor_calibration(
                    x_plane_controller->yoke.pitch.raw_value,
                    x_plane_controller->yoke.pitch.input_range.minimum,
                    x_plane_controller->yoke.pitch.input_range.maximum,
                    "Yoke Pitch"
                );
                break;

            case SENSOR_INDEX_ENGINE_CONTROLS_THROTTLE:
                engine_controls_calibrate_throttle(&x_plane_controller->engine_controls);
                tft_sensor_calibration(
                    x_plane_controller->engine_controls.throttle.raw_value,
                    x_plane_controller->engine_controls.throttle.input_range.minimum,
                    x_plane_controller->engine_controls.throttle.input_range.maximum,
                    "Engine Controls Throttle"
                );
                break;

            case SENSOR_INDEX_ENGINE_CONTROLS_MIXTURE:
                engine_controls_calibrate_mixture(&x_plane_controller->engine_controls);
                tft_sensor_calibration(
                    x_plane_controller->engine_controls.mixture.raw_value,
                    x_plane_controller->engine_controls.mixture.input_range.minimum,
                    x_plane_controller->engine_controls.mixture.input_range.maximum,
                    "Engine Controls Mixture"
                );
                break;

            default:
                break;
        }

        delay(100);
    }

    tft_clear_screen();
}
