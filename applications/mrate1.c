// example app code
#include "../app.h"
#include "../pals_task.h"
#include <stdio.h>
#include <string.h>

void phase1() {
  printf("welcome to phase1 of task1\n");
  int conn_id0 = 3, conn_id1 = 4;
  char msginit[10] = "none";
  char buf[20];
  memcpy(buf, msginit, sizeof(char[10]));
  char msg[10] = "1 to 0";
  int send_hb = pals_send(conn_id1, msg, MAX_MSG_BYTESIZE);
  int recv_hb = pals_recv(conn_id0, &buf, 20);
  
  printf("phase 1 of task1 : recv_heart_beat of task1 : %s success : %d \n", &buf, recv_hb);
  printf("phase 1 of task1 : send_heart_beat of task0 : %s success : %d \n", msg, send_hb);

}



void pals_set_init_data(task_init_t *init_data) {
  init_data->task_id = 1;
  init_data->num_phases = 1;

  init_data->phase_func[0] = phase1;
}
