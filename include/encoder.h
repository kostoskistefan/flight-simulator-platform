#ifndef ENCODER_H
#define ENCODER_H

#include "common.h"
#include "range.h"

typedef struct encoder_s
{
    uint8_t pin_1;
    uint8_t pin_2;

    uint8_t pin_1_state;
    uint8_t pin_2_state;

    uint8_t pin_1_previous_state;
    uint8_t pin_2_previous_state;

    uint8_t value_has_changed;

    range_s value_range;
    float value_adjustment_amount;

    float value;
    const char *data_reference_path;
} encoder_s;

void encoder_initialize(
    encoder_s *encoder, 
    uint8_t pin_1,
    uint8_t pin_2,
    range_s value_range, 
    float value_adjustment_amount);
void encoder_set_data_reference_path(encoder_s *encoder, const char *data_reference_path);
void encoder_run(encoder_s *encoder);

#endif // ENCODER_H