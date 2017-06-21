#include "conf.h"
#include "pals_time.h"

#include <stdint.h>
#include <stdlib.h>

conf_taskgroup_t *get_conf_taskgroup(pals_config_t *conf, uint32_t gid) {
  int i;
  for (i = 0; i < conf->num_taskgroups; ++i)
    if (conf->taskgroups[i].group_id == gid)
      return &conf->taskgroups[i];

  return NULL;
}

conf_task_t *get_conf_task(pals_config_t *conf, uint32_t tid) {
  int i;
  for (i = 0; i < conf->num_tasks; ++i)
    if (conf->tasks[i].task_id == tid)
      return &conf->tasks[i];

  return NULL;
}

// Returns 0 for multi-phase
uint64_t pals_period_conn(pals_config_t *conf, conf_conn_t *conn) {
  uint32_t sender_gid = get_conf_task(conf, conn->sender_task_id)->group_id;
  conf_taskgroup_t *sender_group = get_conf_taskgroup(conf, sender_gid);
  uint64_t pals_period = sender_group->period;

  int i;
  int is_same_group = 1;
  for (i = 0; i < conn->num_recv_tasks; ++i) {
    uint32_t recv_gid = get_conf_task(conf, conn->receiver_task_ids[i])->group_id;
    conf_taskgroup_t *recv_group = get_conf_taskgroup(conf, recv_gid);

    if (recv_gid != sender_gid) is_same_group = 0;
    pals_period = get_lcm_period(pals_period, recv_group->period);
  }

  if (is_same_group > 0) pals_period = 0;

  return pals_period;
}

uint8_t is_task_receiver(conf_conn_t *conn, uint32_t tid) {
  int i;
  for (i = 0; i < conn->num_recv_tasks; ++i)
    if (conn->receiver_task_ids[i] == tid) return 1;

  return 0;
}
