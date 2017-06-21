#include "pals_socket.h"
#include "pals_task.h"
#include "pals_timer.h"
#include "pals_read_msg.h"
#include "conf.h"
#include "app.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

// extern task_t task;

int task_init(pals_config_t *conf, conf_task_t *conf_task, conf_taskgroup_t *conf_taskgroup, task_init_t idata) {
  int i;

  task.task_id = conf_task->task_id;
  task.group_id = conf_taskgroup->group_id;
  task.period = conf_taskgroup->period;
  task.num_phases = conf_taskgroup->num_phases;

  for (i = 0; i < task.num_phases; ++i) {
    task.phases[i].start_time = conf_taskgroup->phases[i];
    task.phases[i].run = idata.phase_func[i];
  }

  task.tx_port_set.num_ports = 0;
  task.rx_port_set.num_ports = 0;

  // socket
  // YH : Check return value of task open function in function code(?)
  if ((task.socket_tx = pals_open_tx_socket()) < 0) {
    // socket open error
    return -2;
  }
  if ((task.socket_rx = pals_open_rx_socket(conf_task->ipaddr, conf_task->port)) < 0) {
    // socket open error
    return -2;
  }
  
  return 0;
}

void conn_init(pals_config_t *conf) {
  int i;
  // pals_ports
  conf_task_t *tmp_conf_task;
  uint64_t pals_period;

  for (i = 0; i < conf->num_conns; ++i) {
    conf_conn_t *conn = &conf->conns[i];
    if (conn->sender_task_id == task.task_id) {
      // open tx port

      char *tx_ipaddr = NULL;
      int tx_port = 0;
      pals_period = pals_period_conn(conf, conn);


	    tmp_conf_task = get_conf_task(conf, conn->receiver_task_ids[0]);
    	tx_ipaddr = tmp_conf_task->ipaddr;
	    tx_port = tmp_conf_task->port;

      printf("open tx_port: conn %d, period: %lu\n", conn->conn_id, pals_period);
      insert_tx_port(&task.tx_port_set,
		     conf->conns[i].conn_id,
		     conf->conns[i].num_recv_tasks,
		     (pals_period == 0),
		     pals_period,
		     tx_ipaddr,
		     tx_port);
    } else if (is_task_receiver(conn, task.task_id) > 0) {
      // open rx port
      pals_period = pals_period_conn(conf, conn);
      printf("open rx_port: conn %d, period: %lu\n", conn->conn_id, pals_period);
      insert_rx_port(&task.rx_port_set,
		     conn->conn_id,
		     conn->sender_task_id,
		     (pals_period == 0),
		     pals_period);
    }
  }
}

int pals_init(pals_config_t *conf) {
  conf_taskgroup_t *conf_taskgroup;
  conf_task_t *conf_task;
  int ret = 0;

  task_init_t idata;
  pals_config_init(conf);

  // TODO: do we need sanity check for conf?
  // such as
  // 1) uniqueness of taskgroups & tasks & conns
  // 2) each task belongs to one taskgroup
  // 3) phases at least 1
  // 4) conn sender_task_id and receiver_task_id exist
  // if (check_conf(conf) < 0) return -1;

  //YH : In my opinion, we can divide this pals_init function to several functions like set_task, set ports.... 
  pals_set_init_data(&idata);

  // task info
  conf_task = get_conf_task(conf, idata.task_id);
  conf_taskgroup = get_conf_taskgroup(conf, conf_task->group_id);
  // phases_check
  if (idata.num_phases != conf_taskgroup->num_phases) {
    // initialization error
    return -1;
  }
  //task 
  ret = task_init(conf, conf_task, conf_taskgroup, idata);
  if (ret == -2)
    return ret;
  //conn
  conn_init(conf);

  return ret;
}

int main() {
  int i;
  // TODO: is conf needed here? doesn't task include all info?
  pals_config_t conf;
  pals_time_t initial_time;
  int read_ret, mcast_read_ret;
  phase_t p;
  
  if (pals_init(&conf) < 0) {
    // init failed
    // TODO: print err msg
    exit(0);
  }
  // use timerfd and task.period
  pals_timer_t task_timer, phase_timer;
  pals_init_timer(&task_timer);
  pals_init_timer(&phase_timer);

  get_pals_time(&initial_time);
  initial_time = add_time_ns(initial_time, MAX(1000000000L, task.period));
  task.state.pals_base_time = get_base_time(initial_time, task.period);
  pals_set_timer(&task_timer, task.state.pals_base_time, task.period);
  // flush socket_rx_buffer
  pals_flush_rx_buffer(task.socket_rx);
  pals_flush_rx_buffer(task.socket_mcast_rx);
  
  while (1) {
    pals_wait_timer(&task_timer);
    for (i = 0; i < task.num_phases; ++i) {
      p = task.phases[i];
      pals_set_timer(&phase_timer, add_time_ns(task.state.pals_base_time, p.start_time), 0);
      pals_wait_timer(&phase_timer);
      task.state.pals_phase_base_time = add_time_ns(task.state.pals_base_time, p.start_time);
      task.state.cur_phase = i;
      pals_msg_buffer_swap(&task);
      read_ret = pals_read_socket(task.socket_rx, &task);
      mcast_read_ret = pals_read_socket(task.socket_mcast_rx, &task);
      if ((read_ret < 0) || (mcast_read_ret < 0)) {
	// TODO: print err msg
	// TODO: handle errors. now just exit
	printf("socket read fail\n");
	exit(0);
      }
      p.run();
    }
    task.state.pals_base_time = add_time_ns(task.state.pals_base_time, task.period);
  }
}
