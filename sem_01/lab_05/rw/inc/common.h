#pragma once

#define MAX_SEMS 4
#define ITERS 20

#define MAX_RAND 3

#ifdef COLORIZED
#define WRITER_COLOR "\033[0;32m"
#define READER_COLOR "\033[0;37m"
#define RESET_COLOR "\033[0m"
#else
#define WRITER_COLOR ""
#define READER_COLOR ""
#define RESET_COLOR ""
#endif
