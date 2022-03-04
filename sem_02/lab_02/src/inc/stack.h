#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>

#define STACK_ERROR 2
#define ALLOCATION_ERROR 3

typedef struct entry
{
    char            *symbolic_name;
    char            *path;
    uint8_t         is_dir :1;
    unsigned long   ino;
    size_t          level;
} entry_t;

struct dirstack;

typedef struct dirstack dirstack_t;

dirstack_t *init();

void push(dirstack_t *const stack, entry_t elem);

entry_t pop(dirstack_t *const stack);

uint8_t is_empty(const dirstack_t *const stack);

void free_entry(entry_t *entry);

void free_stack(dirstack_t **stack);
