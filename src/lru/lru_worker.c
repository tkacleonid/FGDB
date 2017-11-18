#include "lru_worker.h"

// ammount of hot pages we are OK with
const QUEUE_OK = 5;
const SLEEPER_NSEC = 500000;

void* process_queue(void* queue) {
	lru_queue_t* q = (lru_queue_t*) queue;
	arena_page_t* p = NULL;
	struct timespec sleeper;
	sleeper.tv_sec  = 0;
	sleeper.tv_nsec = SLEEPER_NSEC;
	while (true) {
		if (q->queued > QUEUE_OK) {
			p = pop_queue(q);
			if (p) dump_page(p);
		} else {
			nanosleep(&sleeper, NULL);
		}
	}
	destroy_lru_queue(q);
	return NULL;
}

uint32_t start_lru_worker(lru_queue_t* q) {
	pthread_t pt;
	pthread_create (pthread_t* &pt, NULL, process_queue, (void*) q);
}
