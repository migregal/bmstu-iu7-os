#include <consumer.h>

#include <stdio.h>

#include <sys/sem.h>

#include <time.h>
#include <unistd.h>

struct sembuf C_LOCK[2] = {{BUF_FULL, -1, 0}, {BIN_SEM, -1, 0}};
struct sembuf C_RELEASE[2] = {{BUF_EMPTY, 1, 0}, {BIN_SEM, 1, 0}};

int c_run(cbuf_t *const buf, const int sem_id, const int c_id) {
    if (!buf)
        return -1;

    srand(time(NULL) + c_id + P_COUNT);

    int stime;
    char ch;

    for (size_t i = 0; i < ITERS; ++i) {
        stime = rand() % MAX_RAND_C + 1;
        sleep(stime);

        if (semop(sem_id, C_LOCK, 2) == -1) {
            perror("consumer lock error");
            exit(1);
        }

        if (read_buf(buf, &ch) == -1) {
            perror("buffer read error");
            return EXIT_FAILURE;
        }

        printf("=> consumer #%d read:  %c // Idle time: %ds\n", c_id, ch, stime);

        if (semop(sem_id, C_RELEASE, 2) == -1) {
            perror("consumer release error");
            exit(1);
        }
    }

    return EXIT_SUCCESS;
}
