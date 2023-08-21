#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

typedef struct button_s
{
    uint8_t pin;
    uint8_t state;
    uint8_t last_state;
    uint8_t state_has_changed;
    uint32_t time_since_last_change;
} button_s;

void button_initialize(button_s *button, uint8_t pin);
void button_read(button_s *button);
uint8_t button_is_pressed(button_s *button);

#endif // BUTTON_H