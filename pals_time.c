#include "pals_time.h"

#include <stdint.h>
#include <time.h>
#include <byteswap.h>
#include <arpa/inet.h>
#include <stdio.h>

pals_time_t add_time_ns(pals_time_t t, uint64_t ns) {
  pals_time_t added_t;
  
  added_t.sec = t.sec + (ns / 1000000000L);
  added_t.nanosec = t.nanosec + (ns % 1000000000L);

  if (added_t.nanosec >= 1000000000UL) {
    added_t.sec = added_t.sec + 1;
    added_t.nanosec = added_t.nanosec - 1000000000L;
    
  }

  return added_t;
}

uint64_t get_sec(uint64_t t) {
  return t / ((long) 1000000000);
}

uint64_t get_nanosec(uint64_t t) {
  return t % ((long) 1000000000);
}

void get_pals_time(pals_time_t *ptime) {

  struct timespec tspec;
  clock_gettime(CLOCK_REALTIME, &tspec);
  ptime->sec = tspec.tv_sec;
  ptime->nanosec = tspec.tv_nsec;
}

pals_time_t get_base_time(pals_time_t cur, uint64_t period) {
  
  pals_time_t base_time;
  uint64_t remainder; 
  remainder = ((cur.sec * 1000000000L) + cur.nanosec) % period;

  base_time.sec = cur.sec - (remainder / 1000000000L);
  base_time.nanosec = cur.nanosec - (remainder%1000000000L);
  
  if (base_time.nanosec > 1000000000L) {
    base_time.sec = base_time.sec - 1;
    base_time.nanosec = base_time.nanosec + 1000000000L;
  }
  
  return base_time;
}

int compare_pals_time(pals_time_t fst, pals_time_t snd) {
  uint64_t fst_pals_time;
  uint64_t snd_pals_time;

  fst_pals_time = (fst.sec * 1000000000L) + fst.nanosec;
  snd_pals_time = (snd.sec * 1000000000L) + snd.nanosec;

  if (fst_pals_time > snd_pals_time) {
    // fst_pals_time is future
    return 1;
  } else if (fst_pals_time < snd_pals_time) {
    // fst_pals_time is past
    return -1;
  } else if (fst_pals_time == snd_pals_time) {
    // same
    return 0;
  }
}

pals_time_t hton_pals_time(pals_time_t ptime) {
  pals_time_t ntime;
  // TODO: find or implement htonl for 64bit
  ntime.sec = bswap_64(ptime.sec);
  ntime.nanosec = bswap_64(ptime.nanosec);

  return ntime;
}

pals_time_t ntoh_pals_time(pals_time_t ptime) {
  pals_time_t htime;
  htime.sec = bswap_64(ptime.sec);
  htime.nanosec = bswap_64(ptime.nanosec);

  return htime;
}
uint32_t _gcd(uint64_t p1, uint64_t p2) {
  if (p2 == 0)
    return p1;
  else
    return _gcd(p2, p1%p2);
}

uint32_t get_lcm_period(uint64_t p1, uint64_t p2) {
  uint32_t gcd = _gcd(p1, p2);
  return (p1 / gcd) * p2;
}
