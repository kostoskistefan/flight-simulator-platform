#include <flight_instruments.h>
#include "tft.h"
#include "common.h"

void flight_instruments_airspeed_indicator(flight_instruments_s *flight_instruments);
void flight_instruments_altimeter(flight_instruments_s *flight_instruments);
void flight_instruments_attitude_indicator(flight_instruments_s *flight_instruments);
void flight_instruments_vertical_speed_indicator(flight_instruments_s *flight_instruments);
void flight_instruments_heading_indicator(flight_instruments_s *flight_instruments);
void flight_instruments_set_meter_style(lv_obj_t *meter, lv_color_t highlight_color, const lv_font_t *meter_ticks_font);

void flight_instruments_initialize(flight_instruments_s *flight_instruments)
{
    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/airspeed_kts_pilot",
        DATA_REFERENCE_REPLY_INDEX_AIRSPEED,
        2
    );

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/altitude_ft_pilot",
        DATA_REFERENCE_REPLY_INDEX_ALTITUDE,
        2
    );

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/roll_vacuum_deg_pilot",
        DATA_REFERENCE_REPLY_INDEX_ATTITUDE_ROLL,
        2
    );

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/pitch_vacuum_deg_pilot",
        DATA_REFERENCE_REPLY_INDEX_ATTITUDE_PITCH,
        2
    );

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/heading_electric_deg_mag_pilot",
        DATA_REFERENCE_REPLY_INDEX_HEADING,
        2
    );

    x_plane_interface_subscribe_to_data_reference(
        "sim/cockpit2/gauges/indicators/vvi_fpm_pilot",
        DATA_REFERENCE_REPLY_INDEX_VERTICAL_SPEED,
        2
    );

    tft_initialize();

    flight_instruments_airspeed_indicator(flight_instruments);
    flight_instruments_altimeter(flight_instruments);
    flight_instruments_attitude_indicator(flight_instruments);
    flight_instruments_heading_indicator(flight_instruments);
    flight_instruments_vertical_speed_indicator(flight_instruments);

    tft_timer_handler();
}

void flight_instruments_run(flight_instruments_s *flight_instruments, rref_reply_packet_s *received_packet)
{
    tft_timer_handler();

    if (received_packet->index == -1)
        return;

    switch (received_packet->index)
    {
        case DATA_REFERENCE_REPLY_INDEX_AIRSPEED:
            lv_meter_set_indicator_value(
                flight_instruments->airspeed_indicator_meter,
                flight_instruments->airspeed_indicator_needle,
                received_packet->value
            );
            break;

        case DATA_REFERENCE_REPLY_INDEX_ALTITUDE:
            lv_meter_set_indicator_value(
                flight_instruments->altimeter_meter,
                flight_instruments->altimeter_one_thousand_feet_needle,
                (((int32_t) (received_packet->value)) / 10) % 100
            );
            lv_meter_set_indicator_value(
                flight_instruments->altimeter_meter,
                flight_instruments->altimeter_ten_thousand_feet_needle,
                (((int32_t) (received_packet->value)) / 100) % 100
            );
            break;

        case DATA_REFERENCE_REPLY_INDEX_ATTITUDE_ROLL:
            lv_meter_set_indicator_value(
                flight_instruments->attitude_meter,
                flight_instruments->attitude_roll_needle,
                received_packet->value
            );
            break;

        case DATA_REFERENCE_REPLY_INDEX_ATTITUDE_PITCH:
            lv_meter_set_indicator_value(
                flight_instruments->attitude_meter,
                flight_instruments->attitude_pitch_needle,
                received_packet->value
            );
            break;

        case DATA_REFERENCE_REPLY_INDEX_HEADING:
            lv_meter_set_indicator_value(
                flight_instruments->heading_meter,
                flight_instruments->heading_needle,
                ((int32_t) (received_packet->value)) % 360
            );
            break;

        case DATA_REFERENCE_REPLY_INDEX_VERTICAL_SPEED:
            lv_meter_set_indicator_value(
                flight_instruments->vertical_speed_indicator_meter,
                flight_instruments->vertical_speed_indicator_needle,
                received_packet->value
            );
            break;

        default:
            break;
    }
}

