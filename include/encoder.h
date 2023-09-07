#ifndef ENCODER_H
#define ENCODER_H

#include "common.h"
#include "range.h"

typedef struct encoder_s
{
    uint8_t pin_1;
    uint8_t pin_2;

    int8_t old_state;
    
    long position;
    long previous_position; 

    range_s value_range;

    float value;
    float value_adjustment_amount;

    const char *data_reference_path;
} encoder_s;

void encoder_initialize(
    encoder_s *encoder, 
    uint8_t pin_1,
    uint8_t pin_2,
    range_s value_range, 
    float value_adjustment_amount,
    const char *data_reference_path);
void encoder_run(encoder_s *encoder);

#endif // ENCODER_H