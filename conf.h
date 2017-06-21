#ifndef PALS__CONF_H
#define PALS__CONF_H

#include "pals_base.h"
#include <stdint.h>

typedef struct _conf_taskgroup {
  uint32_t group_id;
  uint64_t period; // in nanoseconds, is 32-bit OK?
  uint32_t num_phases;
  uint32_t phases[MAX_NUM_PHASES]; // is always the first elem 0? = [3, 13, 25, 37]
} conf_taskgroup_t;

typedef struct _conf_task {
  uint32_t task_id;
  uint32_t group_id;
  char ipaddr[16];
  int port;
} conf_task_t;

typedef struct _conf_conn {
  uint32_t conn_id;
  uint32_t sender_task_id;
  int num_recv_tasks;
  uint32_t* receiver_task_ids;
} conf_conn_t;

typedef struct _pals_config {
  uint32_t num_taskgroups;
  conf_taskgroup_t taskgroups[MAX_NUM_TASKGROUPS];
  
  uint32_t num_tasks;
  conf_task_t tasks[MAX_NUM_TASKS];

  uint32_t num_conns;
  conf_conn_t conns[MAX_NUM_CONNS];
  // YH : make another data struct for mcast ??
  char mcast_ipaddr[16];
  int mcast_port;
  int mcast_timelive;

} pals_config_t;

void pals_config_init(pals_config_t *conf);

// accessor functions
conf_taskgroup_t *get_conf_taskgroup(pals_config_t *conf, uint32_t gid);
conf_task_t *get_conf_task(pals_config_t *conf, uint32_t tid);

// helpers
uint64_t pals_period_conn(pals_config_t *conf, conf_conn_t *conn);

uint8_t is_task_receiver(conf_conn_t *conn, uint32_t tid);


#endif /* PALS__CONF_H*/
