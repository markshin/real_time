// example app code
#include "../app.h"
#include "../pals_task.h"
#include <stdio.h>

void phase1() {
  
  printf("phase 1 base_time = {sec : %llu, nano : %llu} phase_time = {sec : %llu, nano : %llu}",  
    task.state.pals_base_time.sec, task.state.pals_base_time.nanosec, task.state.pals_phase_base_time.sec, task.state.pals_phase_base_time.nanosec);

}

void phase2() {

  printf("phase 2 base_time = {sec : %llu, nano : %llu} phase_time = {sec : %llu, nano : %llu}",
    task.state.pals_base_time.sec, task.state.pals_base_time.nanosec, task.state.pals_phase_base_time.sec, task.state.pals_phase_base_time.nanosec);
  
}

void pals_set_init_data(task_init_t *init_data) {
  init_data->task_id = 0;
  init_data->num_phases = 2;

  init_data->phase_func[0] = phase1;
  init_data->phase_func[1] = phase2;
}
