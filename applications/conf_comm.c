#include "../conf.h"

// TODO: define example taskgroups, tasks, and conns
pals_config_t config_data = {
  .taskgroups = {
    { .group_id = 0, .period = 1000000000L, .num_phases = 2, .phases = {0, 400000000} }
  },
  .tasks = {
    { .task_id = 3, .group_id = 0, .ipaddr = "127.0.0.1", .port = 4321 },
    { .task_id = 4, .group_id = 0, .ipaddr = "127.0.0.1", .port = 4322 },
  },
  .conns = {
    { .conn_id = 3, .sender_task_id = 3, .num_recv_tasks = 1, .receiver_task_ids = (uint32_t [1]){4} },
    { .conn_id = 4, .sender_task_id = 4, .num_recv_tasks = 1, .receiver_task_ids = (uint32_t [1]){3} }
  },
  .num_taskgroups = 1,
  .num_tasks = 2,
  .num_conns = 2,
  .mcast_ipaddr = "226.1.1.1",
  .mcast_port = 4511
};

void pals_config_init(pals_config_t *conf) {
  *conf = config_data;
}
