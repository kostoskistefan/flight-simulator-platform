#include "com_nav_radio.h"
#include "common.h"

void com_nav_radio_set_frequency_for_display(
    com_nav_radio_s *com_nav_radio,
    uint8_t display_number,
    uint32_t frequency
);

void com_nav_radio_initialize(com_nav_radio_s *com_nav_radio)
{
    com_nav_radio->display.begin(TFT_MOSI, TFT_SCLK, COM_NAV_RADIO_DISPLAY_PIN_CS, 4);

    for (uint8_t i = 0; i < 4; ++i)
    {
        com_nav_radio->display.shutdown(i, 0);
        com_nav_radio->display.setIntensity(i, 4);
        com_nav_radio->display.clearDisplay(i);
    }

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit/radios/com1_freq_hz",
        DATA_REFERENCE_REPLY_INDEX_COM_FREQUENCY,
        1
    );

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit/radios/com1_stdby_freq_hz",
        DATA_REFERENCE_REPLY_INDEX_COM_STANDBY_FREQUENCY,
        1
    );

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit/radios/nav1_freq_hz",
        DATA_REFERENCE_REPLY_INDEX_NAV_FREQUENCY,
        1
    );

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit/radios/nav1_stdby_freq_hz",
        DATA_REFERENCE_REPLY_INDEX_NAV_STANDBY_FREQUENCY,
        1
    );

    button_initialize(
        &com_nav_radio->button_com_frequency_transfer,
        COM_NAV_RADIO_COM_FREQUENCY_TRANSFER_BUTTON_PIN
    );

    button_initialize(
        &com_nav_radio->button_nav_frequency_transfer,
        COM_NAV_RADIO_NAV_FREQUENCY_TRANSFER_BUTTON_PIN
    );

    encoder_initialize(
        &com_nav_radio->encoder_com_khz,
        COM_NAV_RADIO_COM_KHZ_ENCODER_PIN_1,
        COM_NAV_RADIO_COM_KHZ_ENCODER_PIN_2,
        (range_s) {.minimum = 0, .maximum = 1000},
        10
    );

    encoder_initialize(
        &com_nav_radio->encoder_com_mhz,
        COM_NAV_RADIO_COM_MHZ_ENCODER_PIN_1,
        COM_NAV_RADIO_COM_MHZ_ENCODER_PIN_2,
        (range_s) {.minimum = 118, .maximum = 136},
        1
    );

    encoder_initialize(
        &com_nav_radio->encoder_nav_khz,
        COM_NAV_RADIO_NAV_KHZ_ENCODER_PIN_1,
        COM_NAV_RADIO_NAV_KHZ_ENCODER_PIN_2,
        (range_s) {.minimum = 0, .maximum = 1000},
        10
    );

    encoder_initialize(
        &com_nav_radio->encoder_nav_mhz,
        COM_NAV_RADIO_NAV_MHZ_ENCODER_PIN_1,
        COM_NAV_RADIO_NAV_MHZ_ENCODER_PIN_2,
        (range_s) {.minimum = 118, .maximum = 136},
        1
    );

    encoder_set_data_reference_path(
        &com_nav_radio->encoder_com_khz,
        "sim/cockpit2/radios/actuators/com1_standby_frequency_khz"
    );

    encoder_set_data_reference_path(
        &com_nav_radio->encoder_com_mhz,
        "sim/cockpit2/radios/actuators/com1_standby_frequency_Mhz"
    );

    encoder_set_data_reference_path(
        &com_nav_radio->encoder_nav_khz,
        "sim/cockpit2/radios/actuators/nav1_standby_frequency_khz"
    );

    encoder_set_data_reference_path(
        &com_nav_radio->encoder_nav_mhz,
        "sim/cockpit2/radios/actuators/nav1_standby_frequency_Mhz"
    );
}

void com_nav_radio_run(com_nav_radio_s *com_nav_radio, rref_reply_packet_s *received_packets)
{
    button_read(&com_nav_radio->button_com_frequency_transfer);
    button_read(&com_nav_radio->button_nav_frequency_transfer);

    if (button_is_pressed(&com_nav_radio->button_com_frequency_transfer))
    {
        com_nav_radio->com_right_is_selected = !com_nav_radio->com_right_is_selected;

        x_plane_interface_send_data_reference_value(
            "sim/cockpit2/radios/actuators/com1_right_is_selected",
            com_nav_radio->com_right_is_selected
        );
    }

    if (button_is_pressed(&com_nav_radio->button_nav_frequency_transfer))
    {
        com_nav_radio->nav_right_is_selected = !com_nav_radio->nav_right_is_selected;

        x_plane_interface_send_data_reference_value(
            "sim/cockpit2/radios/actuators/nav1_right_is_selected",
            com_nav_radio->nav_right_is_selected
        );
    }

    encoder_run(&com_nav_radio->encoder_com_khz);
    encoder_run(&com_nav_radio->encoder_com_mhz);
    encoder_run(&com_nav_radio->encoder_nav_khz);
    encoder_run(&com_nav_radio->encoder_nav_mhz);

    if (received_packets[0].index == -1)
        return;

    for (uint8_t i = 0; i < 15; ++i)
    {
        switch (received_packets[i].index)
        {
            case DATA_REFERENCE_REPLY_INDEX_COM_FREQUENCY:
                com_nav_radio_set_frequency_for_display(com_nav_radio, 0, received_packets[i].value);
                break;

            case DATA_REFERENCE_REPLY_INDEX_COM_STANDBY_FREQUENCY:
                com_nav_radio_set_frequency_for_display(com_nav_radio, 1, received_packets[i].value);
                break;

            case DATA_REFERENCE_REPLY_INDEX_NAV_FREQUENCY:
                com_nav_radio_set_frequency_for_display(com_nav_radio, 2, received_packets[i].value);
                break;

            case DATA_REFERENCE_REPLY_INDEX_NAV_STANDBY_FREQUENCY:
                com_nav_radio_set_frequency_for_display(com_nav_radio, 3, received_packets[i].value);
                break;

            default:
                break;
        }
    }
}

void com_nav_radio_set_frequency_for_display(
    com_nav_radio_s *com_nav_radio,
    uint8_t display_number,
    uint32_t frequency
)
{
    com_nav_radio->display.setDigit(display_number, 0, (frequency / 10000) % 10, 0);
    com_nav_radio->display.setDigit(display_number, 1, (frequency / 1000) % 10, 0);
    com_nav_radio->display.setDigit(display_number, 2, (frequency / 100) % 10, 1);
    com_nav_radio->display.setDigit(display_number, 3, (frequency / 10) % 10, 0);
    com_nav_radio->display.setDigit(display_number, 4, frequency % 10, 0);
}