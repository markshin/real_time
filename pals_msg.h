#ifndef PALS__PALS_MSG_H
#define PALS__PALS_MSG_H

#include "pals_base.h"
#include "pals_time.h"

typedef struct _pals_msg {
  uint32_t conn_id;
  uint32_t sender_task_id;
  pals_time_t recv_time;
  pals_time_t send_time;
  uint8_t is_read; // 0 : not read, 1: read
  uint32_t seq_num; // sequence number: starts from 0
  char msg_content[MAX_MSG_BYTESIZE];
} pals_msg_t;


typedef struct _pals_msg_buf_info {
  uint32_t num_msg_buf;
  pals_msg_t msg_buf[MAX_MSG_QUEUE_SIZE];
} pals_msg_buf_info_t;

int read_msg_by_seq(pals_msg_buf_info_t *msgs_info, uint32_t seq_n, void *buf, uint32_t size);

#endif /* PALS__PALS_MSG_H */
