#ifndef PALS__PALS_READ_MSG_H
#define PALS__PALS_READ_MSG_H

#include "pals_task.h"

void pals_msg_buffer_swap(task_t *task);
int pals_read_socket(int sock, task_t *task);
void pals_flush_rx_buffer(int sock);
#endif /* PALS__PALS_READ_MSG_H */
