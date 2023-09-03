#include <encoder.h>
#include "x_plane_interface.h"

static const int8_t direction_table[4][4] = {
    { 0, -1,  1,  0},
    { 1,  0,  0, -1},
    {-1,  0,  0,  1},
    { 0,  1, -1,  0}
};

void encoder_initialize(encoder_s *encoder, uint8_t pin_1, uint8_t pin_2)
{
    encoder->pin_1 = pin_1;
    encoder->pin_2 = pin_2;

    encoder->pin_1_state = 0;
    encoder->pin_2_state = 0;

    encoder->pin_1_previous_state = 0;
    encoder->pin_2_previous_state = 0;

    encoder->value_has_changed = 0;

    encoder->value = 0;

    pinMode(encoder->pin_1, INPUT_PULLDOWN);
    pinMode(encoder->pin_2, INPUT_PULLDOWN);
}

void encoder_set_data_reference_path(encoder_s *encoder, const char *data_reference_path)
{
    encoder->data_reference_path = data_reference_path;
}

void encoder_run(encoder_s *encoder)
{
    encoder->pin_1_state = digitalRead(encoder->pin_1);
    encoder->pin_2_state = digitalRead(encoder->pin_2);

    uint8_t current_state = (encoder->pin_1_state << 1) | encoder->pin_2_state;
    uint8_t previous_state = (encoder->pin_1_previous_state << 1) | encoder->pin_2_previous_state;

    encoder->value += 0.05 * direction_table[previous_state][current_state];

    encoder->value_has_changed |= (direction_table[previous_state][current_state] != 0);

    encoder->pin_1_previous_state = encoder->pin_1_state;
    encoder->pin_2_previous_state = encoder->pin_2_state;

    if (encoder->value_has_changed)
    {
        x_plane_interface_send_data_reference_value(
            encoder->data_reference_path,
            encoder->value
        );
        
        encoder->value_has_changed = 0;
    }
}