#ifndef LRUQ_H
#define LRUQ_H

#include "common.h"
#include "arena/arena.h"

typedef struct lru_node {
	arena_page_t* page;
	uint32_t timestamp;
	lru_node_t* prev;
	lru_node_t* next;
} lru_node_t;

typedef struct lru_queue {
	lru_node_t* first;
	lru_node_t* last;
} lru_queue_t;

lru_node_t* page_to_lru(arena_page_t* p);

lru_queue_t* new_queue(void);

void relink_lru_node(lru_node_t* ln);

void push_lru(lru_queue_t* q, arena_page_t* p);

arena_page_t* pop_lru(lru_queue_t* q);

void destroy_lru_node(lru_node_t* ln);

void destroy_lru_queue(lru_queue_t* lq);
