#ifndef PALS__PALS_PORT_H
#define PALS__PALS_PORT_H

#include "pals_base.h"
#include "pals_msg.h"
#include <stdint.h>

typedef struct _tx_pals_port {
  uint32_t conn_id;

  uint8_t multi_phase;
  uint64_t pals_period;

  char ipaddr[16];
  int port;

  int num_recv_tasks;
  uint32_t next_seq_num;
} tx_pals_port_t;

typedef struct _rx_pals_port {
  uint32_t conn_id;
  uint32_t send_task_id;
  pals_msg_buf_info_t cur_msgs_info;
  pals_msg_buf_info_t nxt_msgs_info;
  // required?
  // TODO : is_multi_phase flag needed like tx_pals_port's multi_phase
  uint8_t multi_phase;
  uint64_t pals_period;
  /* char ipaddr[16]; */
  /* int port; */
  uint32_t next_seq_num;
} rx_pals_port_t;

typedef struct _tx_pals_port_set {
  uint32_t num_ports;
  tx_pals_port_t ports[MAX_NUM_SIMPLEX_CONN];
} tx_pals_port_set_t;

typedef struct _rx_pals_port_set {
  uint32_t num_ports;
  rx_pals_port_t ports[MAX_NUM_SIMPLEX_CONN];
} rx_pals_port_set_t;

tx_pals_port_t *get_tx_port(tx_pals_port_set_t *tx_port_set, uint32_t cid);
rx_pals_port_t *get_rx_port(rx_pals_port_set_t *rx_port_set, uint32_t cid);

int insert_tx_port(tx_pals_port_set_t *tx_port_set,
		   uint32_t cid, int num_recv_tasks, uint8_t mp,
		   uint64_t period, char *ipaddr, int port);

int insert_rx_port(rx_pals_port_set_t *rx_port_set,
		   uint32_t cid, uint32_t send_tid, uint8_t mp, uint64_t period);


int recv_msg_rx_port(rx_pals_port_t *rx_port, pals_msg_t *msg);

int read_msg_from_port(rx_pals_port_t* rx_port, void *buf, uint32_t size);

#endif /* PALS__PALS_PORT_H */
