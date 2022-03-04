#pragma once

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "stack.h"
#include "utils.h"

void process_entry(dirstack_t *const stack, const size_t level, char *cur_entry);

void process_dir(dirstack_t *const stack, const entry_t cur_el, struct dirent *dirp);

void dirtree(char *path);
