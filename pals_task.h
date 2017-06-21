#ifndef PALS__PALS_TASK_H
#define PALS__PALS_TASK_H

#include "pals_base.h"
#include "pals_time.h"
#include "pals_port.h"
#include "pals_msg.h"
#include <stdint.h>

typedef struct _phase {
  uint32_t start_time;
  void (*run)();
} phase_t;

typedef struct _task_state {
  pals_time_t pals_base_time;
  pals_time_t pals_phase_base_time;
  uint32_t cur_phase;

} task_state_t;

typedef struct _task {
  // config
  uint32_t task_id;
  uint32_t group_id;

  uint64_t period;

  // phases
  uint32_t num_phases;
  phase_t phases[MAX_NUM_PHASES];

  // socket
  int socket_tx;
  int socket_rx;
  int socket_mcast_tx;
  int socket_mcast_rx;

  // ports
  tx_pals_port_set_t tx_port_set;
  rx_pals_port_set_t rx_port_set;
  // state
  task_state_t state;
} task_t;

// Global variable
task_t task;

#endif /* PALS__PALS_TASK_H */
