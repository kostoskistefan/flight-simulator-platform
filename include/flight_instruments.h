#ifndef FLIGHT_INSTRUMENTS_H
#define FLIGHT_INSTRUMENTS_H

#include <stdint.h>

void airspeed_indicator(void);
void airspeed_indicator_set_value(uint8_t speed);

void altimeter(void);
void altimeter_set_value(int32_t altitude);

void attitude_indicator(void);
void attitude_indicator_set_roll_value(int8_t roll);
void attitude_indicator_set_pitch_value(int8_t pitch);

void vertical_speed_indicator(void);
void vertical_speed_indicator_set_value(int32_t vertical_speed);

void heading_indicator(void);
void heading_indicator_set_value(uint16_t heading);

#endif // FLIGHT_INSTRUMENTS_H
