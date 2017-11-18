#ifndef LRUWORKER_H
#define LRUWORKER_H

#include <pthread.h>
#include <time.h>
#include "common.h"
#include "lruq.h"

void* process_queue(void* q);

uint32_t start_lru_worker(lru_queue_t* q);

#endif
