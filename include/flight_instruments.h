#ifndef FLIGHT_INSTRUMENTS_H
#define FLIGHT_INSTRUMENTS_H

#include <lvgl.h>
#include "x_plane_interface.h"

typedef struct flight_instruments_s
{
    lv_obj_t *airspeed_indicator_meter;
    lv_meter_indicator_t *airspeed_indicator_needle;

    lv_obj_t *altimeter_meter;
    lv_meter_indicator_t *altimeter_one_thousand_feet_needle;
    lv_meter_indicator_t *altimeter_ten_thousand_feet_needle;

    lv_obj_t *vertical_speed_indicator_meter;
    lv_meter_indicator_t *vertical_speed_indicator_needle;

    lv_obj_t *heading_meter;
    lv_meter_indicator_t *heading_needle;

    lv_obj_t *attitude_meter;
    lv_meter_indicator_t *attitude_roll_needle;
    lv_meter_indicator_t *attitude_pitch_needle;
} flight_instruments_s;

void flight_instruments_initialize(flight_instruments_s *flight_instruments);
void flight_instruments_run(flight_instruments_s *flight_instruments, rref_reply_packet_s *received_packet);

#endif // FLIGHT_INSTRUMENTS_H
