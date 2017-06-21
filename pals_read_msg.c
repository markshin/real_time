#include "pals_read_msg.h"
#include "pals_time.h"
#include "pals_socket.h"
#include "pals_port.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

//read from tmp buffer and move it to our task buffer
//buffer

//there is socket
// socket -> tmp buffer one by one
// check in buffer that this is for current period
// if it is move to task buffer
// else keep in tmp buffer
//in next period check packet in the tmp buffer is for current period
// if it is move to task buffer
// if checking is finished, then read from socket one by one
// and do the same thing above

static void swap_msg_buffer(pals_msg_buf_info_t *cur_msg_info, pals_msg_buf_info_t *nxt_msg_info) {

  pals_msg_buf_info_t temp_info;

  temp_info = *cur_msg_info;
  *cur_msg_info = *nxt_msg_info;
  *nxt_msg_info = temp_info;

}

void pals_msg_buffer_swap(task_t *task) {
  // swap cur_msg buffer and nxt_msg buffer
  // YH TODO : Calculate swap timing. multi phase : every time. multi rate : each hyper period start
  int i;
  pals_time_t fit_time;
  for (i = 0; i < task->tx_port_set.num_ports; i ++) {
    if (task->tx_port_set.ports[i].multi_phase == 0) {
      fit_time = get_base_time(task->state.pals_base_time, task->tx_port_set.ports[i].pals_period);
      if (compare_pals_time(task->state.pals_phase_base_time, fit_time) == 0) {
	task->tx_port_set.ports[i].next_seq_num = 0;
      }
    } else {
      task->tx_port_set.ports[i].next_seq_num = 0;
    }
  }

  for (i = 0; i < task->rx_port_set.num_ports; i ++) {
    if (task->rx_port_set.ports[i].multi_phase == 0) {
      // multi casting or multi rate
      fit_time = get_base_time(task->state.pals_base_time, task->rx_port_set.ports[i].pals_period);
      if (compare_pals_time(task->state.pals_phase_base_time, fit_time) == 0) {
	// every hyper-period, swap buffer
	printf("%d-th multi-rate buffer swap!!! \n", i);
	swap_msg_buffer(&task->rx_port_set.ports[i].cur_msgs_info, &task->rx_port_set.ports[i].nxt_msgs_info);
	task->rx_port_set.ports[i].next_seq_num = 0;
	task->rx_port_set.ports[i].nxt_msgs_info.num_msg_buf = 0;
      } else {
	continue;
      }
    } else if (task->rx_port_set.ports[i].multi_phase == 1) {
      // multi phase 
      printf("%d-th multi-phase buffer swap!!! \n", i);
      swap_msg_buffer(&task->rx_port_set.ports[i].cur_msgs_info, &task->rx_port_set.ports[i].nxt_msgs_info);
      task->rx_port_set.ports[i].next_seq_num = 0;
      task->rx_port_set.ports[i].nxt_msgs_info.num_msg_buf = 0;
    }
  }
}

static int pals_recv_msg(int sock, pals_msg_t *msg) {

  size_t buffer_length = sizeof(pals_msg_t);
  
  int len;

  len = pals_socket_recvfrom(sock, msg, buffer_length);

  //change byte order
  msg->conn_id = ntohl(msg->conn_id);
  msg->sender_task_id = ntohl(msg->sender_task_id);
  msg->recv_time = ntoh_pals_time(msg->recv_time);
  msg->send_time = ntoh_pals_time(msg->send_time);
  msg->is_read = 0;

  if (len >= 0)
    printf("##### pals_recv_msg: %d\n", len);

  return len;
}

int pals_read_socket(int sock, task_t *task) {
  int how_many = 0;
  //int move_counter;
  //int current_length;
  pals_msg_t recved_msg;
  rx_pals_port_t *rx_port;
  //current_length = 0;
  //move_counter = 0;

  // bring msg from socket one by one to buffer : make function later
  while(pals_recv_msg(sock, &recved_msg) >= 0) {
    how_many ++;
    rx_port = get_rx_port(&task->rx_port_set, recved_msg.conn_id);
    if (rx_port == NULL) continue; // multicasted, but not for me
    if (recv_msg_rx_port(rx_port, &recved_msg) < 0) {
      return -1;
    }
  }
  printf("how many new msgs arrived? : %d\n", how_many);

  return 0; 
}

void pals_flush_rx_buffer(int sock) {
  pals_msg_t flushed_msg;  
  while(pals_recv_msg(sock, &flushed_msg) >= 0) {
    continue;
  }

}
