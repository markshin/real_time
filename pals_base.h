#ifndef PALS__PALS_BASE_H
#define PALS__PALS_BASE_H

#define MAX_NUM_TASKGROUPS 32 // 2^5
#define MAX_NUM_TASKS 128 // 2^7
#define MAX_NUM_PHASES 8 // 2^3
#define MAX_NUM_CONNS 1024 // 2^10

#define MAX_NUM_SIMPLEX_CONN 32 // 2^5

#define MAX_MSG_BYTESIZE 4096 // TODO: can we assume the uniform size?

#define MAX_MSG_QUEUE_SIZE 128 // expected maximum msg queue length

#endif /* PALS__PALS_BASE_H */
