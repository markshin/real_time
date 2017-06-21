#include "pals_timer.h"

#include "pals_time.h"
#include <sys/timerfd.h>
#include <unistd.h>
#include <stdint.h>

// TODO: fill in here

void pals_init_timer(pals_timer_t *timer) {
  timer->id = timerfd_create(CLOCK_REALTIME, 0);
}

void pals_set_timer(pals_timer_t *timer, pals_time_t initial_time, uint64_t period) {

  struct itimerspec setting_val;

  setting_val.it_value.tv_sec = initial_time.sec;
  setting_val.it_value.tv_nsec = initial_time.nanosec;

  setting_val.it_interval.tv_sec = get_sec(period);
  setting_val.it_interval.tv_nsec = get_nanosec(period);

  timerfd_settime(timer->id, TFD_TIMER_ABSTIME, &setting_val, NULL);
  //Question: return _return_value_ of timerfd_settime?? 
}


int pals_wait_timer(pals_timer_t *timer) {
  uint64_t spended_period;
  read(timer->id, &spended_period, sizeof(spended_period));
  //Question: check read function's return value??
  return (int)spended_period - 1;
}
