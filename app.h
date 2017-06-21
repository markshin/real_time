#ifndef PALS__APP_H
#define PALS__APP_H

#include "pals_base.h"
#include <stdint.h>

typedef struct _task_init {
  uint32_t task_id;
  int num_phases;
  void (*phase_func[MAX_NUM_PHASES]) (void);
} task_init_t;

int pals_send(uint32_t conn_id, void *msg, uint32_t size);
int pals_recv(uint32_t conn_id, void *msg, uint32_t size);

void pals_set_init_data(task_init_t *init_data);

#endif /* PALS__APP_H */
