#pragma once

#include <stdlib.h>
#include <inttypes.h>

typedef struct tree_entry {
    char            *symbolic_name;
    char            *path;
    uint8_t         is_dir :1;
    unsigned long   ino;
    size_t          level;
} tree_entry_t;