void flight_instruments_set_meter_style(lv_obj_t *meter, lv_color_t highlight_color, const lv_font_t *meter_ticks_font)
{
    lv_obj_set_style_pad_all(meter, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(meter, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_color(meter, highlight_color, LV_PART_MAIN);
    lv_obj_set_style_text_color(meter, lv_color_white(), LV_PART_TICKS);
    lv_obj_set_style_text_font(meter, meter_ticks_font, LV_PART_TICKS);
}

void flight_instruments_airspeed_indicator(flight_instruments_s *flight_instruments)
{
    flight_instruments->airspeed_indicator_meter = lv_meter_create(lv_scr_act());

    lv_obj_set_size(flight_instruments->airspeed_indicator_meter, 116, 116);
    lv_obj_align(flight_instruments->airspeed_indicator_meter, LV_ALIGN_TOP_LEFT, 0, 0);

    flight_instruments_set_meter_style(flight_instruments->airspeed_indicator_meter, lv_color_white(), &lv_font_montserrat_10);

    lv_meter_scale_t *scale = lv_meter_add_scale(flight_instruments->airspeed_indicator_meter);
    lv_meter_set_scale_range(flight_instruments->airspeed_indicator_meter, scale, 0, 200, 330, 270);
    lv_meter_set_scale_ticks(flight_instruments->airspeed_indicator_meter, scale, 21, 2, 7, lv_color_white());
    lv_meter_set_scale_major_ticks(flight_instruments->airspeed_indicator_meter, scale, 2, 2, 9, lv_color_white(), 11);

    lv_meter_indicator_t *indicator =
        lv_meter_add_arc(flight_instruments->airspeed_indicator_meter, scale, 4, lv_color_white(), 1);
    lv_meter_set_indicator_start_value(flight_instruments->airspeed_indicator_meter, indicator, 0);
    lv_meter_set_indicator_end_value(flight_instruments->airspeed_indicator_meter, indicator, 40);

    indicator = lv_meter_add_arc(flight_instruments->airspeed_indicator_meter, scale, 4, lv_palette_main(LV_PALETTE_GREEN), 1);
    lv_meter_set_indicator_start_value(flight_instruments->airspeed_indicator_meter, indicator, 40);
    lv_meter_set_indicator_end_value(flight_instruments->airspeed_indicator_meter, indicator, 130);

    indicator = lv_meter_add_arc(flight_instruments->airspeed_indicator_meter, scale, 4, lv_palette_main(LV_PALETTE_YELLOW), 1);
    lv_meter_set_indicator_start_value(flight_instruments->airspeed_indicator_meter, indicator, 130);
    lv_meter_set_indicator_end_value(flight_instruments->airspeed_indicator_meter, indicator, 160);

    indicator = lv_meter_add_arc(flight_instruments->airspeed_indicator_meter, scale, 4, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(flight_instruments->airspeed_indicator_meter, indicator, 160);
    lv_meter_set_indicator_end_value(flight_instruments->airspeed_indicator_meter, indicator, 200);

    indicator =
        lv_meter_add_scale_lines(flight_instruments->airspeed_indicator_meter, scale, lv_color_white(), lv_color_white(), 0, 0);
    lv_meter_set_indicator_start_value(flight_instruments->airspeed_indicator_meter, indicator, 0);
    lv_meter_set_indicator_end_value(flight_instruments->airspeed_indicator_meter, indicator, 40);

    indicator = lv_meter_add_scale_lines(
        flight_instruments->airspeed_indicator_meter,
        scale,
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_GREEN),
        0,
        0
    );
    lv_meter_set_indicator_start_value(flight_instruments->airspeed_indicator_meter, indicator, 40);
    lv_meter_set_indicator_end_value(flight_instruments->airspeed_indicator_meter, indicator, 130);

    indicator = lv_meter_add_scale_lines(
        flight_instruments->airspeed_indicator_meter,
        scale,
        lv_palette_main(LV_PALETTE_YELLOW),
        lv_palette_main(LV_PALETTE_YELLOW),
        0,
        0
    );
    lv_meter_set_indicator_start_value(flight_instruments->airspeed_indicator_meter, indicator, 130);
    lv_meter_set_indicator_end_value(flight_instruments->airspeed_indicator_meter, indicator, 160);

    indicator = lv_meter_add_scale_lines(
        flight_instruments->airspeed_indicator_meter,
        scale,
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_RED),
        0,
        0
    );
    lv_meter_set_indicator_start_value(flight_instruments->airspeed_indicator_meter, indicator, 160);
    lv_meter_set_indicator_end_value(flight_instruments->airspeed_indicator_meter, indicator, 200);

    lv_obj_t *label = lv_label_create(flight_instruments->airspeed_indicator_meter);
    lv_label_set_text(label, "KTS");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 18);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);

    flight_instruments->airspeed_indicator_needle =
        lv_meter_add_needle_line(flight_instruments->airspeed_indicator_meter, scale, 2, lv_color_white(), -10);
}

