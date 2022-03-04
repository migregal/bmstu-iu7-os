#pragma once

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "stack.h"
#include "utils.h"

void dirtree(char *path);
