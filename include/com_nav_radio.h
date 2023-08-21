#ifndef COM_NAV_RADIO_H
#define COM_NAV_RADIO_H

#include "button.h"
#include "encoder.h"
#include "LedControl_SW_SPI.h"

typedef struct com_nav_radio_s
{
    uint8_t com_right_is_selected;
    uint8_t nav_right_is_selected;

    button_s button_com_frequency_transfer;
    button_s button_nav_frequency_transfer;

    encoder_s encoder_com_mhz;
    encoder_s encoder_com_khz;
    encoder_s encoder_nav_mhz;
    encoder_s encoder_nav_khz;

    LedControl_SW_SPI display;
} com_nav_radio_s;

void com_nav_radio_initialize(com_nav_radio_s *com_nav_radio);
void com_nav_radio_read(com_nav_radio_s *com_nav_radio);
void com_nav_radio_send_update(com_nav_radio_s *com_nav_radio);
void com_nav_radio_set_frequency_for_display(com_nav_radio_s *com_nav_radio, uint8_t display_number, uint32_t frequency);

#endif // COM_NAV_RADIO_H