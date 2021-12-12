#include <writer.h>

#include <monitor.h>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <unistd.h>

int w_run(int *const shcntr, const int sem_id, const int w_id) {
    if (!shcntr)
        return -1;

    srand(time(NULL) + w_id + READERS_COUNT);

    for (size_t i = 0; i < ITERS; ++i) {
        int stime = rand() % MAX_RAND + 1;
        sleep(stime);

        if (start_write(sem_id)) {
            perror("writing start error");
            exit(1);
        }

        int val = ++(*shcntr);
        printf("<= Writer #%d wrote: %3d // Idle time: %ds\n", w_id, val, stime);

        if (stop_write(sem_id)) {
            perror("writing end error");
            exit(1);
        }
    }

    return EXIT_SUCCESS;
}
