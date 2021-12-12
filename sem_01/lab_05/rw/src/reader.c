#include <reader.h>

#include <monitor.h>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <unistd.h>

int r_run(int *const shcntr, const int sem_id, const int r_id) {
    if (!shcntr)
        return -1;

    srand(time(NULL) + r_id);

    for (size_t i = 0; i < ITERS; ++i) {
        int stime = rand() % MAX_RAND + 1;
        sleep(stime);

        if (start_read(sem_id)) {
            perror("reading start error");
            exit(1);
        }

        int val = *shcntr;
        printf("=> Reader #%d read:  %3d // Idle time: %ds\n", r_id, val, stime);

        if (stop_read(sem_id)) {
            perror("reading end error");
            exit(1);
        }
    }

    return EXIT_SUCCESS;
}
