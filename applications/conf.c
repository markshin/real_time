#include "../conf.h"

// TODO: define example taskgroups, tasks, and conns
pals_config_t config_data = {
  .taskgroups = {
    { .group_id = 0, .period = 100000L, .num_phases = 2, .phases = {0, 40000} }
  },
  .tasks = {
    { .task_id = 0, .group_id = 0, .ipaddr = "192.168.0.10", .port = 9950 },
//    { .task_id = 1, .group_id = 0, .ipaddr = "192.168.0.11", .port = 9950 },
  },
  .conns = {
//    { .conn_id = 1, .sender_task_id = 0, .receiver_task_id = 1},
//    { .conn_id = 2, .sender_task_id = 1, .receiver_task_id = 0}
  },
  .num_taskgroups = 1,
  .num_tasks = 1,
  .num_conns = 0,
  .mcast_ipaddr = "226.1.1.1",
  .mcast_port = 4511
};

void pals_config_init(pals_config_t *conf) {
  *conf = config_data;
}
