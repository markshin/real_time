#include "pals_port.h"
#include "pals_time.h"
#include "pals_base.h"
#include "pals_task.h"
#include "pals_msg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

tx_pals_port_t *get_tx_port(tx_pals_port_set_t *tx_port_set, uint32_t cid) {
  int i;

  for (i = 0; i < tx_port_set->num_ports; ++i) {
    if (tx_port_set->ports[i].conn_id == cid)
      return (&tx_port_set->ports[i]);
  }

  return NULL;
}

rx_pals_port_t *get_rx_port(rx_pals_port_set_t *rx_port_set, uint32_t cid) {
  int i;

  for (i = 0; i < rx_port_set->num_ports; ++i) {
    if (rx_port_set->ports[i].conn_id == cid)
      return (&rx_port_set->ports[i]);
  }

  return NULL;
}

int insert_tx_port(tx_pals_port_set_t *tx_port_set,
		   uint32_t cid, int num_recv_tasks, uint8_t mp,
		   uint64_t period, char *ipaddr, int port) {
  tx_pals_port_t *pals_port = &tx_port_set->ports[tx_port_set->num_ports];
  
  if (tx_port_set->num_ports + 1 > MAX_NUM_SIMPLEX_CONN)
    return -1;
 
  pals_port->conn_id = cid;
  pals_port->num_recv_tasks = num_recv_tasks;
  strncpy(pals_port->ipaddr, ipaddr, 16);
  pals_port->port = port;

  pals_port->multi_phase = mp;
  pals_port->pals_period = period;

  ++tx_port_set->num_ports;

  return 0;
}

int insert_rx_port(rx_pals_port_set_t *rx_port_set,
		   uint32_t cid, uint32_t send_tid, uint8_t mp, uint64_t period) {
  rx_pals_port_t *pals_port = &rx_port_set->ports[rx_port_set->num_ports];

  if (rx_port_set->num_ports + 1 > MAX_NUM_SIMPLEX_CONN)
    return -1;

  pals_port->conn_id = cid;
  pals_port->send_task_id = send_tid;
  pals_port->cur_msgs_info.num_msg_buf = 0;
  pals_port->nxt_msgs_info.num_msg_buf = 0;
  
  pals_port->multi_phase = mp;
  pals_port->pals_period = period;
  ++rx_port_set->num_ports;
  
  return 0;
}

int recv_msg_rx_port(rx_pals_port_t *rx_port, pals_msg_t *msg) {
  int compare_recv_time;
  int compare_early_time;
  pals_time_t port_base_time;
  pals_time_t early_arrival;

  if (rx_port->multi_phase == 0) {
    port_base_time = get_base_time(task.state.pals_base_time, rx_port->pals_period);
    early_arrival = add_time_ns(port_base_time, rx_port->pals_period);
  } else if (rx_port->multi_phase == 1) {
    port_base_time = task.state.pals_phase_base_time;
    if (task.state.cur_phase + 1 < task.num_phases) {
      early_arrival = add_time_ns(task.state.pals_base_time, task.phases[task.state.cur_phase + 1].start_time);
    } else if (task.state.cur_phase + 1 == task.num_phases) {
      early_arrival = add_time_ns(task.state.pals_base_time, task.period);
    }  
  }

  compare_recv_time = compare_pals_time(msg->recv_time, port_base_time);
  compare_early_time = compare_pals_time(msg->recv_time, early_arrival);

  if (compare_recv_time == 0) {
    memcpy(&rx_port->cur_msgs_info.msg_buf[rx_port->cur_msgs_info.num_msg_buf], msg, sizeof(pals_msg_t));
    rx_port->cur_msgs_info.num_msg_buf ++;
    printf("fit-time msg recv_time sec = %lu, nanosec = %lu\n", msg->recv_time.sec, msg->recv_time.nanosec);
  } else if (compare_early_time == 0) {
    memcpy(&rx_port->nxt_msgs_info.msg_buf[rx_port->nxt_msgs_info.num_msg_buf], msg, sizeof(pals_msg_t));
    rx_port->nxt_msgs_info.num_msg_buf ++;
    printf("early msg recv_time sec = %lu, nanosec = %lu\n", msg->recv_time.sec, msg->recv_time.nanosec);
  } else {
    printf("ERROR: msg arrived at wrong time. fit-time: %lu %lu, early: %lu %lu, target: %lu %lu\n",
	   port_base_time.sec, port_base_time.nanosec, early_arrival.sec, early_arrival.nanosec,
	   msg->recv_time.sec, msg->recv_time.nanosec);
    return -1;
  }

  return 0;
}

int read_msg_from_port(rx_pals_port_t* rx_port, void *buf, uint32_t size) {
  int ret = read_msg_by_seq(&rx_port->cur_msgs_info, rx_port->next_seq_num, buf, size);
  rx_port->next_seq_num += 1;

  return ret;
}
