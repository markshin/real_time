#ifndef PALS__PALS_TIMER_H
#define PALS__PALS_TIMER_H

// TODO: Wrapper for timerfd?
#include "pals_time.h"
#include <stdint.h>

typedef struct _pals_timer {
  int id;
} pals_timer_t;

// initialize timer
void pals_init_timer(pals_timer_t *timer);

// set the initial time and period
void pals_set_timer(pals_timer_t *timer, pals_time_t initial_time, uint64_t period);

// wait timer clock
int pals_wait_timer(pals_timer_t *timer);

#endif /* PALS__PALS_TIMER_H */
