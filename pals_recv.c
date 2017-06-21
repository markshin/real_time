#include "app.h"

#include "pals_task.h"
#include "pals_read_msg.h"
#include "pals_port.h"
#include "pals_time.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

int pals_recv(uint32_t conn_id, void *msg, uint32_t size) {
  // pop msg from the received msgs of task
  // copy data to msg (using memcpy?)
  // return -1 if failed
  // is it needs conn_id?? need for safe??
  int i;
  rx_pals_port_t *rx_port;
  if (MAX_MSG_BYTESIZE < size) 
    return -2;

  rx_port = get_rx_port(&task.rx_port_set, conn_id);
  
  int ret = read_msg_from_port(rx_port, msg, size);
  if (ret < 0)
    return -1; // message not found

  printf("num_cur_buf : %d \n", rx_port->cur_msgs_info.num_msg_buf);

  return size;
  
}