void flight_instruments_altimeter(flight_instruments_s *flight_instruments)
{
    flight_instruments->altimeter_meter = lv_meter_create(lv_scr_act());

    lv_obj_set_size(flight_instruments->altimeter_meter, 116, 116);
    lv_obj_align(flight_instruments->altimeter_meter, LV_ALIGN_TOP_RIGHT, 0, 0);

    flight_instruments_set_meter_style(
        flight_instruments->altimeter_meter,
        lv_palette_main(LV_PALETTE_RED),
        &lv_font_montserrat_12
    );

    lv_meter_scale_t *one_thousand_feet_scale = lv_meter_add_scale(flight_instruments->altimeter_meter);
    lv_meter_scale_t *one_thousand_feet_scale_visual = lv_meter_add_scale(flight_instruments->altimeter_meter);
    lv_meter_scale_t *ten_thousand_feet_scale = lv_meter_add_scale(flight_instruments->altimeter_meter);
    lv_meter_scale_t *ten_thousand_feet_scale_visual = lv_meter_add_scale(flight_instruments->altimeter_meter);

    lv_meter_set_scale_range(flight_instruments->altimeter_meter, one_thousand_feet_scale, 0, 100, 360, 270);
    lv_meter_set_scale_range(flight_instruments->altimeter_meter, ten_thousand_feet_scale, 0, 100, 360, 270);
    lv_meter_set_scale_range(flight_instruments->altimeter_meter, one_thousand_feet_scale_visual, 0, 50, 360, 270);
    lv_meter_set_scale_range(flight_instruments->altimeter_meter, ten_thousand_feet_scale_visual, 0, 9, 324, 270);
    lv_meter_set_scale_ticks(flight_instruments->altimeter_meter, one_thousand_feet_scale_visual, 51, 1, 7, lv_color_white());
    lv_meter_set_scale_ticks(flight_instruments->altimeter_meter, ten_thousand_feet_scale_visual, 10, 1, 5, lv_color_white());
    lv_meter_set_scale_major_ticks(
        flight_instruments->altimeter_meter,
        ten_thousand_feet_scale_visual,
        1,
        2,
        9,
        lv_color_white(),
        10
    );

    lv_obj_t *label = lv_label_create(flight_instruments->altimeter_meter);
    lv_label_set_text(label, "ALT");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 16);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

    flight_instruments->altimeter_one_thousand_feet_needle = lv_meter_add_needle_line(
        flight_instruments->altimeter_meter,
        one_thousand_feet_scale,
        2,
        lv_palette_main(LV_PALETTE_RED),
        -10
    );

    flight_instruments->altimeter_ten_thousand_feet_needle = lv_meter_add_needle_line(
        flight_instruments->altimeter_meter,
        ten_thousand_feet_scale,
        4,
        lv_palette_main(LV_PALETTE_RED),
        -20
    );
}

