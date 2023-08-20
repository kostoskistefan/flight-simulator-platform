#include "common.h"
#include "tft.h"
#include "yoke.h"
#include "engine_controls.h"
#include "x_plane_interface.h"
#include "flight_instruments.h"

yoke_s yoke;
engine_controls_s engine_controls;
rref_reply_packet_s received_packet;

unsigned long update_timer = 0;

void subscribe_to_data_references(void)
{
    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/airspeed_kts_pilot",
        DATA_REFERENCE_REPLY_INDEX_AIRSPEED,
        2);

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/altitude_ft_pilot",
        DATA_REFERENCE_REPLY_INDEX_ALTITUDE,
        2);

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/roll_vacuum_deg_pilot",
        DATA_REFERENCE_REPLY_INDEX_ATTITUDE_ROLL,
        2);

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/pitch_vacuum_deg_pilot",
        DATA_REFERENCE_REPLY_INDEX_ATTITUDE_PITCH,
        2);

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/heading_electric_deg_mag_pilot",
        DATA_REFERENCE_REPLY_INDEX_HEADING,
        2);

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/vvi_fpm_pilot",
        DATA_REFERENCE_REPLY_INDEX_VERTICAL_SPEED,
        2);
}

void handle_received_x_plane_data(void)
{
    received_packet = x_plane_interface_poll_for_packet();

    if (received_packet.index == -1)
        return;

    switch (received_packet.index)
    {
        case DATA_REFERENCE_REPLY_INDEX_AIRSPEED:
            airspeed_indicator_set_value(received_packet.value);
            break;

        case DATA_REFERENCE_REPLY_INDEX_ALTITUDE:
            altimeter_set_value(received_packet.value);
            break;

        case DATA_REFERENCE_REPLY_INDEX_ATTITUDE_ROLL:
            attitude_indicator_set_roll_value(received_packet.value);
            break;

        case DATA_REFERENCE_REPLY_INDEX_ATTITUDE_PITCH:
            attitude_indicator_set_pitch_value(received_packet.value);
            break;

        case DATA_REFERENCE_REPLY_INDEX_HEADING:
            heading_indicator_set_value(received_packet.value);
            break;

        case DATA_REFERENCE_REPLY_INDEX_VERTICAL_SPEED:
            vertical_speed_indicator_set_value(received_packet.value);
            break;

        default:
            break;
    }
}

void flight_instruments_initialize(void)
{
    tft_initialize();

    airspeed_indicator();
    altimeter();
    attitude_indicator();
    heading_indicator();
    vertical_speed_indicator();

    airspeed_indicator_set_value(0);
    altimeter_set_value(0);
    attitude_indicator_set_roll_value(0);
    attitude_indicator_set_pitch_value(0);
    heading_indicator_set_value(0);
    vertical_speed_indicator_set_value(0);

    tft_timer_handler();
}

void setup(void)
{
    analogSetWidth(10);
    analogReadResolution(10);

    pinMode(BOOT_INTO_CALIBRATION_MODE_PIN, INPUT_PULLDOWN);

    x_plane_interface_initialize();
    subscribe_to_data_references();

    uint8_t use_calibration = 0;

    yoke_initialize(&yoke, use_calibration);
    engine_controls_initialize(&engine_controls, use_calibration);

    flight_instruments_initialize();
}

void loop(void)
{
    tft_timer_handler();

    handle_received_x_plane_data();

    if (millis() - update_timer >= UPDATE_SEND_PERIOD)
    {
        yoke_read(&yoke);
        engine_controls_read(&engine_controls);

        yoke_send_update(&yoke);
        engine_controls_send_update(&engine_controls);

        update_timer = millis();
    }
}