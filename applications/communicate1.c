// example app code
#include "../app.h"
#include <stdio.h>
#include <string.h>

void phase1() {
  printf("welcome to phase1\n");
  int conn_id0 = 3, conn_id1 = 4;
  char buf[20];
  char msg[10] ="apple";
  char msg2[10] = "orange";
  int send_hb = pals_send(conn_id1, msg, 7);
  int recv_hb = pals_recv(conn_id0, &buf, 20);
  int send_hb2 = pals_send(conn_id1, msg2, 7);
  
  printf("phase 1 of task1 : recv_heart_beat of task1 : %s success : %d \n", (char*)&buf, recv_hb);
  printf("phase 1 of task1 : send_heart_beat of task0 : %s success : %d \n", msg, send_hb);
  printf("phase 1 of task1 : send_heart_beat of task0 : %s success : %d \n", msg2, send_hb);

}

void phase2() {
  printf("no works in phase 2\n");
}


void pals_set_init_data(task_init_t *init_data) {
  init_data->task_id = 4;
  init_data->num_phases = 2;

  init_data->phase_func[0] = phase1;
  init_data->phase_func[1] = phase2;
}
