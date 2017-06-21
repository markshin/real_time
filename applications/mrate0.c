// example app code
#include "../app.h"
#include <stdio.h>
#include <string.h>

void phase1() {
  printf("\nMRATE0 : welcome to phase1\n");
  int conn_id0 = 3, conn_id1 = 4;
  char msginit[10] = "none";
  char buf0[20];
  memcpy(buf0, msginit, sizeof(char[10]));
  char msg0[10] = "0 to 1";

  int send_hb0 = pals_send(conn_id0, msg0, MAX_MSG_BYTESIZE);
  int recv_hb0 = pals_recv(conn_id1, &buf0, 20);

  printf("phase 1 of task0 : recv_heart_beat of task1 : %s success : %d \n", buf0, recv_hb0);
  printf("phase 1 of task0 : send_heart_beat to task1 : %s success : %d \n", msg0, send_hb0);                                                                                     

}

void phase2() {
  printf("\nMRATE0 : no works in phase 2\n");
}


void pals_set_init_data(task_init_t *init_data) {
  init_data->task_id = 0;
  init_data->num_phases = 2;

  init_data->phase_func[0] = phase1;
  init_data->phase_func[1] = phase2;
}
