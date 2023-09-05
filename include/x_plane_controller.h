#ifndef X_PLANE_CONTROLLER_H
#define X_PLANE_CONTROLLER_H

#include "yoke.h"
#include "com_nav_radio.h"
#include "elevator_trim.h"
#include "engine_controls.h"
#include "flight_instruments.h"
#include "x_plane_interface.h"

typedef struct x_plane_controller_s
{
    yoke_s yoke;
    com_nav_radio_s com_nav_radio;
    elevator_trim_s elevator_trim;
    engine_controls_s engine_controls;
    flight_instruments_s flight_instruments;

    rref_reply_packet_s received_packets[15];
} x_plane_controller_s;

void x_plane_controller_initialize(x_plane_controller_s *x_plane_controller);
void x_plane_controller_run(x_plane_controller_s *x_plane_controller);

#endif // X_PLANE_CONTROLLER_H