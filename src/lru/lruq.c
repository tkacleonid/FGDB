#include "lruq.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

lru_node_t* page_to_lru (arena_page_t* p) {
	lru_node_t* ln = malloc(sizeof(lru_node_t));
	ln->page = p;
	ln->timestamp = (uint32_t) time(NULL);
	ln->prev = NULL;
	ln->next = NULL;
	return ln;
}

lru_queue_t* new_queue(void) {
	lru_queue_t* lq = malloc(sizeof(lru_queue_t));
	lq->first = NULL;
	lq->last  = NULL;
	return lq;
}

void relink_lru_node(lru_node_t* ln) {
	if (ln->next != NULL) {
		ln->next->prev = ln->prev;
	}

	if (ln->prev != NULL) {
		ln->prev->next = ln->next;
	}
}

void push_lru(lru_queue_t* q, arena_page_t* p) {
	lru_node_t* ln = page_to_lru(p);
	q->last->prev = ln;
	ln->next = q->last;
	q->last = ln;
}

arena_page_t* pop_lru(lru_queue_t* q) {
	lru_node_t* cur_node;
	arena_page_t* p;
	if (q->first == NULL) {
		return NULL;
	}
	cur_node = q->first;
	while (cur_node->page->header.lock != UNLOCKED_PAGE) {
		cur_node = cur_node->prev;
		if (cur_node == NULL) {
			return NULL;
		}
	}
	p = cur_node->page;
	destroy_lru_node(cur_node);
	return p;
}

void destroy_lru_node(lru_node_t* ln) {
	relink_lru_node(ln);
	free(ln);
}

void destroy_lru_queue(lru_queue_t* q) {
	lru_node_t* cur_node = q->first;
	lru_node_t* del_node = NULL;

	while (cur_node != NULL) {
		del_node = cur_node;
		cur_node = cur_node->prev;
		destroy_lru_node(del_node);
	}
	free(q);
}
