// example app code
#include "../app.h"
#include <stdio.h>
#include <string.h>

void phase1() {

}

void phase2() {
  printf("welcome to phase2\n");
  int conn_id0 = 3, conn_id1 = 4;
  char buf[20];
  char buf2[20];
  char msg[10] = "pineapple";
  int send_hb = pals_send(conn_id0, msg, 20);
  int recv_hb = pals_recv(conn_id1, &buf, 20);
  int recv_hb2 = pals_recv(conn_id1, &buf2, 20);
   
  printf("phase 2 of task0 : recv_heart_beat of task1 : %s success : %d \n", (char*)&buf, recv_hb);
  printf("phase 2 of task0 : recv_heart_beat of task1 : %s success : %d \n", (char*)&buf2, recv_hb);
  printf("phase 2 of task0 : send_heart_beat of task0 : %s success : %d \n", msg, send_hb);

}

void pals_set_init_data(task_init_t *init_data) {
  init_data->task_id = 3;
  init_data->num_phases = 2;

  init_data->phase_func[0] = phase1;
  init_data->phase_func[1] = phase2;
}
