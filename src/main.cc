#include "x_plane_controller.h"

x_plane_controller_s x_plane_controller;

void setup(void)
{
    Serial.begin(115200);
    x_plane_controller_initialize(&x_plane_controller);
}

void loop(void)
{
    x_plane_controller_run(&x_plane_controller);
}