void flight_instruments_attitude_indicator(flight_instruments_s *flight_instruments)
{
    flight_instruments->attitude_meter = lv_meter_create(lv_scr_act());

    lv_obj_set_size(flight_instruments->attitude_meter, 116, 116);
    lv_obj_align(flight_instruments->attitude_meter, LV_ALIGN_CENTER, 0, 0);

    flight_instruments_set_meter_style(
        flight_instruments->attitude_meter,
        lv_palette_main(LV_PALETTE_BLUE_GREY),
        &lv_font_montserrat_12
    );

    lv_meter_scale_t *roll_scale = lv_meter_add_scale(flight_instruments->attitude_meter);
    lv_meter_set_scale_range(flight_instruments->attitude_meter, roll_scale, -60, 60, 120, 210);
    lv_meter_set_scale_ticks(flight_instruments->attitude_meter, roll_scale, 13, 1, 5, lv_color_white());
    lv_meter_set_scale_major_ticks(flight_instruments->attitude_meter, roll_scale, 3, 2, 9, lv_color_white(), 11);

    lv_meter_scale_t *pitch_scale = lv_meter_add_scale(flight_instruments->attitude_meter);
    lv_meter_set_scale_range(flight_instruments->attitude_meter, pitch_scale, -20, 20, 120, 30);
    lv_meter_set_scale_ticks(flight_instruments->attitude_meter, pitch_scale, 9, 1, 5, lv_color_white());
    lv_meter_set_scale_major_ticks(flight_instruments->attitude_meter, pitch_scale, 2, 2, 9, lv_color_white(), 11);

    lv_meter_indicator_t *indicator =
        lv_meter_add_arc(flight_instruments->attitude_meter, pitch_scale, 10, lv_palette_main(LV_PALETTE_BLUE), 1);
    lv_meter_set_indicator_start_value(flight_instruments->attitude_meter, indicator, 0);
    lv_meter_set_indicator_end_value(flight_instruments->attitude_meter, indicator, 20);

    indicator = lv_meter_add_arc(flight_instruments->attitude_meter, pitch_scale, 10, lv_palette_main(LV_PALETTE_BROWN), 1);
    lv_meter_set_indicator_start_value(flight_instruments->attitude_meter, indicator, -20);
    lv_meter_set_indicator_end_value(flight_instruments->attitude_meter, indicator, 0);

    indicator = lv_meter_add_arc(flight_instruments->attitude_meter, roll_scale, 10, lv_palette_main(LV_PALETTE_BLUE), 1);
    lv_meter_set_indicator_start_value(flight_instruments->attitude_meter, indicator, -60);
    lv_meter_set_indicator_end_value(flight_instruments->attitude_meter, indicator, 60);

    lv_obj_t *instrument_label = lv_label_create(flight_instruments->attitude_meter);
    lv_label_set_text(instrument_label, "ATT");
    lv_obj_align(instrument_label, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_obj_set_style_text_font(instrument_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(instrument_label, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_PART_MAIN);

    lv_obj_t *roll_label = lv_label_create(flight_instruments->attitude_meter);
    lv_label_set_text(roll_label, "Roll");
    lv_obj_align(roll_label, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_font(roll_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(roll_label, lv_color_white(), LV_PART_MAIN);

    lv_obj_t *pitch_label = lv_label_create(flight_instruments->attitude_meter);
    lv_label_set_text(pitch_label, "Pitch");
    lv_obj_align(pitch_label, LV_ALIGN_CENTER, 0, 17);
    lv_obj_set_style_text_font(pitch_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(pitch_label, lv_color_white(), LV_PART_MAIN);

    flight_instruments->attitude_roll_needle =
        lv_meter_add_needle_line(flight_instruments->attitude_meter, roll_scale, 2, lv_color_white(), -10);

    flight_instruments->attitude_pitch_needle =
        lv_meter_add_needle_line(flight_instruments->attitude_meter, pitch_scale, 2, lv_color_white(), -10);
}

void flight_instruments_heading_indicator(flight_instruments_s *flight_instruments)
{
    flight_instruments->heading_meter = lv_meter_create(lv_scr_act());

    lv_obj_set_size(flight_instruments->heading_meter, 116, 116);
    lv_obj_align(flight_instruments->heading_meter, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    flight_instruments_set_meter_style(
        flight_instruments->heading_meter,
        lv_palette_main(LV_PALETTE_ORANGE),
        &lv_font_montserrat_12
    );

    lv_meter_scale_t *scale = lv_meter_add_scale(flight_instruments->heading_meter);
    lv_meter_set_scale_range(flight_instruments->heading_meter, scale, 0, 359, 360, 270);

    lv_meter_scale_t *scale_visual = lv_meter_add_scale(flight_instruments->heading_meter);
    lv_meter_set_scale_range(flight_instruments->heading_meter, scale_visual, 0, 35, 350, 270);
    lv_meter_set_scale_ticks(flight_instruments->heading_meter, scale_visual, 36, 1, 4, lv_color_white());
    lv_meter_set_scale_major_ticks(flight_instruments->heading_meter, scale_visual, 3, 2, 5, lv_color_white(), 11);

    lv_obj_t *label = lv_label_create(flight_instruments->heading_meter);
    lv_label_set_text(label, "HDG");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 16);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);

    flight_instruments->heading_needle =
        lv_meter_add_needle_line(flight_instruments->heading_meter, scale, 2, lv_palette_main(LV_PALETTE_ORANGE), -10);
}

void flight_instruments_vertical_speed_indicator(flight_instruments_s *flight_instruments)
{
    flight_instruments->vertical_speed_indicator_meter = lv_meter_create(lv_scr_act());

    lv_obj_set_size(flight_instruments->vertical_speed_indicator_meter, 116, 116);
    lv_obj_align(flight_instruments->vertical_speed_indicator_meter, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    flight_instruments_set_meter_style(
        flight_instruments->vertical_speed_indicator_meter,
        lv_palette_main(LV_PALETTE_CYAN),
        &lv_font_montserrat_12
    );

    lv_meter_scale_t *scale = lv_meter_add_scale(flight_instruments->vertical_speed_indicator_meter);
    lv_meter_set_scale_range(flight_instruments->vertical_speed_indicator_meter, scale, -2000, 2000, 300, 30);

    lv_meter_scale_t *scale_visual = lv_meter_add_scale(flight_instruments->vertical_speed_indicator_meter);
    lv_meter_set_scale_range(flight_instruments->vertical_speed_indicator_meter, scale_visual, -20, 20, 300, 30);
    lv_meter_set_scale_ticks(flight_instruments->vertical_speed_indicator_meter, scale_visual, 41, 1, 5, lv_color_white());
    lv_meter_set_scale_major_ticks(
        flight_instruments->vertical_speed_indicator_meter,
        scale_visual,
        5,
        2,
        9,
        lv_color_white(),
        11
    );

    lv_obj_t *label = lv_label_create(flight_instruments->vertical_speed_indicator_meter);
    lv_label_set_text(label, "VSI");
    lv_obj_align(label, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_CYAN), LV_PART_MAIN);

    flight_instruments->vertical_speed_indicator_needle = lv_meter_add_needle_line(
        flight_instruments->vertical_speed_indicator_meter,
        scale,
        2,
        lv_palette_main(LV_PALETTE_CYAN),
        -10
    );
}
