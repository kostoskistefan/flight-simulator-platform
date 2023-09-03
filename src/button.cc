#include "button.h"
#include "common.h"

void button_initialize(button_s *button, uint8_t pin)
{
    button->pin = pin;
    button->state = 0;
    button->last_state = 0;
    button->state_has_changed = 0;
    button->time_since_last_change = millis();
    pinMode(button->pin, INPUT_PULLDOWN);
}

void button_read(button_s *button)
{
    uint32_t current_time = millis();

    if (current_time - button->time_since_last_change < BUTTON_DEBOUNCE_TIME)
        button->state_has_changed = 0;

    else
    {
        button->last_state = button->state;
        button->state = digitalRead(button->pin);
        button->state_has_changed = (button->state != button->last_state);

        if (button->state_has_changed)
            button->time_since_last_change = current_time;
    }
}

uint8_t button_is_pressed(button_s *button)
{
    return (button->state && button->state_has_changed);
}