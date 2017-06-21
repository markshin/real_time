#include "app.h"
#include "pals_task.h"
#include "pals_port.h"
#include "pals_socket.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

// TODO: include more
// extern task_t task;

int pals_send(uint32_t conn_id, void *msg, uint32_t size) {
  // TODO:
  // find tx_port of conn_id from tx_ports of task
  // make pals_msg and send via socket_tx
  // return -1 if failed?

  // static?
  pals_msg_t pals_msg;

 // int i;

  tx_pals_port_t *tx_port = get_tx_port(&task.tx_port_set, conn_id);
  // TODO: abort when tx_port = NULL;

  if (size > MAX_MSG_BYTESIZE)
    return -1;

  memcpy(pals_msg.msg_content, msg, size);

  pals_msg.conn_id = htonl(conn_id);
  pals_msg.sender_task_id = htonl(task.task_id); // unchanged
  pals_msg.send_time = hton_pals_time(task.state.pals_phase_base_time);
  pals_msg.seq_num = tx_port->next_seq_num;
  tx_port->next_seq_num += 1;

  pals_time_t recv_time;
  if (tx_port->multi_phase > 0) {
    // recv_time = next phase start time
    if (task.state.cur_phase + 1 < task.num_phases)
      recv_time = add_time_ns(task.state.pals_base_time, task.phases[task.state.cur_phase + 1].start_time);
    else if (task.state.cur_phase + 1 == task.num_phases)
      recv_time = add_time_ns(task.state.pals_base_time, task.period);
    else
      // error
      return -1;
  } else {
    // recv_time = pals_period of multirate
    recv_time = add_time_ns(get_base_time(task.state.pals_base_time, tx_port->pals_period), tx_port->pals_period);
  }
  pals_msg.recv_time = hton_pals_time(recv_time);
    // uni-casting
    printf("send\n");
    return pals_socket_sendto(task.socket_tx, &pals_msg, sizeof(pals_msg), tx_port->ipaddr, tx_port->port);
}
