#pragma once

#include <common.h>

#define READERS_COUNT 5
#define WRITERS_COUNT 3

#define ACTIVE_READERS 0
#define ACTIVE_WRITERS 1

#define WAITING_READERS 2
#define WAITING_WRITERS 3

int start_read(int s_id);

int stop_read(int s_id);

int start_write(int s_id);

int stop_write(int s_id);
