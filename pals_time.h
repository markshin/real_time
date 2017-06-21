#ifndef PALS__PALS_TIME_H
#define PALS__PALS_TIME_H

#include <stdint.h>

typedef struct _pals_time {
  uint64_t sec;
  uint64_t nanosec;
} pals_time_t;

/* time_t get_gcd_time(time_t t1, time_t t2); */

pals_time_t get_base_time(pals_time_t cur, uint64_t period);

pals_time_t add_time_ns(pals_time_t t, uint64_t ns);

uint64_t get_sec(uint64_t t);

uint64_t get_nanosec(uint64_t t);

void get_pals_time(pals_time_t *ptime);

int compare_pals_time(pals_time_t fst, pals_time_t snd);

pals_time_t hton_pals_time(pals_time_t ptime);

pals_time_t ntoh_pals_time(pals_time_t ptime);
 
uint32_t get_lcm_period(uint64_t p1, uint64_t p2);

#endif /* PALS__PALS_TIME_H */
