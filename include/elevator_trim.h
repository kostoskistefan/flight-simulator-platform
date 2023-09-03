#ifndef ELEVATOR_TRIM_H
#define ELEVATOR_TRIM_H

#include "common.h"
#include "encoder.h"

typedef encoder_s elevator_trim_s;

void elevator_trim_initialize(elevator_trim_s *elevator_trim);
void elevator_trim_run(elevator_trim_s *elevator_trim);

#endif // ELEVATOR_TRIM_H