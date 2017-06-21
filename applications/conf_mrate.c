#include "../conf.h"

// TODO: define example taskgroups, tasks, and conns
pals_config_t config_data = {
  .taskgroups = {
    { .group_id = 0, .period = 700000000L, .num_phases = 2, .phases = {0, 40000000} },
    { .group_id = 1, .period = 200000000L, .num_phases = 1, .phases = {0} },
    { .group_id = 2, .period = 100000000L, .num_phases = 1, .phases = {0} }
  },
  .tasks = {
    { .task_id = 0, .group_id = 0, .ipaddr = "127.0.0.1", .port = 4321 },
    { .task_id = 1, .group_id = 1, .ipaddr = "127.0.0.1", .port = 4322 },
    { .task_id = 2, .group_id = 2, .ipaddr = "127.0.0.1", .port = 4323 }
  },
  .conns = {
    { .conn_id = 3, .sender_task_id = 0, .num_recv_tasks = 1, .receiver_task_ids = (uint32_t [1]){1} },
    { .conn_id = 4, .sender_task_id = 1, .num_recv_tasks = 1, .receiver_task_ids = (uint32_t [1]){0} },
    { .conn_id = 5, .sender_task_id = 0, .num_recv_tasks = 1, .receiver_task_ids = (uint32_t [1]){2} },
    { .conn_id = 6, .sender_task_id = 2, .num_recv_tasks = 1, .receiver_task_ids = (uint32_t [1]){0} }
  },
  .num_taskgroups = 3,
  .num_tasks = 3,
  .num_conns = 4,
  .mcast_ipaddr = "226.1.1.1",
  .mcast_port = 4511
};

void pals_config_init(pals_config_t *conf) {
  *conf = config_data;
}
