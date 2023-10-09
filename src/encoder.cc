#include "encoder.h"
#include "x_plane_interface.h"

const int8_t direction_table[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

void encoder_initialize(
    encoder_s *encoder,
    uint8_t pin_1,
    uint8_t pin_2,
    range_s value_range,
    float value_adjustment_amount, 
    const char *data_reference_path
)
{
    encoder->pin_1 = pin_1;
    encoder->pin_2 = pin_2;

    encoder->old_state = 0;
    encoder->position = 0;
    encoder->previous_position = 0;

    encoder->value = 0;
    encoder->value_range = value_range;
    encoder->value_adjustment_amount = value_adjustment_amount;

    encoder->data_reference_path = data_reference_path;

    pinMode(encoder->pin_1, INPUT_PULLUP);
    pinMode(encoder->pin_2, INPUT_PULLUP);
}

void encoder_run(encoder_s *encoder)
{
    int8_t current_state = digitalRead(encoder->pin_1) | (digitalRead(encoder->pin_2) << 1);

    if (encoder->old_state == current_state)
        return;

    encoder->position += direction_table[current_state | (encoder->old_state << 2)];
    encoder->old_state = current_state;

    if (current_state != 0b11)
        return;

    long current_position = encoder->position >> 2; 

    if (encoder->previous_position > current_position)
    {
        encoder->value += encoder->value_adjustment_amount;
        encoder->previous_position = current_position;
    }

    else if (encoder->previous_position < current_position)
    {
        encoder->value -= encoder->value_adjustment_amount;
        encoder->previous_position = current_position;
    }

    encoder->value = constrain(encoder->value, encoder->value_range.minimum, encoder->value_range.maximum);

    x_plane_interface_send_data_reference_value(
        encoder->data_reference_path,
        encoder->value
    );
}
