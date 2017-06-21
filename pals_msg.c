#include "pals_msg.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int read_msg_by_seq(pals_msg_buf_info_t *msgs_info, uint32_t seq_n, void *buf, uint32_t size) {
  int i = 0;
  int found = -1;
  for (i = 0; i < msgs_info->num_msg_buf; ++i) {
    if (msgs_info->msg_buf[i].seq_num == seq_n) {
      memcpy(buf, msgs_info->msg_buf[i].msg_content, size);
      msgs_info->msg_buf[i].is_read = 1;
      found = 1;
      break;
    }
  }
  return found;
}
