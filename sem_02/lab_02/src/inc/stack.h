#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>

#define STACK_ERROR 2
#define ALLOCATION_ERROR 3

struct dirstack;

typedef struct dirstack dirstack_t;

dirstack_t *init(void (*el_free)(void **));

void push(dirstack_t *const stack, void *elem);

void *pop(dirstack_t *const stack);

uint8_t is_empty(const dirstack_t *const stack);

void free_stack(dirstack_t **stack);
