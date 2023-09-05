#include "elevator_trim.h"

void elevator_trim_initialize(elevator_trim_s *elevator_trim)
{
    encoder_initialize(
        elevator_trim, 
        ELEVATOR_TRIM_PIN_1, 
        ELEVATOR_TRIM_PIN_2,
        (range_s) {.minimum = -1, .maximum = 1},
        0.05
    );
    encoder_set_data_reference_path(elevator_trim, "sim/cockpit2/controls/elevator_trim");
}

void elevator_trim_run(elevator_trim_s *elevator_trim)
{
    encoder_run(elevator_trim);
}