#include "encoder.h"
#include "x_plane_interface.h"

#define DIRECTION_CLOCKWISE         16
#define DIRECTION_COUNTER_CLOCKWISE 32

const unsigned char ttable[7][4] = {
    {0, 2, 4,  0},
    {3, 0, 1, 16},
    {3, 2, 0,  0},
    {3, 2, 1,  0},
    {6, 0, 4,  0},
    {6, 5, 0, 32},
    {6, 5, 4,  0},
};

void encoder_initialize(
    encoder_s *encoder,
    uint8_t pin_1,
    uint8_t pin_2,
    range_s value_range,
    float value_adjustment_amount
)
{
    encoder->pin_1 = pin_1;
    encoder->pin_2 = pin_2;

    encoder->pin_1_state = 0;
    encoder->pin_2_state = 0;

    encoder->value_has_changed = 0;

    encoder->value = value_range.minimum;

    encoder->value_range = value_range;
    encoder->value_adjustment_amount = value_adjustment_amount;

    pinMode(encoder->pin_1, INPUT_PULLUP);
    pinMode(encoder->pin_2, INPUT_PULLUP);
}

void encoder_set_data_reference_path(encoder_s *encoder, const char *data_reference_path)
{
    encoder->data_reference_path = data_reference_path;
}

void encoder_run(encoder_s *encoder)
{
    encoder->pin_1_state = digitalRead(encoder->pin_1);
    encoder->pin_2_state = digitalRead(encoder->pin_2);

    static int state = 0;

    unsigned char pins_state = (encoder->pin_2_state << 1) | encoder->pin_1_state;

    // Determine new state from the pins and state table.
    state = ttable[state & 0x0f][pins_state];

    Serial.print("Pins state: ");
    Serial.println(state);

    // Return emit bits, ie the generated event.
    if ((state & 0x30) == DIRECTION_CLOCKWISE)
    {
        encoder->value += encoder->value_adjustment_amount;
        encoder->value_has_changed = 1;
    }

    else if ((state & 0x30) == DIRECTION_COUNTER_CLOCKWISE)
    {
        encoder->value -= encoder->value_adjustment_amount;
        encoder->value_has_changed = 1;
    }

    // static uint8_t current_state = (encoder->pin_1_state << 1) | encoder->pin_2_state;
    // static uint8_t previous_state = 0;
    // static uint32_t last_debounce_time = 0;

    // if (current_state != previous_state)
    //     last_debounce_time = millis();

    // if (millis() - last_debounce_time >= BUTTON_DEBOUNCE_TIME)
    // {
    //     if (current_state == 0b11)
    //     {
    //         if (previous_state == 0b01)
    //         {
    //             encoder->value += encoder->value_adjustment_amount;
    //             encoder->value_has_changed = 1;
    //         }

    //         else if (previous_state == 0b10)
    //         {
    //             encoder->value -= encoder->value_adjustment_amount;
    //             encoder->value_has_changed = 1;
    //         }
    //     }
    // }

    // previous_state = current_state;

    encoder->value = constrain(encoder->value, encoder->value_range.minimum, encoder->value_range.maximum);

    if (encoder->value_has_changed)
    {
        Serial.println(encoder->value);

        // x_plane_interface_send_data_reference_value(
        //     encoder->data_reference_path,
        //     encoder->value
        // );

        encoder->value_has_changed = 0;
    }
}